`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 23:01:01
// Design Name: 
// Module Name: regfile
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


module regfile(
    input ref_clk,
    input ref_ena,
    input ref_rst,

    input ref_r,
    input ref_w,

    input [4:0] RsC,
    input [4:0] RtC,
    input [4:0] RdC,

    input [31:0] Rd_data_in,
    output [31:0] Rs_data_out,
    output [31:0] Rt_data_out

    );

reg [31:0] array_reg [31:0];              //定义的寄存器堆

//对输出的值进行获取
assign Rs_data_out = ref_ena? array_reg[RsC] : 32'hz;
assign Rt_data_out = ref_ena? array_reg[RtC] : 32'hz;


always @(negedge ref_clk or posedge ref_rst)
begin
    if(ref_ena && ref_rst)
    begin
        array_reg[0] <= 32'h0;
        array_reg[1] <= 32'h0;
        array_reg[2] <= 32'h0;
        array_reg[3] <= 32'h0;
        array_reg[4] <= 32'h0;
        array_reg[5] <= 32'h0;
        array_reg[6] <= 32'h0;
        array_reg[7] <= 32'h0;
        array_reg[8] <= 32'h0;
        array_reg[9] <= 32'h0;
        array_reg[10] <= 32'h0;
        array_reg[11] <= 32'h0;
        array_reg[12] <= 32'h0;
        array_reg[13] <= 32'h0;
        array_reg[14] <= 32'h0;
        array_reg[15] <= 32'h0;
        array_reg[16] <= 32'h0;
        array_reg[17] <= 32'h0;
        array_reg[18] <= 32'h0;
        array_reg[19] <= 32'h0;
        array_reg[20] <= 32'h0;
        array_reg[21] <= 32'h0;
        array_reg[22] <= 32'h0;
        array_reg[23] <= 32'h0;
        array_reg[24] <= 32'h0;
        array_reg[25] <= 32'h0;
        array_reg[26] <= 32'h0;
        array_reg[27] <= 32'h0;
        array_reg[28] <= 32'h0;
        array_reg[29] <= 32'h0;
        array_reg[30] <= 32'h0;
        array_reg[31] <= 32'h0;
    end
    else if(ref_ena && ref_w && (RdC != 5'h0))
    begin
        array_reg[RdC] <= Rd_data_in;
    end
    
end

endmodule
