`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    Êý¾Ý´æ´¢Ä£¿é
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module dmem(
    input clk,
    input rst,
    input write,
	input [31:0] addr,
    input [31:0] idata,
    output [31:0] odata
    );
    
	integer i;
    reg [31:0] memory[0:2047];
    wire [31:0] addr_real;
    
    assign addr_real = addr - 32'h10010000;
	assign odata = memory[addr_real[10:0]];

	always @(negedge clk or posedge rst) begin
        if(rst==1) begin
            for(i=0;i<2048;i=i+1) memory[i] <= 0; end
        else if(write==1) begin
            memory[addr_real[10:0]] <= idata; end
	end
endmodule