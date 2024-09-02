`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 16:44:16
// Design Name: 
// Module Name: IMEM
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


module IMEM(
    input [10:0] IMEM_addr_in,     //11位指令码地址，从IMEM中读指令
    output [31:0] IMEM_inst_out   //32位指令码
    ); 

dist_mem_gen_0 imem(    //实例化IP核，输入指令码地址返回对应的指令
    .a(IMEM_addr_in),     //接口和IMEM模块对应
    .spo(IMEM_inst_out)
    );
endmodule

