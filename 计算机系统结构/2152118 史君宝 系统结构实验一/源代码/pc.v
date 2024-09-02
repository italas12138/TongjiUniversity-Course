`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    PC¼Ä´æÆ÷Ä£¿é
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module PC(
	input clk,
    input rst,
    input [31:0] PC_in,
    output reg [31:0] PC_out
    );
	
    always@(posedge clk or posedge rst) begin
        if(rst==1)
            PC_out <= 32'h00400000;
        else
            PC_out <= PC_in;  
    end
endmodule