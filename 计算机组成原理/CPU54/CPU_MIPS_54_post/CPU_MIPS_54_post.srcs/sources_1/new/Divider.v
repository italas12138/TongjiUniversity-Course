`timescale 1ns / 1ps
module Divider(clk,rst_n,clk_out);
/* ?������������� */
input clk;                //???��
input rst_n;              //����?�,�?�?��?
output reg clk_out;       //�������CPU��?��
/* ?���?���� */
reg [31:0] count3=32'd0; //50,000,000��?
//50,000,000��?
always @(posedge clk) 
begin 
    if(!rst_n)
    begin
      count3 <= 1'b0;
      clk_out <= 0;  
    end
    else if(count3 == 32'd50000000) 
    begin 
        count3 <= 32'd0; 
        clk_out <= ~clk_out; 
    end 
    else
        count3 <= count3+1'b1; 
end
endmodule