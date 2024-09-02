`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 22:59:07
// Design Name: 
// Module Name: cpu
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


module cpu(
    input CPU_clk,
    input CPU_ena,
    input CPU_rst,
    input [31:0] inst_in,
    input [31:0] DMEM_data_out,
    
    output DMEM_ena,
    output DMEM_r,
    output DMEM_w,

    output [31:0] pc_out,
    output [31:0] DMEM_addr_temp,
    output [31:0] DMEM_data_in,

    output sb_ena,
    output sh_ena,
    output sw_ena,
    output lb_ena,
    output lh_ena,
    output lbu_ena,
    output lhu_ena,
    output lw_ena

    );

//ALU閮ㄥ垎
wire [31:0] ALU_A, ALU_B;             //ALU鐨勪袱涓緭鍏ユ暟锟??????
wire [5:0] ALU_choice;                //閫夋嫨鐨凙LU璁＄畻鏂瑰紡
wire [31:0] ALU_result;                  //ALU鐨勮绠楃粨锟??????
wire Z;                               //ALU鐨勯浂淇″彿鏍囧織
wire C;                               //ALU鐨勮繘浣嶄俊鍙锋爣锟??????
wire N;                               //ALU鐨勮礋鏁颁俊鍙锋爣锟??????
wire O;                               //ALU鐨勬孩鍑轰俊鍙锋爣锟??????


//PC锟斤拷锟斤拷
wire [31:0] pc_addr_in;
wire [31:0] pc_addr_out;


//regfile閮ㄥ垎
wire ref_r;
wire ref_w;
wire [4:0] RsC;
wire [4:0] RtC;
wire [4:0] RdC;

wire [31:0] Rs_data_out;
wire [31:0] Rt_data_out;
wire [31:0] Rd_data_in; 


//解析指令部分
wire [53:0] op_ena;
wire [4:0] shamt;
wire [15:0] immediate;
wire [25:0] address;


//CP0 部分
wire [31:0] CP0_addr;
wire [31:0] CP0_data_in;
wire [31:0] CP0_data_out;

wire [4:0] CP0_choice;
wire [31:0] CP0_pc_out;



//乘法器部分
wire sign_ena;

wire [31:0] MUL_A;
wire [31:0] MUL_B;

wire [31:0] HI;
wire [31:0] LO;


//除法器部分
wire [31:0] DIV_A;
wire [31:0] DIV_B;

wire [31:0] R;
wire [31:0] Q;


//HI_LO寄存器部分
wire HI_w;
wire LO_w;

wire [31:0] HI_data_in;
wire [31:0] LO_data_in;
wire [31:0] HI_data_out;
wire [31:0] LO_data_out;


//CLZ前导计零部分
wire [31:0] CLZ_data_in;
wire [31:0] CLZ_cnt_out;

assign CLZ_data_in = Rs_data_out;



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


//数据存储器部分
assign DMEM_ena = (DMEM_r || DMEM_w)? 1'b1 : 1'b0;
assign DMEM_addr_temp = ALU_result;
assign DMEM_data_in = Rt_data_out;
assign sb_ena   = op_ena[SB];
assign sh_ena   = op_ena[SH];
assign sw_ena   = op_ena[SW];
assign lb_ena   = op_ena[LB];
assign lh_ena   = op_ena[LH];
assign lbu_ena  = op_ena[LBU];
assign lhu_ena  = op_ena[LHU];
assign lw_ena   = op_ena[LW];


//数据拓展器部分
wire EXT1_ena;
wire EXT5_ena;
wire EXT16_ena;
wire EXT16_sign_ena;
wire EXT18_sign_ena;

wire [31:0] EXT1_out;
wire [31:0] EXT5_out;
wire [31:0] EXT16_out;
wire [31:0] EXT16_sign_out;
wire [31:0] EXT18_sign_out;


assign EXT1_out = (op_ena[SLT] || op_ena[SLTU])? N : (op_ena[SLTI] || op_ena[SLTIU])? C : 32'hz;

assign EXT5_out = (op_ena[SLL] || op_ena[SRL] || op_ena[SRA])? {27'h0, shamt} : 32'hz;

assign EXT16_out = (op_ena[ANDI] || op_ena[ORI] || op_ena[XORI] || op_ena[LUI])? {16'h0, immediate} : 32'h0;

assign EXT16_sign_out = (op_ena[ADDI] || op_ena[ADDIU] || op_ena[LW] || op_ena[SW] || 
                         op_ena[SLTI] || op_ena[SLTIU] || op_ena[SB] || op_ena[SH] ||
                         op_ena[LB] || op_ena[LH] || op_ena[LBU] || op_ena[LHU])? {{16{immediate[15]}}, immediate} : 32'hz;

assign EXT18_sign_out = (op_ena[BEQ] || op_ena[BNE] || op_ena[BGEZ])? {{14{immediate[15]}}, immediate, 2'b0} : 32'hz;



//拼接器部分
wire [31:0] GET_out;
wire GET_ena;

assign GET_out = GET_ena? {pc_addr_out[31:28], address, 2'b0} : 32'hz;



//ALU部分
wire [31:0] MUX_A_out;
wire MUX_A_ena;
wire [31:0] MUX_B_out;
wire MUX_B_ena;

assign MUX_A_ena = (op_ena[SLL] || op_ena[SRL] || op_ena[SRA])? 1'b1 : 1'b0;
assign MUX_B_ena = (op_ena[ADDI] || op_ena[ADDIU] || op_ena[ANDI] || op_ena[ORI] ||
                    op_ena[XORI] || op_ena[LW] || op_ena[SW] || op_ena[SLTI] ||
                    op_ena[SLTIU] || op_ena[LUI] || op_ena[SB] || op_ena[SH] ||
                    op_ena[LB] || op_ena[LH] || op_ena[LBU] || op_ena[LHU])? 1'b1 : 1'b0; 


assign MUX_A_out = MUX_A_ena? EXT5_out : Rs_data_out;
assign ALU_A = MUX_A_out;

assign MUX_B_out = MUX_B_ena? ((op_ena[ANDI] || op_ena[ORI] || op_ena[XORI] || op_ena[LUI])? EXT16_out : EXT16_sign_out) : Rt_data_out;
assign ALU_B = op_ena[BGEZ]? 32'd0 : MUX_B_out;


//CP0 部分
wire [31:0] MUX_CP0_out;

assign CP0_addr = MUX_CP0_out;

assign MUX_CP0_out = (op_ena[MFC0])? Rt_data_out : Rs_data_out;


//加法器
wire [31:0] ADD_1;
wire [31:0] ADD_2;


//PC 部分
wire [31:0] NPC;
wire [31:0] MUX_pc_out;
wire [2:0] MUX_pc_choice;

assign NPC = pc_addr_out + 4;

assign pc_out = pc_addr_out;

assign pc_addr_in = MUX_pc_out;

assign MUX_pc_out = ((MUX_pc_choice == 3'b000)? Rs_data_out :
                    ((MUX_pc_choice == 3'b001)? NPC :
                    ((MUX_pc_choice == 3'b010)? ADD_1 :
                    ((MUX_pc_choice == 3'b011)? 32'h4 :
                    ((MUX_pc_choice == 3'b100)? CP0_pc_out :
                    ((MUX_pc_choice == 3'b101)? GET_out :
                    ((MUX_pc_choice == 3'b110)? 32'hz : 32'hz)))))));
                    

//HI_LO 部分
assign HI_data_in = (!op_ena[MTHI])? ((!op_ena[MULTU])? R : HI) : Rs_data_out;
assign LO_data_in = (!op_ena[MTLO])? ((!op_ena[MULTU])? Q : LO) : Rs_data_out;                  



assign ADD_1 = EXT18_sign_out + NPC;
assign ADD_2 = pc_addr_out + 4;

//乘法器部分和除法器部分
assign MUL_A = (op_ena[MUL] || op_ena[MULTU])? Rs_data_out : 32'hz;
assign MUL_B = (op_ena[MUL] || op_ena[MULTU])? Rt_data_out : 32'hz;
assign DIV_A = (op_ena[DIV] || op_ena[DIVU])? Rs_data_out : 32'hz;
assign DIV_B = (op_ena[DIV] || op_ena[DIVU])? Rt_data_out : 32'hz;


//寄存器写的部分
wire [2:0] MUX_Rd_choice;
wire [31:0] MUX_Rd_out;


assign MUX_Rd_out = ((MUX_Rd_choice == 3'b000)? DMEM_data_out :
                    ((MUX_Rd_choice == 3'b001)? ((op_ena[MFHI])? HI_data_out : LO_data_out):
                    ((MUX_Rd_choice == 3'b010)? CLZ_cnt_out:
                    ((MUX_Rd_choice == 3'b011)? ((op_ena[SLT] || op_ena[SLTU] || op_ena[SLTI] || op_ena[SLTIU])? EXT1_out : ALU_result):
                    ((MUX_Rd_choice == 3'b100)? LO:                                        
                    ((MUX_Rd_choice == 3'b101)? ADD_2:
                    ((MUX_Rd_choice == 3'b110)? CP0_data_out : 32'bz)))))));

assign Rd_data_in = MUX_Rd_out;





ALU alu(
    .ALU_A(ALU_A),
    .ALU_B(ALU_B),
    .ALU_choice(ALU_choice),

    .ALU_result(ALU_result),
    .Z(Z),
    .C(C),
    .N(N),
    .O(O)
);

PC pc(
    .pc_clk(CPU_clk),
    .pc_ena(CPU_ena),
    .pc_rst(CPU_rst),
    .pc_addr_in(pc_addr_in),
    
    .pc_addr_out(pc_addr_out)
);

regfile cpu_ref(
    .ref_clk(CPU_clk),
    .ref_ena(CPU_ena),
    .ref_rst(CPU_rst),

    .ref_r(ref_r),
    .ref_w(ref_w),

    .RsC(RsC),
    .RtC(RtC),
    .RdC(RdC),

    .Rs_data_out(Rs_data_out),
    .Rt_data_out(Rt_data_out),
    .Rd_data_in(Rd_data_in)
);


Inst_Get inst_get(
    .inst_in(inst_in),
    
    .RsC(RsC),
    .RtC(RtC),
    .RdC(RdC),
    
    .shamt(shamt),
    .immediate(immediate),
    .address(address),
    
    .op_ena(op_ena)
);



CP0 cp0(
    .CP0_clk(CPU_clk),
    .CP0_ena(CPU_ena),
    .CP0_rst(CPU_rst),

    .mfc0(op_ena[MFC0]),
    .mtc0(op_ena[MTC0]),
    .eret(op_ena[ERET]),

    .CP0_addr(CP0_addr),
    .pc(pc_addr_out),

    .CP0_data_in(Rt_data_out),
    .CP0_data_out(CP0_data_out),

    .CP0_choice(CP0_choice),

    .CP0_pc_out(CP0_pc_out)
);


MUL mul(
    .sign_ena(sign_ena),
    
    .MUL_A(MUL_A),
    .MUL_B(MUL_B),
    
    .HI(HI),
    .LO(LO)
);


DIV div(
    .sign_ena(sign_ena),
    
    .DIV_A(DIV_A),
    .DIV_B(DIV_B),
    
    .R(R),
    .Q(Q)
);


HI_LO hi_lo(
    .HI_LO_clk(CPU_clk),
    .HI_LO_ena(CPU_ena),
    .HI_LO_rst(CPU_rst),
    
    .HI_data_in(HI_data_in),
    .LO_data_in(LO_data_in),
    
    .HI_w(HI_w),
    .LO_w(LO_w),
    
    .HI_data_out(HI_data_out),
    .LO_data_out(LO_data_out)
);


CLZ clz(
    .CLZ_data_in(CLZ_data_in),
    
    .CLZ_cnt_out(CLZ_cnt_out)
);


Control control(
    .op_ena(op_ena),
    .Z(Z),
    .N(N),
    
    .ref_w(ref_w),
    .ALU_choice(ALU_choice),
    .DMEM_r(DMEM_r),
    .DMEM_w(DMEM_w),
    .HI_w(HI_w),
    .LO_w(LO_w),
    
    .sign_ena(sign_ena),
    
    .GET_ena(GET_ena),
    .CP0_choice(CP0_choice),
    
    .EXT1_ena(EXT1_ena),
    .EXT5_ena(EXT5_ena),
    .EXT16_ena(EXT16_ena),
    .EXT16_sign_ena(EXT16_sign_ena),
    .EXT18_sign_ena(EXT18_sign_ena),
    
    .MUX_Rd_choice(MUX_Rd_choice),
    .MUX_pc_choice(MUX_pc_choice)
);

endmodule
