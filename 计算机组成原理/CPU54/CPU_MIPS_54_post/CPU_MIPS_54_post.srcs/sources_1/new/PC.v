`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 23:00:25
// Design Name: 
// Module Name: PC
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


module PC(
    input pc_clk,
    input pc_ena,
    input pc_rst,
    input [31:0] pc_addr_in,

    output [31:0] pc_addr_out

    );

//用一个寄存器来存储pc
reg [31:0] pc_reg = 32'h00400000;

//时刻更新输出的pc
assign pc_addr_out = pc_ena? pc_reg : 32'hz;

//时钟下降沿或者复位信号上升沿触发
always @(negedge pc_clk or posedge pc_rst)
begin
    if(pc_ena && pc_rst)             //如果pc_rst为高电平，就复位
    begin
        pc_reg <= 32'h00400000;
    end
    else if(pc_ena)                  //如果pc_rst不为高电平，必为时钟下降沿
    begin
        pc_reg <= pc_addr_in;
    end
    
end

endmodule
