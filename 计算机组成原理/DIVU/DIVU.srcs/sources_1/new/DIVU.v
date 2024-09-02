`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/04/03 18:01:25
// Design Name: 
// Module Name: DIVU
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


module DIVU(
    input [31:0] dividend,
    input [31:0] divisor,
    input start,
    input clock,
    input reset,
    output reg [31:0] q,
    output reg [31:0] r,
    output reg busy
    );
    integer i;
    reg [31:0] reg_dividend,reg_divisor;
    reg [63:0] temp_dividend;
    
    always @(posedge clock or posedge reset)
    begin
        if(reset)
        begin
            reg_dividend=32'b0;
            reg_divisor=32'b0;
            temp_dividend=64'b0;
            busy=1'b0;
            q=32'b0;
            r=32'b0;
        end
        else
        begin
            if(start)
            begin
                reg_dividend=dividend;
                reg_divisor=divisor;
                temp_dividend={32'b0,dividend};  
           
                busy=1'b1;
                for(i = 0;i < 32;i = i + 1)  
                begin
                    temp_dividend = {temp_dividend[62:0],1'b0};  
                    if(temp_dividend[63:32] >= reg_divisor)
                    begin  
                        temp_dividend[63:32] = temp_dividend[63:32]- reg_divisor;  
                        temp_dividend=temp_dividend+1'b1;
                    end
                    else  
                        temp_dividend=temp_dividend;  
                    end  
                end
                busy=1'b0;
                q=temp_dividend[31:0];
                r=temp_dividend[63:32];
            end
        end 
endmodule  


   



