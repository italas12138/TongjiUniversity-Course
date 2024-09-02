`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 14:20:35
// Design Name: 
// Module Name: DIV
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


module DIV(
    input sign_ena,

    input [31:0] DIV_A,
    input [31:0] DIV_B,

    output [31:0] R,
    output [31:0] Q
    );


wire signed [31:0] signed_A;  //有符号被除数
wire signed [31:0] signed_B;  //有符号除数

assign signed_A = DIV_A;
assign signed_B = DIV_B;

assign R =  (DIV_B == 32'd0) ? 32'd0 : (sign_ena ? (signed_A % signed_B) : (DIV_A % DIV_B));
assign Q =  (DIV_B == 32'd0) ? 32'd0 : (sign_ena ? (signed_A / signed_B) : (DIV_A / DIV_B));


endmodule
