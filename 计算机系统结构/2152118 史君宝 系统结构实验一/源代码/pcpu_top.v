`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    pcpu¶¥²ãÄ£¿é
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module pcpu_top(
    input clk,
    input rst,
    output [31:0] inst,
    output [31:0] pc
    ); 
        
    wire [31:0] a;
    assign a = pc - 32'h00400000;

    imem imem(a[12:2], inst);
    pcpu pcpu(clk,rst,inst,pc);
endmodule