`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 14:14:50
// Design Name: 
// Module Name: MUL
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


module MUL(
    input sign_ena,

    input [31:0] MUL_A,
    input [31:0] MUL_B,

    output [31:0] HI,
    output [31:0] LO
    );


wire [63:0] result;                 //结果统一存储在result中
wire [63:0] unsigned_result;        //保存无符号乘法结果
wire signed [63:0] signed_result;   //保存有符号乘法结果
wire [63:0] unsigned_A;             //扩展的无符号A
wire [63:0] unsigned_B;             //扩展的无符号B
wire signed [63:0] signed_A;        //扩展的有符号A
wire signed [63:0] signed_B;        //扩展的有符号B

assign unsigned_A = { 32'd0, MUL_A };
assign unsigned_B = { 32'd0, MUL_B };
assign unsigned_result = unsigned_A * unsigned_B;

assign signed_A = { {32{MUL_A[31]}} , MUL_A };
assign signed_B = { {32{MUL_B[31]}} , MUL_B };
assign signed_result = signed_A * signed_B;

assign result = sign_ena ? signed_result : unsigned_result;

assign HI = result[63:32];
assign LO = result[31:0];
endmodule
