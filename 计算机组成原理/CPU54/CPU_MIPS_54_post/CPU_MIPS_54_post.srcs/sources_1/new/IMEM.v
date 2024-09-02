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
    input [10:0] IMEM_addr_in,     //11�f???????????IMEM?�\????
    output [31:0] IMEM_inst_out   //32�f?????
    ); 

dist_mem_gen_0 imem(    //?????IP?????????????????????????
    .a(IMEM_addr_in),     //????IMEM?????
    .spo(IMEM_inst_out)
    );
endmodule

