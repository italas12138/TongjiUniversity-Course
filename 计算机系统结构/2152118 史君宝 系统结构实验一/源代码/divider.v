`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Usage    用于下板时的分频器模块
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module divider#(parameter num = 2)(
    input I_CLK,
    input rst,
    output reg O_CLK
    );
    integer i = 0;
    always @ (posedge I_CLK or posedge rst) begin
        if(rst == 1) begin
            O_CLK <= 0; i <= 0; end
        else begin
            if(i == num - 1) begin
                O_CLK <= ~O_CLK; i <= 0; end
            else begin
                i <= i + 1; end
        end
    end
endmodule