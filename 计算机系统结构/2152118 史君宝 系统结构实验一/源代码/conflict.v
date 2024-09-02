`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    ³åÍ»ÅÐ¶ÏÄ£¿é
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module conflict(
    input jump,
    input [31:0] inst,
    input write_ID,
    input write_EXE,
    input write_MEM,
    input [4:0] waddr_ID,
    input [4:0] waddr_EXE,
    input [4:0] waddr_MEM,
    output stall
    );

	wire [4:0] rs;
	wire [4:0] rt;
    
    assign rs = inst[25:21];
	assign rt = inst[20:16];

    assign stall =  (~jump)&&(((write_ID && waddr_ID != 5'b0)&&((rs==waddr_ID)||(rt==waddr_ID)))||
                    ((write_EXE && waddr_EXE != 5'b0)&&((rs==waddr_EXE)||(rt==waddr_EXE)))||
                    ((write_MEM && waddr_MEM != 5'b0)&&((rs==waddr_MEM)||(rt==waddr_MEM))));
endmodule