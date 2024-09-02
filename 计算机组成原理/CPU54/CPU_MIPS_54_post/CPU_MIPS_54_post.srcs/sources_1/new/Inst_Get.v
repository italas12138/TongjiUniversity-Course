`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 23:01:55
// Design Name: 
// Module Name: Inst_Get
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


module Inst_Get(
    input [31:0] inst_in,

    output [4:0] RsC,
    output [4:0] RtC,
    output [4:0] RdC,
    output [4:0] shamt,
    output [15:0] immediate,
    output [25:0] address,
    
    output [53:0] op_ena
    );


//指出对应指令中的代码
parameter ADD_ena   = 6'b100000;
parameter ADDU_ena  = 6'b100001;
parameter SUB_ena   = 6'b100010;
parameter SUBU_ena  = 6'b100011;
parameter AND_ena   = 6'b100100;
parameter OR_ena    = 6'b100101;
parameter XOR_ena   = 6'b100110;
parameter NOR_ena   = 6'b100111;
parameter SLT_ena   = 6'b101010;
parameter SLTU_ena  = 6'b101011;
parameter SLL_ena   = 6'b000000;
parameter SRL_ena   = 6'b000010;
parameter SRA_ena   = 6'b000011;
parameter SLLV_ena  = 6'b000100;
parameter SRLV_ena  = 6'b000110;
parameter SRAV_ena  = 6'b000111;
parameter JR_ena    = 6'b001000;
parameter ADDI_ena  = 6'b001000;
parameter ADDIU_ena = 6'b001001;
parameter ANDI_ena  = 6'b001100;
parameter ORI_ena   = 6'b001101;
parameter XORI_ena  = 6'b001110;
parameter LW_ena    = 6'b100011;
parameter SW_ena    = 6'b101011;
parameter BEQ_ena   = 6'b000100;
parameter BNE_ena   = 6'b000101;
parameter SLTI_ena  = 6'b001010;
parameter SLTIU_ena = 6'b001011;
parameter LUI_ena   = 6'b001111;
parameter J_ena     = 6'b000010;
parameter JAL_ena   = 6'b000011;

/* 54条指令添加部分 */
parameter CLZ_ena     = 6'b100000;
parameter JALR_ena    = 6'b001001;
parameter MTHI_ena    = 6'b010001;
parameter MFHI_ena    = 6'b010000;
parameter MTLO_ena    = 6'b010011;
parameter MFLO_ena    = 6'b010010;
parameter SB_ena      = 6'b101000;
parameter SH_ena      = 6'b101001;
parameter LB_ena      = 6'b100000;
parameter LH_ena      = 6'b100001;
parameter LBU_ena     = 6'b100100;
parameter LHU_ena     = 6'b100101;
parameter ERET_ena    = 6'b011000;
parameter BREAK_ena   = 6'b001101;
parameter SYSCALL_ena = 6'b001100;
parameter TEQ_ena     = 6'b110100;
parameter MFC0_ena    = 5'b00000;
parameter MTC0_ena    = 5'b00100; //注意这两个指令是靠另外五位order区分的，op和func都一样
parameter MUL_ena     = 6'b000010;
parameter MULTU_ena   = 6'b011001;
parameter DIV_ena     = 6'b011010;
parameter DIVU_ena    = 6'b011011;
parameter BGEZ_ena    = 6'b000001;



/* 54条指令对应的标志位 */
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

/* 下面是赋值 */
/* 对指令进行译码，判断是哪个指令 */
assign op_ena[ADD]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == ADD_ena )) ? 1'b1 : 1'b0;
assign op_ena[ADDU]  = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == ADDU_ena)) ? 1'b1 : 1'b0;
assign op_ena[SUB]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SUB_ena )) ? 1'b1 : 1'b0;
assign op_ena[SUBU]  = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SUBU_ena)) ? 1'b1 : 1'b0;
assign op_ena[AND]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == AND_ena )) ? 1'b1 : 1'b0;
assign op_ena[OR]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == OR_ena  )) ? 1'b1 : 1'b0;
assign op_ena[XOR]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == XOR_ena )) ? 1'b1 : 1'b0;
assign op_ena[NOR]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == NOR_ena )) ? 1'b1 : 1'b0;
assign op_ena[SLT]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SLT_ena )) ? 1'b1 : 1'b0;
assign op_ena[SLTU]  = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SLTU_ena)) ? 1'b1 : 1'b0;
assign op_ena[SLL]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SLL_ena )) ? 1'b1 : 1'b0;
assign op_ena[SRL]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SRL_ena )) ? 1'b1 : 1'b0;
assign op_ena[SRA]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SRA_ena )) ? 1'b1 : 1'b0;
assign op_ena[SLLV]  = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SLLV_ena)) ? 1'b1 : 1'b0;
assign op_ena[SRLV]  = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SRLV_ena)) ? 1'b1 : 1'b0;
assign op_ena[SRAV]  = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SRAV_ena)) ? 1'b1 : 1'b0;
assign op_ena[JR]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == JR_ena  )) ? 1'b1 : 1'b0;
assign op_ena[ADDI]  = (inst_in[31:26] == ADDI_ena ) ? 1'b1 : 1'b0;
assign op_ena[ADDIU] = (inst_in[31:26] == ADDIU_ena) ? 1'b1 : 1'b0;
assign op_ena[ANDI]  = (inst_in[31:26] == ANDI_ena ) ? 1'b1 : 1'b0;
assign op_ena[ORI]   = (inst_in[31:26] == ORI_ena  ) ? 1'b1 : 1'b0;
assign op_ena[XORI]  = (inst_in[31:26] == XORI_ena ) ? 1'b1 : 1'b0;
assign op_ena[LW]    = (inst_in[31:26] == LW_ena   ) ? 1'b1 : 1'b0;
assign op_ena[SW]    = (inst_in[31:26] == SW_ena   ) ? 1'b1 : 1'b0;
assign op_ena[BEQ]   = (inst_in[31:26] == BEQ_ena  ) ? 1'b1 : 1'b0;
assign op_ena[BNE]   = (inst_in[31:26] == BNE_ena  ) ? 1'b1 : 1'b0;
assign op_ena[SLTI]  = (inst_in[31:26] == SLTI_ena ) ? 1'b1 : 1'b0;
assign op_ena[SLTIU] = (inst_in[31:26] == SLTIU_ena) ? 1'b1 : 1'b0;
assign op_ena[LUI]   = (inst_in[31:26] == LUI_ena  ) ? 1'b1 : 1'b0;
assign op_ena[J]     = (inst_in[31:26] == J_ena    ) ? 1'b1 : 1'b0;
assign op_ena[JAL]   = (inst_in[31:26] == JAL_ena  ) ? 1'b1 : 1'b0;

assign op_ena[CLZ]     = ((inst_in[31:26] == 6'b011100) && (inst_in[5:0] == CLZ_ena )) ? 1'b1 : 1'b0;
assign op_ena[JALR]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == JALR_ena )) ? 1'b1 : 1'b0;
assign op_ena[MTHI]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == MTHI_ena )) ? 1'b1 : 1'b0;
assign op_ena[MTLO]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == MTLO_ena )) ? 1'b1 : 1'b0;
assign op_ena[MFHI]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == MFHI_ena )) ? 1'b1 : 1'b0;
assign op_ena[MFLO]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == MFLO_ena )) ? 1'b1 : 1'b0;
assign op_ena[SB]      = (inst_in[31:26] == SB_ena  ) ? 1'b1 : 1'b0;
assign op_ena[SH]      = (inst_in[31:26] == SH_ena  ) ? 1'b1 : 1'b0;
assign op_ena[LB]      = (inst_in[31:26] == LB_ena  ) ? 1'b1 : 1'b0;
assign op_ena[LH]      = (inst_in[31:26] == LH_ena  ) ? 1'b1 : 1'b0;
assign op_ena[LBU]     = (inst_in[31:26] == LBU_ena ) ? 1'b1 : 1'b0;
assign op_ena[LHU]     = (inst_in[31:26] == LHU_ena ) ? 1'b1 : 1'b0;
assign op_ena[ERET]    = ((inst_in[31:26] == 6'b010000) && (inst_in[5:0] == ERET_ena    )) ? 1'b1 : 1'b0;
assign op_ena[BREAK]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == BREAK_ena   )) ? 1'b1 : 1'b0;
assign op_ena[SYSCALL] = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == SYSCALL_ena )) ? 1'b1 : 1'b0;
assign op_ena[TEQ]     = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == TEQ_ena     )) ? 1'b1 : 1'b0;
assign op_ena[MFC0]    = ((inst_in[31:26] == 6'b010000) && (inst_in[5:0] == 6'h0) && (inst_in[25:21] == MFC0_ena)) ? 1'b1 : 1'b0;
assign op_ena[MTC0]    = ((inst_in[31:26] == 6'b010000) && (inst_in[5:0] == 6'h0) && (inst_in[25:21] == MTC0_ena)) ? 1'b1 : 1'b0;
assign op_ena[MUL]     = ((inst_in[31:26] == 6'b011100) && (inst_in[5:0] == MUL_ena     )) ? 1'b1 : 1'b0;
assign op_ena[MULTU]   = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == MULTU_ena   )) ? 1'b1 : 1'b0;
assign op_ena[DIV]     = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == DIV_ena     )) ? 1'b1 : 1'b0;
assign op_ena[DIVU]    = ((inst_in[31:26] == 6'h0) && (inst_in[5:0] == DIVU_ena    )) ? 1'b1 : 1'b0;
assign op_ena[BGEZ]    = ((inst_in[31:26] == BGEZ_ena) && (inst_in[20:16] == 5'b00001) ) ? 1'b1 : 1'b0;


/* 取出指令中各部分的值 */
assign RsC = (op_ena[ADD]  || op_ena[ADDU] || op_ena[SUB]  || op_ena[SUBU]  ||
              op_ena[AND]  || op_ena[OR]   || op_ena[XOR]  || op_ena[NOR]   ||
              op_ena[SLT]  || op_ena[SLTU] || op_ena[SLLV] || op_ena[SRLV]  ||
              op_ena[SRAV] || op_ena[JR]   || op_ena[ADDI] || op_ena[ADDIU] ||
              op_ena[ANDI] || op_ena[ORI]  || op_ena[XORI] || op_ena[LW]    ||
              op_ena[SW]   || op_ena[BEQ]  || op_ena[BNE]  || op_ena[SLTI]  ||
              op_ena[SLTIU]|| op_ena[CLZ]  || op_ena[JALR] || op_ena[MTHI]  ||
              op_ena[MTLO] || op_ena[SB]   || op_ena[SH]   || op_ena[LB]    ||
              op_ena[LH]   || op_ena[LBU]  || op_ena[LHU]  || op_ena[TEQ]   ||
              op_ena[MUL]  || op_ena[MULTU]|| op_ena[DIV]  || op_ena[DIVU]  || 
              op_ena[BGEZ]) ? inst_in[25:21] : 
              (op_ena[MTC0] ? inst_in[15:11] : 5'hz);

assign RtC = (op_ena[ADD]  || op_ena[ADDU]  || op_ena[SUB]   || op_ena[SUBU] ||
              op_ena[AND]  || op_ena[OR]    || op_ena[XOR]   || op_ena[NOR]  ||
              op_ena[SLT]  || op_ena[SLTU]  || op_ena[SLL]   || op_ena[SRL]  ||
              op_ena[SRA]  || op_ena[SLLV]  || op_ena[SRLV]  || op_ena[SRAV] ||
              op_ena[SW]   || op_ena[BEQ]   || op_ena[BNE]   || op_ena[SB]   ||
              op_ena[SH]   || op_ena[TEQ]   || op_ena[MTC0]  || op_ena[MUL]  ||
              op_ena[MULTU]|| op_ena[DIV]   || op_ena[DIVU]) ? inst_in[20:16] : 
              (op_ena[MFC0] ? inst_in[15:11] : 5'hz);

assign RdC = (op_ena[ADD]  || op_ena[ADDU]  || op_ena[SUB]  || op_ena[SUBU]  ||
              op_ena[AND]  || op_ena[OR]    || op_ena[XOR]  || op_ena[NOR]   ||
              op_ena[SLT]  || op_ena[SLTU]  || op_ena[SLL]  || op_ena[SRL]   ||
              op_ena[SRA]  || op_ena[SLLV]  || op_ena[SRLV] || op_ena[SRAV]  ||
              op_ena[CLZ]  || op_ena[JALR]  || op_ena[MFHI] || op_ena[MFLO]  ||
              op_ena[MUL]) ? inst_in[15:11] : ((
              op_ena[ADDI] || op_ena[ADDIU] || op_ena[ANDI] || op_ena[ORI]   || 
              op_ena[XORI] || op_ena[LW]    || op_ena[SLTI] || op_ena[SLTIU] ||
              op_ena[LUI]  || op_ena[MFC0]  || op_ena[LB]   || op_ena[LH]    || 
              op_ena[LBU]  || op_ena[LHU]   ) ? inst_in[20:16] : (op_ena[JAL] ? 5'd31 : 5'hz));

assign shamt = (op_ena[SLL] || op_ena[SRL] || op_ena[SRA]) ? inst_in[10:6] : 5'hz;        

assign immediate = (op_ena[ADDI] || op_ena[ADDIU] || op_ena[ANDI]  || op_ena[ORI] || 
                    op_ena[XORI] || op_ena[LW]    || op_ena[SW]    || op_ena[BEQ] || 
                    op_ena[BNE]  || op_ena[SLTI]  || op_ena[SLTIU] || op_ena[LUI] ||
                    op_ena[SB]   || op_ena[SH]    || op_ena[LB]    || op_ena[LH]  ||
                    op_ena[LBU]  || op_ena[LHU]   || op_ena[BGEZ]) ? inst_in[15:0] : 16'hz;

assign address = (op_ena[J] || op_ena[JAL]) ? inst_in[25:0] : 26'hz;


endmodule
