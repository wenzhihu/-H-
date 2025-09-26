/*******************************************************************************
*                 
*                 		       пƼ
--------------------------------------------------------------------------------
* ʵ  		 : ⺯ģ崴
* ʵ˵       : 
* ӷʽ       : 
* ע    		 : 	
*******************************************************************************/

#include "stm32f4xx.h"
#include "arm_math.h"

#include "SysTick.h"
#include "system.h"
#include "usart.h"

#include "led.h"
#include "tftlcd.h"
#include "my_lcd.h"
#include "data.h"
#include "ffttest.h"
#include "exti.h"
#include "spi.h"
#include "key.h"
/*******************************************************************************
*            : main
* 		   : 
*              : 
*              : 
*******************************************************************************/
////	Deal_Data(test_t,test,6);
////	LCD_DL_F(50,600,FDATA_X,test,6,BLACK);
//	
////	Draw_Coordinate(50, 400,400,300,1);
//	Draw_Coordinate(50, 600,400,300,1);
//////	Creat_TriWave(1,1.5,0,SIZE,phaseB);
//	Creat_Wave(2,1,120,SIZE,phaseA);	
//	Creat_Wave(1,3,0,SIZE,phaseB);
//	Add_Wave(phaseA,phaseB,FDATA_X,xiebo,SIZE);
//	LCD_DL_F(50,600,FDATA_X,xiebo,SIZE,RED);
//	CalXiebo(xiebo);
////	CalPhase_m(xiebo);
//	
////	u8 i=0;
//	u16 color=1;

extern float FDATA_X[SIZE];
extern float FDATA_Y[SIZE];
extern uint16_t SPI_FPGA[SIZE];
extern uint8_t SETPHASE;
extern uint8_t FREA,FREB,KINDA,KINDB;
extern float xiebo[128];
extern uint16_t SPI_FPGA1[128],SPI_FPGA2[128]; //spi
extern float RESULT1[128],RESULT2[128];  //exti
float phase_1,phase_2;
float delta[2]={0,0},mid;
uint8_t ALLSTART;
uint8_t SHOWSTART;
extern uint8_t WAVEPHASE;
int main(void)
{
	SysTick_Init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	LED_Init();
	KEY_Init();
	My_EXTI_Init();
	USART1_Init(9600);
	TFTLCD_Init();
	SPI1_Init();
//	LCD_ShowString(10,50,12*6,24,24,"hello");
	LCD_ShowNum(50,20,5,2,16);
	LCD_ShowFontHZ_16(10, 20,"");
	LCD_ShowChar(42,20,':',16,0);
	LCD_ShowFontHZ(10, 55,"λ");
	LCD_ShowChar(138,59,':',24,0);
	LCD_ShowNum(150,59,SETPHASE,3,24);
	
	LCD_ShowChar(230,29,'A',24,0);
	LCD_ShowChar(242,29,':',24,0);
	LCD_ShowChar(230,59,'B',24,0);
	LCD_ShowChar(242,59,':',24,0);
	delay_ms(5000);
	
//	Creat_Wave(1,1,250,SIZE,phaseA);
//	Creat_Wave(1,2,0,SIZE,phaseB);
//	Add_Wave(phaseA,phaseB,FDATA_X,xiebo,SIZE);
//	CalXiebo(xiebo);
//	LCD_DL_F(50,600,FDATA_X,xiebo,SIZE,RED);
////	FREA=85,FREB=65;
//	KINDA=0x13,KINDB=0x17;  //13
	uint32_t i=0;
//	Draw_Coordinate(50,600,400,300,1);
	while(1)
	{
//		if(ALLSTART==1)
//		{
			
			Add_SPI(SPI_FPGA2,SPI_FPGA1,128*2);  //2ΪC
			delay_ms(1);
			Add_SPI(SPI_FPGA2,SPI_FPGA1,128*2);
			if(FREA==0||FREB==0)
			{
				FPGA_RESET_0;delay_ms(5);
				FPGA_RESET_1;delay_ms(5);
			}
			Deal_Data(SPI_FPGA1,RESULT1,128);   
			Deal_Data(SPI_FPGA2,RESULT2,128);  //c
//			LCD_DL_F(50,625,RESULT1,RESULT1,SIZE,BLACK);
			
			phase_1=CalXiebo(RESULT2);
//			phase_2=CalShuchu(RESULT1);
			
//			phase_1=CalShuchu1(RESULT2);
			phase_2=CalShuchu2(RESULT1);
			
			mid=delta[1];
			if(phase_1<phase_2) phase_1+=360.0;
			delta[1]=phase_1-phase_2;//((phase_1-phase_2>0)?phase_1-phase_2:phase_2-phase_1);
			delta[0]=mid;
			mid=delta[1]-delta[0];			
			
			if(mid>0.0)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_SET);  //F9   LED1
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);  //F10ʹ
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);
			}
			else if(mid<0.0)
			{
				GPIO_WriteBit(GPIOF,GPIO_Pin_9,Bit_RESET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_SET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_10,Bit_RESET);
			}
		
//		}
		
		if(SHOWSTART==1)
		{
//			CalXiebo(RESULT2);
			Usart_Mes(KINDA,FREA,KINDB,FREB,WAVEPHASE);
			LCD_Fill(10,200,400,800,WHITE);
			Show_Wavemessage(KINDA,KINDB,FREA,FREB);
			Draw_Coordinate(50,400,400,300,1);
			LCD_DL_F(50,400,RESULT2,RESULT2,SIZE,BLACK);
			SHOWSTART=0;
		}
				
	}
}


