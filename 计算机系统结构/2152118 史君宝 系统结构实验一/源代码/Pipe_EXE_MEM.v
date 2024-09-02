`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    EXE_MEM Á÷Ë®Ïß¼Ä´æÆ÷
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module Pipe_EXE_MEM(
    input clk,
    input rst,

    input DM_w_EXE,
    input write_EXE,
    input [4:0] waddr_EXE,
    input [1:0] mux_wdata_EXE,
    input [31:0] alu_EXE,
    input [31:0] npc_EXE,
    input [31:0] DM_wdata_EXE,

    output reg DM_w_MEM = 1'b0,
    output reg write_MEM = 1'b0,
    output reg [4:0] waddr_MEM = 5'b0,
    output reg [1:0] mux_wdata_MEM = 2'b0,
    output reg [31:0] alu_MEM = 32'b0,
    output reg [31:0] npc_MEM = 32'b0,
    output reg [31:0] DM_wdata_MEM = 32'b0
    );

    always @(posedge rst or posedge clk) begin
        if(rst) begin
            write_MEM <= 1'b0;
            alu_MEM <= 32'b0;
            waddr_MEM <= 5'b0;
            mux_wdata_MEM <= 1'b0;
            DM_wdata_MEM <= 32'b0;
            DM_w_MEM <= 1'b0;
            npc_MEM <= 32'b0;
        end 
        else begin
            write_MEM <= write_EXE;
            alu_MEM <= alu_EXE;
            waddr_MEM <= waddr_EXE;
            mux_wdata_MEM <= mux_wdata_EXE;
            DM_wdata_MEM <= DM_wdata_EXE;
            DM_w_MEM <= DM_w_EXE;
            npc_MEM <= npc_EXE;
        end
    end
endmodule