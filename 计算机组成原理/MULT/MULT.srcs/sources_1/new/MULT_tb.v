`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/03/28 18:12:50
// Design Name: 
// Module Name: MULTU_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module MULT_tb;

    reg [31:0] a;
    reg [31:0] b;
    reg clk;
    reg reset;
    wire [63:0] z;
    MULT uut(.a(a),.b(b),.clk(clk),.reset(reset),.z(z));

    initial
    begin
        clk=0;
        reset=1;
        a=0;
        b=0;

        #3;
        clk=~clk;
        a=32'b00000000000000000000000000000000;
        b=32'b11111111111111111111111111111111;

        #3;
        reset=0;
        
        repeat (21)
        #5 clk=~clk;

        #3;
        reset=1;
    end

endmodule
