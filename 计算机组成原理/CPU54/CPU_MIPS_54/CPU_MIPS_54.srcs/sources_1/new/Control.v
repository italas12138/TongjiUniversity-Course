`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 16:46:19
// Design Name: 
// Module Name: Control
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




module Control(
    input [53:0] op_ena,     //54条指令对应的编码
    input Z,           //比较指令用，0标志位
    input N,

    output ref_w,              //RegFile寄存器堆是否可写入
    output [5:0] ALU_choice,         //ALUC的指令，决定ALUC执行何种操作
    output DMEM_r,               //DMEM是否可写入
    output DMEM_w,               //是否从DMEM中读取数据
    output HI_w,               //HI是否可写入
    output LO_w,               //LO是否可写入

    output sign_ena,
    
    output GET_ena,
    output [4:0] CP0_choice,
    
    output EXT1_ena,
    output EXT5_ena,
    output EXT16_ena,
    output EXT16_sign_ena,
    output EXT18_sign_ena,
    
    output [2:0] MUX_Rd_choice,
    output [2:0] MUX_pc_choice,

    output [10:0] mux,         //10个多路选择器的状态（选择0还是选择1）(0没用到，为了使MUX编号和数组下标对应所以多一个)
    output [2:0]  mux6_1,      //6路选择器
    output [2:0]  mux7_1       //7路选择器
    );

/* 54条指令的状态编码 */
parameter ADDI  = 6'b000000;
parameter ADDIU = 6'b000001;
parameter ANDI  = 6'b000010;
parameter ORI   = 6'b000011;
parameter SLTIU = 6'b000100;
parameter LUI   = 6'b000101;
parameter XORI  = 6'b000110;
parameter SLTI  = 6'b000111;
parameter ADDU  = 6'b001000;
parameter AND   = 6'b001001;
parameter BEQ   = 6'b001010;
parameter BNE   = 6'b001011;
parameter J     = 6'b001100;
parameter JAL   = 6'b001101;
parameter JR    = 6'b001110;
parameter LW    = 6'b001111;
parameter XOR   = 6'b010000;
parameter NOR   = 6'b010001;
parameter OR    = 6'b010010;
parameter SLL   = 6'b010011;
parameter SLLV  = 6'b010100;
parameter SLTU  = 6'b010101;
parameter SRA   = 6'b010110;
parameter SRL   = 6'b010111;
parameter SUBU  = 6'b011000;
parameter SW    = 6'b011001;
parameter ADD   = 6'b011010;
parameter SUB   = 6'b011011;
parameter SLT   = 6'b011100;
parameter SRLV  = 6'b011101;
parameter SRAV  = 6'b011110;
parameter CLZ   = 6'b011111;
parameter DIVU  = 6'b100000;
parameter ERET  = 6'b100001;
parameter JALR  = 6'b100010;
parameter LB    = 6'b100011;
parameter LBU   = 6'b100100;
parameter LHU   = 6'b100101;
parameter SB    = 6'b100110;
parameter SH    = 6'b100111;
parameter LH    = 6'b101000;
parameter MFC0  = 6'b101001;
parameter MFHI  = 6'b101010;
parameter MFLO  = 6'b101011;
parameter MTC0  = 6'b101100;
parameter MTHI  = 6'b101101;
parameter MTLO  = 6'b101110;
parameter MUL   = 6'b101111;
parameter MULTU = 6'b110000;
parameter SYSCALL = 6'b110001;
parameter TEQ   = 6'b110010;
parameter BGEZ  = 6'b110011;
parameter BREAK = 6'b110100;
parameter DIV   = 6'b110101;



/* 下面是赋值，也就是根据要执行的操作决定各元器件的状态 */
assign ref_w = (!op_ena[JR]    && !op_ena[SW]      && !op_ena[BEQ] && !op_ena[BNE]  && !op_ena[J]     &&
                !op_ena[MTHI]  && !op_ena[MTLO]    && !op_ena[SB]  && !op_ena[SH]   && !op_ena[ERET]  &&
                !op_ena[BREAK] && !op_ena[SYSCALL] && !op_ena[TEQ] && !op_ena[MTC0] && !op_ena[MULTU] &&
                !op_ena[DIV]   && !op_ena[DIVU]    && !op_ena[BGEZ]) ? 1'b1 : 1'b0;



assign DMEM_r = (op_ena[LW] || op_ena[LB] || op_ena[LH] || op_ena[LBU] || op_ena[LHU])? 1'b1 : 1'b0;
assign DMEM_w = (op_ena[SW] || op_ena[SB] || op_ena[SH])? 1'b1 : 1'b0;


assign HI_w = (op_ena[MTHI] || op_ena[MULTU] || op_ena[DIV] || op_ena[DIVU]) ? 1'b1 : 1'b0;
assign LO_w = (op_ena[MTLO] || op_ena[MULTU] || op_ena[DIV] || op_ena[DIVU]) ? 1'b1 : 1'b0;


assign GET_ena = (op_ena[J] || op_ena[JAL])? 1'b1 : 1'b0;


assign ALU_choice[0] = (op_ena[ADDIU]   || op_ena[ORI]  || op_ena[LUI]  || op_ena[SLTI] ||
                        op_ena[AND]     || op_ena[BNE]  || op_ena[JAL]  || op_ena[LW]   || 
                        op_ena[NOR]     || op_ena[SLL]  || op_ena[SLTU] || op_ena[SRL]  || 
                        op_ena[SW]      || op_ena[SUB]  || op_ena[SRLV] || op_ena[CLZ]  || 
                        op_ena[ERET]    || op_ena[LB]   || op_ena[LHU]  || op_ena[SH]   || 
                        op_ena[MFC0]    || op_ena[MFLO] || op_ena[MTHI] || op_ena[MUL]  ||
                        op_ena[SYSCALL] || op_ena[BGEZ] || op_ena[DIV])? 1'b1 : 1'b0;

assign ALU_choice[1] = (op_ena[ANDI]    || op_ena[ORI]  || op_ena[XORI] || op_ena[SLTI] ||
                        op_ena[BEQ]     || op_ena[BNE]  || op_ena[JR]   || op_ena[LW]   || 
                        op_ena[OR]      || op_ena[SLL]  || op_ena[SRA]  || op_ena[SRL]  || 
                        op_ena[ADD]     || op_ena[SUB]  || op_ena[SRAV] || op_ena[CLZ]  || 
                        op_ena[JALR]    || op_ena[LB]   || op_ena[SB]   || op_ena[SH]   || 
                        op_ena[MFHI]    || op_ena[MFLO] || op_ena[MTLO] || op_ena[MUL]  ||
                        op_ena[TEQ]     || op_ena[BGEZ])? 1'b1 : 1'b0;

assign ALU_choice[2] = (op_ena[SLTIU]   || op_ena[LUI]  || op_ena[XORI] || op_ena[SLTI] ||
                        op_ena[J]       || op_ena[JAL]  || op_ena[JR]   || op_ena[LW]   || 
                        op_ena[SLLV]    || op_ena[SLTU] || op_ena[SRA]  || op_ena[SRL]  || 
                        op_ena[SLT]     || op_ena[SRLV] || op_ena[SRAV] || op_ena[CLZ]  || 
                        op_ena[LBU]     || op_ena[LHU]  || op_ena[SB]   || op_ena[SH]   || 
                        op_ena[MTC0]    || op_ena[MTHI] || op_ena[MTLO] || op_ena[MUL]  ||
                        op_ena[BREAK]   || op_ena[DIV])? 1'b1 : 1'b0;

assign ALU_choice[3] = (op_ena[ADDU]    || op_ena[AND]  || op_ena[BEQ]  || op_ena[BNE]  ||
                        op_ena[J]       || op_ena[JAL]  || op_ena[JR]   || op_ena[LW]   || 
                        op_ena[SUBU]    || op_ena[SW]   || op_ena[ADD]  || op_ena[SUB]  || 
                        op_ena[SLT]     || op_ena[SRLV] || op_ena[SRAV] || op_ena[CLZ]  || 
                        op_ena[LH]      || op_ena[MFC0] || op_ena[MFHI] || op_ena[MFLO] || 
                        op_ena[MTC0]    || op_ena[MTHI] || op_ena[MTLO] || op_ena[MUL])? 1'b1 : 1'b0;

assign ALU_choice[4] = (op_ena[XOR]     || op_ena[NOR]  || op_ena[OR]   || op_ena[SLL]  ||
                        op_ena[SLLV]    || op_ena[SLTU] || op_ena[SRA]  || op_ena[SRL]  || 
                        op_ena[SUBU]    || op_ena[SW]   || op_ena[ADD]  || op_ena[SUB]  || 
                        op_ena[SLT]     || op_ena[SRLV] || op_ena[SRAV] || op_ena[CLZ]  || 
                        op_ena[MULTU] || op_ena[SYSCALL] || op_ena[TEQ] || op_ena[BGEZ] || 
                        op_ena[BREAK]   || op_ena[DIV])? 1'b1 : 1'b0;

assign ALU_choice[5] = (op_ena[DIVU]    || op_ena[ERET] || op_ena[JALR] || op_ena[LB]   ||
                        op_ena[LBU]     || op_ena[LHU]  || op_ena[SB]   || op_ena[SH]   || 
                        op_ena[LH]      || op_ena[MFC0] || op_ena[MFHI] || op_ena[MFLO] || 
                        op_ena[MTC0]    || op_ena[MTHI] || op_ena[MTLO] || op_ena[MUL]  || 
                        op_ena[MULTU] || op_ena[SYSCALL] || op_ena[TEQ] || op_ena[BGEZ] || 
                        op_ena[BREAK]   || op_ena[DIV])? 1'b1 : 1'b0;


assign CP0_choice = op_ena[SYSCALL]? 5'b01000 : (op_ena[BREAK]? 5'b01001 : (op_ena[TEQ]? 5'b01101 : 5'bz));


assign EXT1_ena = (op_ena[SLT]  || op_ena[SLTU]  || op_ena[SLTI] || op_ena[SLTIU])? 1'b1 : 1'b0;

assign EXT5_ena = (op_ena[SLL]  || op_ena[SRL]   || op_ena[SRA])? 1'b1 : 1'b0;                   

assign EXT16_ena = (op_ena[ANDI] || op_ena[ORI]   || op_ena[XORI] || op_ena[LUI])? 1'b1 : 1'b0;   

assign EXT16_sign_ena = (op_ena[ADDI] || op_ena[ADDIU] || op_ena[LW]   || op_ena[SW] ||
                         op_ena[SLTI] || op_ena[SLTIU] || op_ena[SB]   || op_ena[SH] ||
                         op_ena[LB]   || op_ena[LH]    || op_ena[LBU]  || op_ena[LHU])? 1'b1 : 1'b0;   

assign EXT18_sign_ena = (op_ena[BEQ]  || op_ena[BNE]   || op_ena[BGEZ])? 1'b1 : 1'b0;                 


assign sign_ena = (op_ena[MUL] || op_ena[DIV])? 1'b1 : 1'b0;

                     
assign MUX_Rd_choice = (op_ena[LW] || op_ena[LB] || op_ena[LH] || op_ena[LBU] || op_ena[LHU])? 3'b000 :
                       ((op_ena[MFHI] || op_ena[MFLO])? 3'b001 : (op_ena[CLZ]? 3'b010 : (op_ena[MUL]? 3'b100 :
                       ((op_ena[JALR] || op_ena[JAL])? 3'b101 :(op_ena[MFC0]? 3'b110 : 3'b011)))));

assign MUX_pc_choice = (op_ena[JR] || op_ena[JALR])? 3'b000 : ((op_ena[BEQ] && Z) || (op_ena[BNE] && !Z) || (op_ena[BGEZ] && !N)? 3'b010 : 
                       (op_ena[BREAK] || op_ena[SYSCALL] || (op_ena[TEQ] && Z)? 3'b011 : (op_ena[ERET]? 3'b100 :
                       (op_ena[J] || op_ena[JAL]? 3'b101 : 3'b001))));

                   

endmodule
