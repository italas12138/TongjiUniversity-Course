`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    IF_ID Á÷Ë®Ïß¼Ä´æÆ÷
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module Pipe_IF_ID(
    input clk,
    input rst,
    input [31:0] npc_IF,
    input [31:0] inst_IF,
    output reg [31:0] npc_ID = 32'b0,
    output reg [31:0] inst_ID = 32'b0
    );

    always @(posedge clk or posedge rst) begin
        if(rst) begin
            inst_ID <= 32'b0;   npc_ID <= 32'b0;
        end 
        else begin
            inst_ID <= inst_IF; npc_ID <= npc_IF;
        end
    end
endmodule