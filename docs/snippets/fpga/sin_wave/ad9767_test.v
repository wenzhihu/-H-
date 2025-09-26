
//////////////////////////////////////////////////////////////////////////////////
//正选波发生器--2路输出 -10V ~ +10V
//////////////////////////////////////////////////////////////////////////////////
module ad9767_test(
    input clk,            //fpga clock
	 input rst_n,
	 output da1_clk,             //DA1 时钟信号
	 output da1_wrt,             //DA1 数据写信号
    output [13:0] da1_data,     //DA1 data
	 
	 output da2_clk,             //DA2 时钟信号
	 output da2_wrt,	           //DA2 数据写信号
    output [13:0] da2_data ,    //DA2 data
	 input 	uart_rxd,	//接收端口
	 output 	uart_txd	,//发送端口
	 input    back_en,
	 input back_num
    );
wire clk_back;
always@(posedge back_en or negedge rst_n )
			if(!rst_n)
				num<=34'd53687020;
					else if(a_fq==0&&b_fq==0)
					num<=num;
						else if(back_en==1)begin
					if(num>34'd53686800&&num<34'd53687240)begin
						if(back_num==1)
							num<=num+1;
							else 
								num<=num-1;
								end 
								else begin
									if(num<=34'd53686800)
										num<=num+1;
											else if(num>=34'd53687240)
												num<=num-1;
												else 
													num<=num;
								end
					end
				else num<=num;




wire clk_1;
wire clk_125;
wire [7:0] rxd/*synthesis keep*/;


assign da1_clk=clk_125;
assign da1_wrt=clk_125;
assign da1_data=rom_data_a;

assign da2_clk=clk_125;
assign da2_wrt=clk_125;
assign da2_data=rom_data_b;


reg  [13:0]rom_data_a;
reg  [13:0]rom_data_b;



reg   td/*synthesis noprune*/;



reg [7:0]  satea=0/*synthesis noprune*/;


reg  [7:0] rxd_1;


reg  [7:0]a_wave;
reg  [7:0]a_fq;
reg  [7:0]b_wave;
reg   [7:0]b_fq;  
reg  [7:0] phase=0;



reg [33:0] num=34'd53687020;
//////////////////////////////////////////////////////////

always@(posedge clk or negedge rst_n)
	if(!rst_n)
		rom_data_a<=0;
			else if(a_wave==8'h13)
			rom_data_a<=rom_data_a_sin;
				else if(a_wave==8'h17)
					rom_data_a<=rom_data_a_tri;
						else 
						rom_data_a<=rom_data_a;

always@(posedge clk or negedge rst_n)
	if(!rst_n)
		rom_data_b<=0;
			else if(b_wave==8'h13)
			rom_data_b<=rom_data_b_sin;
				else if(b_wave==8'h17)
					rom_data_b<=rom_data_b_tri;
						else 
						rom_data_b<=rom_data_b;

always@(posedge clk or negedge rst_n)
	if(!rst_n)
		NUM_a <= num[31:0];
			else if(a_fq!=100)
					NUM_a<=(num*a_fq)/100;
					else NUM_a<=num[31:0];
always@(posedge clk or negedge rst_n)
	if(!rst_n)
		NUM_b <= num[31:0];
		
			else if(b_fq!=100)
					NUM_b<=(num*b_fq)/100;
						else NUM_b<=num[31:0];


/////////////////////////////////////

always@(posedge clk or negedge rst_n)
	if(!rst_n)
		td<=0;
		else if(rxd==8'h88)begin
			td<=1;
			end
			else if(rxd==8'h99)
				 td<=0;
					else 
					td<=td;
always @(posedge clk or negedge rst_n)
		if(!rst_n)	begin
			satea<=0;
			a_wave<=0;
			b_wave<=0;
			rxd_1<=0;
			a_fq <=0;
			b_fq <=0;
			end
			else if(td==1)
				case(satea)
					0:
					if((rxd_1!=rxd)&(rxd!=0))begin
					 //a_wave<=rxd;
					rxd_1<=rxd;
					satea<=satea+1;
					end
					1:if((rxd_1!=rxd)&(rxd!=0))begin
					 a_wave <=rxd;
					rxd_1<=rxd;
					satea<=satea+1;
					end
					2:if((rxd_1!=rxd)&(rxd!=0))begin
					 a_fq <=rxd;
					rxd_1<=rxd;
					satea<=satea+1;
					end
					3:if((rxd_1!=rxd)&(rxd!=0))begin
					 b_wave <=rxd;
					rxd_1<=rxd;
					satea<=satea+1;
					end
					4:if((rxd_1!=rxd)&(rxd!=0))begin
					 b_fq <=rxd;
					rxd_1<=rxd;
					satea<=satea+1;
					end
					5:if((rxd_1!=rxd)&(rxd!=0))begin
					 phase <=rxd;
					rxd_1<=rxd;
					satea<=satea+1;
					end
					6:if((rxd_1!=rxd)&(rxd!=0))begin
					satea<=0;
					rxd_1<=rxd;
					end
					default:begin
					rxd_1<=0;
					satea<=0;
					end
				endcase	

///////////////////////////////////////////////////
reg [8:0] add_a=0;
reg [8:0] add_b=0;
reg  [7:0] cnt_a;
reg  [7:0] cnt_b;
wire [13:0] rom_data_a_sin;
wire [13:0] rom_data_a_tri;
wire [13:0] rom_data_b_sin;
wire [13:0] rom_data_b_tri;
reg [31:0]NUM_a ;
reg [31:0]NUM_b ;
reg [31:0]rom_addr_a=0;
reg [31:0]rom_addr_b=0;
wire [8:0] rom_phase_1;
assign rom_phase_1=add_b+phase;

always@(posedge clk or negedge rst_n)
		if(!rst_n)
			add_a<=0;
			else 
				add_a<=rom_addr_a[31:23];

always@(posedge clk_125 or negedge rst_n )
		if(!rst_n)begin
		cnt_a<=0;
		rom_addr_a<=32'd0;
		end 
		else if(cnt_a<24)
			cnt_a<=cnt_a+1;
		else begin
			cnt_a<=0;
			if(rom_addr_a==32'd0)
				rom_addr_a<=32'd1;
			else 
				rom_addr_a<=rom_addr_a+NUM_a;
		end 
		
		
//////////////////////////////////////////////		
always@(posedge clk or negedge rst_n)
		if(!rst_n)
			add_b<=0;
			else if(phase!=1)
					add_b<=(b_fq/a_fq)*add_a;
					else 
				add_b<=rom_addr_b[31:23];

always@(posedge clk_125 or negedge rst_n )
		if(!rst_n)begin
		cnt_b<=0;
		rom_addr_b<=32'd0;
		end 
		else if(cnt_b<24)
			cnt_b<=cnt_b+1;
		else begin
			cnt_b<=0;
			if(rom_addr_b==32'd0)
				rom_addr_b<=32'd1;
			else 
				rom_addr_b<=rom_addr_b+NUM_b;
		end 
ROM ROM_inst (
  .clock   (clk_125), 
  .address (add_a), 
  .q       (rom_data_a_sin) 
);


rom_1 u_rom(
	.clock (clk_125),
	.address (add_a),
	.q       (rom_data_a_tri)
 
);
rom_2 u2(
	.clock (clk_125),
	.address (rom_phase_1),
	.q       (rom_data_b_sin)



);
rom_3 u3(
	.clock (clk_125),
	.address (rom_phase_1),
	.q       (rom_data_b_tri)




);



PLL PLL_inst(
	.areset  (1'b0),
	.inclk0  (clk),
	.c0      (clk_1),
	.c1      (clk_125),
	.c2		(clk_back),//10khz
	.locked  ()
	);
uart_top u_uart_top(
		.sys_clk(clk),
		.sys_rst_n(rst_n),
		.uart_rxd(uart_rxd),
		.uart_txd(uart_txd),
		.rxd(rxd)
		



);
endmodule
