`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 14:25:46
// Design Name: 
// Module Name: HI_LO
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


module HI_LO(
    input HI_LO_clk,
    input HI_LO_ena,
    input HI_LO_rst,

    input [31:0] HI_data_in,
    input [31:0] LO_data_in,

    input HI_w,
    input LO_w,

    output [31:0] HI_data_out,
    output [31:0] LO_data_out
    );


reg [31:0] HI_reg;
reg [31:0] LO_reg;

assign HI_data_out = (HI_LO_ena)? HI_reg : 32'hz;
assign LO_data_out = (HI_LO_ena)? LO_reg : 32'hz;


always @(negedge HI_LO_clk or posedge HI_LO_rst)
begin
    if(HI_LO_ena && HI_LO_rst)
    begin
        HI_reg <= 32'b0;
        LO_reg <= 32'b0;
    end
    else if(HI_LO_ena)
    begin
        if(HI_w)
        begin
            HI_reg <= HI_data_in;
        end
        if(LO_w)
        begin
            LO_reg <= LO_data_in;
        end
    end
end

endmodule
