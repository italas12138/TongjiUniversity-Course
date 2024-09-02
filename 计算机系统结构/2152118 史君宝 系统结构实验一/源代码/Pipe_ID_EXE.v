`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    ID_EXE Á÷Ë®Ïß¼Ä´æÆ÷
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module Pipe_ID_EXE(
    input clk,
    input rst,

    input DM_w_ID,
    input write_ID,
    input mux_alua_ID,
    input [1:0] mux_alub_ID,
    input [1:0] mux_wdata_ID,
    input [3:0] aluc_ID,
    input [31:0] npc_ID,
    input [4:0] waddr_ID,
    input [31:0] sa32_ID,
    input [31:0] simm32_ID,
    input [31:0] uimm32_ID,
    input [31:0] rs_data_ID,
    input [31:0] rt_data_ID,
    input [31:0] DM_wdata_ID,

    output reg DM_w_EXE = 1'b0,
    output reg write_EXE = 1'b0,
    output reg mux_alua_EXE = 1'b0,
    output reg [1:0] mux_alub_EXE = 2'b0,
    output reg [1:0] mux_wdata_EXE = 2'b0,
    output reg [3:0] aluc_EXE = 4'b0,
    output reg [31:0] npc_EXE = 32'b0,
    output reg [4:0] waddr_EXE = 5'b0,
    output reg [31:0] sa32_EXE = 32'b0,
    output reg [31:0] simm32_EXE = 32'b0,
    output reg [31:0] uimm32_EXE = 32'b0,
    output reg [31:0] rs_data_EXE = 32'b0,
    output reg [31:0] rt_data_EXE = 32'b0,
    output reg [31:0] DM_wdata_EXE = 32'b0
    );

    always @(posedge rst or posedge clk) begin
        if(rst) begin
            rs_data_EXE <= 32'b0;
            sa32_EXE <= 32'b0;
            simm32_EXE <= 32'b0;
            uimm32_EXE <= 32'b0;
            rt_data_EXE <= 32'b0;
            mux_alua_EXE <= 1'b0;
            mux_alub_EXE <= 2'b0;
            aluc_EXE <= 4'b0;
            write_EXE <= 'b0;
            waddr_EXE <= 5'b0;
            mux_wdata_EXE <= 1'b0;
            DM_wdata_EXE <= 32'b0;
            DM_w_EXE <= 1'b0;
            npc_EXE <= 32'b0;
        end 
        else begin
            rs_data_EXE <= rs_data_ID;
            sa32_EXE <= sa32_ID;
            simm32_EXE <= simm32_ID;
            uimm32_EXE <= uimm32_ID;
            rt_data_EXE <= rt_data_ID;
            mux_alua_EXE <= mux_alua_ID;
            mux_alub_EXE <= mux_alub_ID;
            aluc_EXE <= aluc_ID;
            write_EXE <= write_ID;
            waddr_EXE <= waddr_ID;
            mux_wdata_EXE <= mux_wdata_ID;
            DM_wdata_EXE <= DM_wdata_ID;
            DM_w_EXE <= DM_w_ID;
            npc_EXE <= npc_ID;
        end
    end
endmodule