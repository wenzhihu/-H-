# 代码库结构与学习指引

本项目源自 2023 年全国大学生电子设计竞赛 H 题的国一等奖方案，围绕“高速采集 + 波形合成 + FFT/相位反馈”构建了一套 FPGA + STM32 的协同系统。仓库当前以两个压缩包保存原始工程文件，我们提取了核心源码供浏览，并在下文概述其职责与学习建议。

## 顶层组成

- `README.md`：项目简要介绍，说明方案使用 FFT 获取相位/频率并通过相位差反馈稳定输出。【F:README.md†L1-L5】
- `di.7z`：包含两个 Quartus FPGA 工程——`adc_fifo_spi`（高速采集与 SPI 回传）和 `sin_wave`（双通道波形合成）。
- `finally.zip`：包含基于 STM32F407 的 Keil 工程，负责 FFT 计算、LCD 可视化、键盘/按键交互以及与 FPGA 的 SPI 通信。
- `docs/snippets/`：提取出的关键源码片段，便于快速检索具体逻辑。

## FPGA 工程

### `adc_fifo_spi`：AD7606 采样与 SPI 回传

该工程的顶层模块 `adc` 将双路 AD7606 数据采样后写入 FIFO，再通过 SPI 从端将数据返回给 MCU，同时向 AD 芯片提供采样时钟和工作状态控制。【F:docs/snippets/fpga/adc_fifo_spi/adc.v†L1-L120】

核心子模块：

- `ad.v`：对 AD7606 双通道数据做简单同步寄存，保证 FIFO 写入时序稳定。【F:docs/snippets/fpga/adc_fifo_spi/ad.v†L1-L17】
- `fifo_rd.v`：检测 FIFO 满空标志，控制读出节拍，在 SPI 读取完成后复位状态机。【F:docs/snippets/fpga/adc_fifo_spi/fifo_rd.v†L1-L83】
- `spi.v`：实现 SPI 从机收发状态机，在 SCK 上升沿采样、下降沿输出，同时检测指令字（0x88/0x99 等）调节数据路径。【F:docs/snippets/fpga/adc_fifo_spi/spi.v†L1-L120】

### `sin_wave`：双通道 DDS 波形合成

`ad9767_test` 顶层管理与 MCU 的 UART 交互，根据配置生成双通道波形并驱动 AD9767 DAC，同时保留反馈链路（`back_en/back_num` 用于调节数控振荡频率）。【F:docs/snippets/fpga/sin_wave/ad9767_test.v†L1-L160】

- 内部使用 `ad7606` 模块读取 8 路模拟量，支持过采样倍率设置、忙标志轮询和逐通道读出控制。【F:docs/snippets/fpga/sin_wave/ad7606.v†L1-L205】
- 波形数据来自若干 ROM 表（`rom_*.v`）和相位累加器，通过 `phase`、`NUM_a/NUM_b` 等寄存器调整频率和相位，结合 MCU 写入的波形类型 `a_wave/b_wave`、倍频 `a_fq/b_fq` 等指令生成输出码字。

## STM32F4 固件工程

### 主控流程

- `User/main.c`：系统初始化（时钟、GPIO、LCD、SPI、EXTI）、主循环和相位反馈逻辑。主循环周期性向 FPGA 请求采样 (`Add_SPI`)、将数据转换为幅值/相位 (`Deal_Data`、`CalXiebo`、`CalShuchu2`)，根据相位差 `delta` 控制输出以及刷新 LCD 与串口。【F:docs/snippets/mcu/User/main.c†L1-L133】

### 关键功能模块

- `APP/fft/ffttest.c`：封装基于 CMSIS DSP 库的 FFT 处理流程，计算主频幅值、谐波判断以及相位（`CalXiebo`、`CalShuchu1/2`），并输出频率、波形类别等决策结果供显示与反馈。【F:docs/snippets/mcu/APP/fft/ffttest.c†L1-L120】
- `APP/spi/spi.c`：初始化 SPI1/SPI2、配置 GPIO 以及与 FPGA 的片选/复位引脚控制，同时提供字节级读写接口与速度设定函数。【F:docs/snippets/mcu/APP/spi/spi.c†L1-L120】
- `APP/exti/exti.c`：配置按键外部中断，提供相位设定步进、波形显示刷新等交互逻辑，并对特殊控制码（0x88/0x99）做过滤处理，防止与通信协议冲突。【F:docs/snippets/mcu/APP/exti/exti.c†L1-L120】

固件还包含 LCD 绘图、按键扫描、数据准备 (`data.c`) 等模块，可按需阅读。

## 入门重点

1. **数据链路理解**：梳理“AD7606 → FPGA FIFO → SPI → MCU FFT → LCD/反馈”闭环，确认各阶段的时序约束和控制信号。【F:docs/snippets/fpga/adc_fifo_spi/adc.v†L1-L120】【F:docs/snippets/mcu/User/main.c†L78-L133】
2. **FFT 与决策逻辑**：重点阅读 `ffttest.c` 中的 `CalXiebo`、`CalShuchu1/2`，理解如何定位主频、计算相位以及区分波形类型，为后续算法优化打基础。【F:docs/snippets/mcu/APP/fft/ffttest.c†L31-L120】
3. **交互与反馈**：`exti.c` 的按键中断与 `main.c` 的相位比较共同实现相位锁定；结合 `spi.c` 了解 MCU 与 FPGA 的命令/数据协议，确保软硬件配合。【F:docs/snippets/mcu/APP/exti/exti.c†L53-L112】【F:docs/snippets/mcu/APP/spi/spi.c†L1-L120】

## 后续学习建议

- **硬件平台**：熟悉 AD7606/AD9767 数据手册以及 SPI、FIFO IP 核的时序要求，理解代码中的魔数（如延时计数、命令字节）的来源。
- **信号处理**：复现 FFT 分析流程，尝试对 `CalXiebo` 中的阈值（`cisan`、`jiaosan`）做数据驱动的调整，或引入窗函数/插值提高频率分辨率。
- **系统优化**：
  - 在 FPGA 侧，可评估使用流水线或 DMA 改进 FIFO 访问；
  - 在 MCU 侧，考虑将关键运算迁移至定时器中断或 RTOS 任务，提升实时性；
  - 增加串口/液晶调试输出，帮助定位相位漂移或通信异常。
- **文档化与自动化**：整理硬件连线图、测试流程，并构建脚本（如 Python）生成测试波形与对比曲线，降低回归验证成本。

通过以上路径，新人能够快速把握系统架构，再逐步深入各模块的实现细节与优化空间。
