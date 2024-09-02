`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 15:07:09
// Design Name: 
// Module Name: DMEM
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


module DMEM(
    input DMEM_clk,
    input DMEM_ena,
    
    input DMEM_r,
    input DMEM_w,

    input [31:0] DMEM_data_in,
    input [6:0] DMEM_addr,

    output [31:0] DMEM_data_out,

    input sb_ena,
    input sh_ena,
    input sw_ena,
    input lb_ena,
    input lh_ena,
    input lbu_ena,
    input lhu_ena,
    input lw_ena
    );


reg [31:0] DMEM_reg [31:0];


assign DMEM_data_out = (DMEM_ena && DMEM_r && !DMEM_w)? 
                     (lb_ena ? { {24{DMEM_reg[DMEM_addr][7]}} , DMEM_reg[DMEM_addr][7:0] } : 
                     (lbu_ena ? { 24'h0 , DMEM_reg[DMEM_addr][7:0] } :
                     (lh_ena ? { {16{DMEM_reg[DMEM_addr >> 1][15]}} , DMEM_reg[DMEM_addr >> 1][15:0] } :
                     (lhu_ena ? { 16'h0 , DMEM_reg[DMEM_addr >> 1][15:0] } :
                     (lw_ena ? DMEM_reg[DMEM_addr >> 2]: 32'bz))))) : 32'bz;//必须是使能端开启、读指令有效且写指令无效时，才将对应地址的数据送出，否则置为高阻抗

always @(negedge DMEM_clk)//时钟上升沿写入数据
begin
    if(DMEM_ena && DMEM_w && !DMEM_r)//必须是使能端开启、写指令有效且读指令无效时，才向寄存器中写入数据
    begin
        if(sb_ena)         //如果是SB指令，不需要对地址再做处理
            DMEM_reg[DMEM_addr][7:0] <= DMEM_data_in[7:0];
        else if(sh_ena)    //如果是SH指令，需要对地址除以二
            DMEM_reg[DMEM_addr >> 1][15:0] <= DMEM_data_in[15:0];
        else if(sw_ena)    //剩下的就是SW指令，需要对地址除以四
            DMEM_reg[DMEM_addr >> 2] <= DMEM_data_in;
    end
end

endmodule
