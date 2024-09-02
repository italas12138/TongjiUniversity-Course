`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/04/03 20:0:43
// Design Name: 
// Module Name: DIVU_tb
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

module DIV_tb;
    reg [31:0] dividend,divisor;
    reg clock;
    reg reset;
    reg start;
    wire [31:0] q,r;
    wire busy;
    DIV uut(.dividend(dividend),.divisor(divisor),.clock(clock),.reset(reset),.start(start),.q(q),.r(r),.busy(busy));

    initial
    begin
        clock=0;
        reset=1;
        start=1;
        dividend=0;
        divisor=0;
        
        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=10;
        divisor=3;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;
        
        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=10;
        divisor=-3;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=-10;
        divisor=3;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=-10;
        divisor=-3;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b11101111;
        divisor=32'b1011;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

         #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b1110111111;
        divisor=32'b11011;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;   

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b00000000000000000000000000000000;
        divisor=32'b11111111111111111111111111111111;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;
        
        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b11111111111111111111111111111000;
        divisor=32'b00000000000000000000000000000011;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b00000000000000000111111111111111;
        divisor=32'b00000000000000001000000000000000;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b00000000000000000000000000001000;
        divisor=32'b11111111111111111111111111111101;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

        #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b11111111111111111111111111111000;
        divisor=32'b00000000000000000000000000000010 ;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;

         #15;
        clock=~clock;

        #15;
        reset=0;
        clock=~clock;
        dividend=32'b01010101010101010101010101010101;
        divisor=32'b10101010101010101010101010101010;
        
        #25;
        clock=~clock;
        
        #15;
        reset=1;
        clock=~clock;   

    end
endmodule
