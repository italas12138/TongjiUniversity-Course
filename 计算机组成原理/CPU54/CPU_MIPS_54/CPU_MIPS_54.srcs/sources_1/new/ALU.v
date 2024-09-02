`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 22:59:46
// Design Name: 
// Module Name: ALU
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


module ALU(
    input [31:0] ALU_A,
    input [31:0] ALU_B,
    input [5:0] ALU_choice,

    output [31:0] ALU_result,
    output Z,
    output C,
    output N,
    output O

    );

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


reg [32:0] result;
wire signed [31:0] signA, signB;
assign signA = ALU_A;
assign signB = ALU_B;

always @(*)
begin
    case (ALU_choice)
        ADDI:
            begin
                result <= signA + signB;
            end
        ADDIU:
            begin
                result <= ALU_A + ALU_B;
            end
        ANDI:
            begin
                result <= ALU_A & ALU_B;
            end
        ORI:
            begin
                result <= ALU_A | ALU_B;
            end
        SLTIU:
            begin
                result <= ALU_A - ALU_B;
            end
        LUI:
            begin
                result <= {ALU_B[15:0], 16'b0};
            end
        XORI:
            begin
                result <= ALU_A ^ ALU_B;
            end
        SLTI:
            begin
                result <= signA - signB;
            end
        ADDU:
            begin
                result <= ALU_A + ALU_B;
            end
        AND:
            begin
                result <= ALU_A & ALU_B;
            end
        BEQ:
            begin
                result <= ALU_A - ALU_B;
            end
        BNE:
            begin
                result <= ALU_A - ALU_B;
            end
        J:
            begin
                result <= ALU_A + ALU_B;
            end
        JAL:
            begin
                result <= ALU_A + ALU_B;
            end
        JR:
            begin
                result <= ALU_A + ALU_B;
            end
        LW:
            begin
                result <= ALU_A + ALU_B;
            end
        XOR:
            begin
                result <= ALU_A ^ ALU_B;
            end
        NOR:
            begin
                result <= ~(ALU_A | ALU_B);
            end
        OR:
            begin
                result <= ALU_A | ALU_B;
            end
        SLL:
            begin
                result <= ALU_B << ALU_A;
            end
        SLLV:
            begin
                result <= ALU_B << ALU_A;
            end
        SLTU:
            begin
                result <= ALU_A - ALU_B;
            end
        SRA:
            begin
                result <= signB >>> signA;
            end
        SRL:
            begin
                result <= ALU_B >> ALU_A;
            end
        SUBU:
            begin
                result <= ALU_A - ALU_B;
            end
        SW:
            begin
                result <= ALU_A + ALU_B;
            end
        ADD:
            begin
                result <= signA + signB;
            end
        SUB:
            begin
                result <= signA - signB;
            end
        SLT:
            begin
                result <= signA - signB;
            end
        SRLV:
            begin
                result <= ALU_B >> ALU_A;
            end
        SRAV:
            begin
                result <= signB >>> signA;
            end
        CLZ:
            begin
                result <= ALU_A + ALU_B;
            end
        DIVU:
            begin
                result <= ALU_A + ALU_B;
            end
        ERET:
            begin
                result <= ALU_A + ALU_B;
            end
        JALR:
            begin
                result <= ALU_A + ALU_B;
            end
        LB:
            begin
                result <= signA + signB;
            end
        LBU:
            begin
                result <= signA + signB;
            end
        LHU:
            begin
                result <= signA + signB;
            end
        SB:
            begin
                result <= signA + signB;
            end
        SH:
            begin
                result <= signA + signB;
            end
        LH:
            begin
                result <= signA + signB;
            end
        MFC0:
            begin
                result <= ALU_A - ALU_B;
            end
        MFHI:
            begin
                result <= ALU_A + ALU_B;
            end
        MFLO:
            begin
                result <= ALU_A + ALU_B;
            end
        MTC0:
            begin
                result <= ALU_A + ALU_B;
            end
        MTHI:
            begin
                result <= ALU_A + ALU_B;
            end
        MTLO:
            begin
                result <= ALU_A + ALU_B;
            end
        MUL:
            begin
                result <= ALU_A + ALU_B;
            end
        MULTU:
            begin
                result <= ALU_A + ALU_B;
            end
        SYSCALL:
            begin
                result <= ALU_A + ALU_B;
            end
        TEQ:
            begin
                result <= signA + signB;
            end
        BGEZ:
            begin
                result <= signA - signB;
            end
        BREAK:
            begin
                result <= ALU_A + ALU_B;
            end
        DIV:
            begin
                result <= ALU_A + ALU_B;
            end
        default: ;
    endcase
end


assign ALU_result = result[31:0];
assign Z = (result == 32'b0)? 1 : 0;
assign C = result[32];
assign N = result[32];
assign O = result[32];

endmodule
