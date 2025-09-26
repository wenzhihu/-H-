module ad(
          input ad_clk,
			 input [11:0] ad1_in,
			 input [11:0] ad2_in,
			 
			 output reg [11:0] ad_ch1,
			 output reg [11:0] ad_ch2
    );



always @(posedge ad_clk)
begin
    ad_ch1 <= ad1_in;    	 
	 ad_ch2 <= ad2_in;
end 


endmodule