module adc(
		input clk,
		input rst_n,
		input  sys_rst,
		input mosi,
		output miso,
		input sck,
		input cs,
		input [11:0] ad1_in,
		output ad1_clk,
		output reg spi_out=1,
		output en,
		input [11:0] ad2_in,
		output ad2_clk	
		


);

wire clk0_64m;//the real is 1.28m

wire [11:0] ad_ch1;
wire [11:0] ad_ch2/*synthesis keep*/;
assign en=fifo_rd_en;
reg         wr_end=1;
wire         fifo_wr_en        ;  // FIFO写使能信号
wire           fifo_rd_en         ;  // FIFO读使能信号
wire  [11:0]  fifo_dout          ;  // 从FIFO读出的数据
wire         fifo_full          ;  // FIFO满信号
wire         fifo_empty         ;  // FIFO空信号
wire         spi_clk;
wire  [7:0] rxd/*synthesis keep*/;
reg   clk_s;
reg   [7:0]txd;
reg   [11:0] fifo_wr_data;
wire  [7:0] fifo_wr_num;

reg  manuall_rd_en=0;


always@(posedge clk or negedge rst_n)
		if(!rst_n)
			fifo_wr_data<=0;
				else if(fifo_wr_en==1)begin
					if(fifo_wr_num[0]==0)
						fifo_wr_data<=ad_ch1;
						else 
							fifo_wr_data<=ad_ch2;
							end 
						else fifo_wr_data<=fifo_wr_data;
always @(posedge clk or negedge sys_rst)
		if(!sys_rst)
		clk_s<=0;
		else if(rxd==8'h88)
			clk_s<=0;
				else if(rxd==8'h99)
					clk_s<=1;
						else
							clk_s<=clk_s;

assign spi_clk=clk_s;
assign ad1_clk=clk0_64m;
assign ad2_clk=clk0_64m;
assign fifo_wr_en=wr_end;
reg [7:0] cnt=0;	
always@(posedge clk or negedge sys_rst )
	if (!sys_rst)begin
		wr_end<=1;
			manuall_rd_en<=0;
			end 
	else if(fifo_full==1)
	begin
			wr_end<=0;
			manuall_rd_en<=1;
			end
			else if(fifo_empty==1)begin
					if(cnt<=10)
					cnt<=cnt+1;
						else begin
						cnt<=0;
						wr_end<=1;
						
						end
					manuall_rd_en<=0;	
					end
					else begin 
					wr_end<=wr_end;
					manuall_rd_en<=manuall_rd_en;
					end

always@(negedge clk or negedge sys_rst )
			if(!sys_rst)
				spi_out<=1;
			else if(rxd==8) begin
				spi_out<=0;
				end
				else 
				spi_out<=1;
					
				
					
always@(posedge clk or negedge rst_n or negedge sys_rst )
	if(!sys_rst)
		txd<=0;
	else  if(!rst_n)
		txd<=0;
			else if(rxd==8'h88)
				txd<=fifo_dout[7:0];
					else if(rxd==8'h99)
						txd<={4'd0,fifo_dout[11:8]};
							else 
							txd<=txd;
pll u_pll(
		.inclk0(clk),
		.c0(clk0_64m)

);
ad u2 (
		.ad_clk                  (clk0_64m),                           
		.ad1_in                  (ad1_in),             //ad1 input
	   .ad2_in                  (ad2_in),
      .ad_ch1                  (ad_ch1),           //ad1 data 12bit
		.ad_ch2                  (ad_ch2)  
 );
 
 fifo u_fifo(
		.data(fifo_wr_data),
		.wrclk(clk0_64m),
		.wrreq(fifo_wr_en),
		.wrfull(fifo_full),
		.rdreq((manuall_rd_en&fifo_rd_en)),
		.q(fifo_dout),
		.rdclk(spi_clk),
		.rdempty(fifo_empty ),
		.wrusedw(fifo_wr_num)
		


);
 fifo_rd  u_fifo_rd(
    .clk          ( clk   ),    // 读时钟
    .rst_n        ( sys_rst  ),    // 复位信号

    .fifo_rd_en   ( fifo_rd_en ),    // fifo读请求
    .fifo_dout    ( fifo_dout  ),    // 从FIFO输出的数据
    .empty        ( fifo_empty ),    // fifo空信号
    .full         ( fifo_full  )     // fifo满信号
);

spi u_spi(
		.clk(clk),
		.rst(rst_n),
		.sys_n(sys_rst),
		.cs(cs),
		.sck(sck),
		.MOSI(mosi),
		.MISO(miso),
		.rxd_out(rxd),
		.txd_data(txd),
		.rxd_flga()


);
 
 endmodule 