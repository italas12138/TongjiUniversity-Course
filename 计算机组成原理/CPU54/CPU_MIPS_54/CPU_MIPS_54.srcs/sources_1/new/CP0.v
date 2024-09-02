`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/11 21:21:26
// Design Name: 
// Module Name: CP0
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


module CP0(
    input CP0_clk,
    input CP0_ena,
    input CP0_rst,
    
    input mfc0,
    input mtc0,
    input eret,
    
    input [31:0] CP0_addr,

    input [31:0] pc,

    input [31:0] CP0_data_in,
    output [31:0] CP0_data_out,
    
    input [4:0] CP0_choice,

    output [31:0] CP0_pc_out

    );

parameter SYSCALL = 5'b01000;   //SYSCALL
parameter BREAK   = 5'b01001;   //BREAK
parameter TEQ     = 5'b01101;   //TEQ


parameter STATUS_REG_NUM = 4'b1100;
parameter CAUSE_REG_NUM = 4'b1101;
parameter EPC_REG_NUM = 4'b1110;

reg [31:0] CP0_reg [31:0];


assign CP0_pc_out = CP0_ena && eret? CP0_reg[EPC_REG_NUM] : 32'hz;
assign CP0_data_out = CP0_ena && mfc0? CP0_reg[CP0_addr[4:0]] : 32'hz;



always @(negedge CP0_clk or posedge CP0_rst)
begin
    if(CP0_ena && CP0_rst)
    begin
        CP0_reg[0] <= 32'h0;
        CP0_reg[1] <= 32'h0;
        CP0_reg[2] <= 32'h0;
        CP0_reg[3] <= 32'h0;
        CP0_reg[4] <= 32'h0;
        CP0_reg[5] <= 32'h0;
        CP0_reg[6] <= 32'h0;
        CP0_reg[7] <= 32'h0;
        CP0_reg[8] <= 32'h0;
        CP0_reg[9] <= 32'h0;
        CP0_reg[10] <= 32'h0;
        CP0_reg[11] <= 32'h0;
        CP0_reg[12] <= 32'h0;
        CP0_reg[13] <= 32'h0;
        CP0_reg[14] <= 32'h0;
        CP0_reg[15] <= 32'h0;
        CP0_reg[16] <= 32'h0;
        CP0_reg[17] <= 32'h0;
        CP0_reg[18] <= 32'h0;
        CP0_reg[19] <= 32'h0;
        CP0_reg[20] <= 32'h0;
        CP0_reg[21] <= 32'h0;
        CP0_reg[22] <= 32'h0;
        CP0_reg[23] <= 32'h0;
        CP0_reg[24] <= 32'h0;
        CP0_reg[25] <= 32'h0;
        CP0_reg[26] <= 32'h0;
        CP0_reg[27] <= 32'h0;
        CP0_reg[28] <= 32'h0;
        CP0_reg[29] <= 32'h0;
        CP0_reg[30] <= 32'h0;
        CP0_reg[31] <= 32'h0;
    end
    else if(CP0_ena)
    begin
        if(mtc0)
        begin
            CP0_reg[CP0_addr] <= CP0_data_in;
        end
        else if(CP0_choice == SYSCALL || CP0_choice == BREAK || CP0_choice == TEQ)
        begin
            CP0_reg[STATUS_REG_NUM] <= CP0_reg[STATUS_REG_NUM] << 5;
            CP0_reg[CAUSE_REG_NUM] <= {24'b0, CP0_choice, 2'b0};
            CP0_reg[EPC_REG_NUM] <= pc;
        end
        else if(eret)
        begin
            CP0_reg[STATUS_REG_NUM] <= CP0_reg[STATUS_REG_NUM] >> 5;
        end
    end 
end

endmodule
