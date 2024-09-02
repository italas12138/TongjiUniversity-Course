`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    超前加减运算模块
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module cla4(
	input [3:0] g, p,
	input in,
	output og,	//4位超前进位加法器的进位产生函数
	output op,	//4位超前进位加法器的进位传递函数
	output [4:1] out
	);
	assign out[1] = g[0]|(p[0]&in); 
	assign out[2] = g[1]|(p[1]&g[0])|(p[1]&p[0]&in); 
	assign out[3] = g[2]|(p[2]&g[1])|(p[2]&p[1]&g[0])|(p[2]&p[1]&p[0]&in); 
	assign out[4] = g[3]|(p[3]&g[2])|(p[3]&p[2]&g[1])|(p[3]&p[2]&p[1]&g[0])
					|(p[3]&p[2]&p[1]&p[0]&in); 
	assign og = g[3]|(p[3]&g[2])|(p[3]&p[2]&g[1])|(p[3]&p[2]&p[1]&g[0]);
	assign op = p[3]&p[2]&p[1]&p[0];
endmodule

module add1(
	input a, b,
	input carry,
	output g,	//1位超前进位加法器产生函数
	output p,	//1位超前进位加法器的传递函数
	output out
	);
	assign g = a & b;
	assign p = a | b;
	assign out = a ^ b ^ carry;
endmodule

module add4(
	input [3:0] a,b,
	input in,
	output og,	//4位超前进位加法器的进位产生函数
	output op,	//4位超前进位加法器的进位传递函数
	output [3:0] s
	);
	wire [3:0] g, p;
	wire [4:1] carry;
	add1 add_1(a[0], b[0], in, g[0], p[0], s[0]);
	add1 add_2(a[1], b[1], carry[1], g[1], p[1], s[1]);
	add1 add_3(a[2], b[2], carry[2], g[2], p[2], s[2]);
	add1 add_4(a[3], b[3], carry[3], g[3], p[3], s[3]);
	cla4 cla4(g, p, in, og, op, carry);
endmodule

module add16(
	input [15:0] a,b,
	input in,
	output og,	//16位超前进位加法器的进位产生函数
	output op,	//16位超前进位加法器的进位传递函数
	output [15:0] s
	);
	wire [3:0] g,p;
	wire [4:1] carry;
	add4 add_1(a[3:0], b[3:0], in, g[0], p[0], s[3:0]);
	add4 add_2(a[7:4], b[7:4], carry[1], g[1], p[1], s[7:4]);
	add4 add_3(a[11:8], b[11:8], carry[2], g[2], p[2], s[11:8]);
	add4 add_4(a[15:12], b[15:12], carry[3], g[3], p[3], s[15:12]);
	cla4 cla4(g, p, in, og, op, carry);
endmodule

module add32(
	input [31:0] a,b,
	input in,
	output [31:0] r,
	output out
	);
	wire temp;
	wire [1:0] g, p;
	assign temp = g[0]|(p[0]&in);
	assign out = g[1]|(p[1]&g[0])|(p[1]&p[0]&in);
	add16 add_1(a[15:0], b[15:0], in, g[0], p[0], r[15:0]);
	add16 add_2(a[31:16], b[31:16], temp, g[1], p[1], r[31:16]);
endmodule

module neg32(
	input [31:0] in,
	input s,
	output [31:0] out
	);
	assign out[0] = s ^ in[0];
	assign out[1] = s ^ in[1];
	assign out[2] = s ^ in[2];
	assign out[3] = s ^ in[3];
	assign out[4] = s ^ in[4];
	assign out[5] = s ^ in[5];
	assign out[6] = s ^ in[6];
	assign out[7] = s ^ in[7];
	assign out[8] = s ^ in[8];
	assign out[9] = s ^ in[9];
	assign out[10] = s ^ in[10];
	assign out[11] = s ^ in[11];
	assign out[12] = s ^ in[12];
	assign out[13] = s ^ in[13];
	assign out[14] = s ^ in[14];
	assign out[15] = s ^ in[15];
	assign out[16] = s ^ in[16];
	assign out[17] = s ^ in[17];
	assign out[18] = s ^ in[18];
	assign out[19] = s ^ in[19];
	assign out[20] = s ^ in[20];
	assign out[21] = s ^ in[21];
	assign out[22] = s ^ in[22];
	assign out[23] = s ^ in[23];
	assign out[24] = s ^ in[24];
	assign out[25] = s ^ in[25];
	assign out[26] = s ^ in[26];
	assign out[27] = s ^ in[27];
	assign out[28] = s ^ in[28];
	assign out[29] = s ^ in[29];
	assign out[30] = s ^ in[30];
	assign out[31] = s ^ in[31];
endmodule

module adder(
	input [31:0] a,
	input [31:0] b,
	input aluc,
	output [31:0] r
	);
	wire [31:0] b_real;
	wire carry_in;
	wire carry_out;
	assign carry_in = aluc ? 1: 0;
	neg32 neg32(b, aluc, b_real);
	add32 add32(a, b_real, carry_in, r, carry_out);
endmodule