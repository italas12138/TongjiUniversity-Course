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

//��һ���Ĵ������洢pc
reg [31:0] pc_reg = 32'h00400000;

//ʱ�̸��������pc
assign pc_addr_out = pc_ena? pc_reg : 32'hz;

//ʱ���½��ػ��߸�λ�ź������ش���
always @(negedge pc_clk or posedge pc_rst)
begin
    if(pc_ena && pc_rst)             //���pc_rstΪ�ߵ�ƽ���͸�λ
    begin
        pc_reg <= 32'h00400000;
    end
    else if(pc_ena)                  //���pc_rst��Ϊ�ߵ�ƽ����Ϊʱ���½���
    begin
        pc_reg <= pc_addr_in;
    end
    
end

endmodule
