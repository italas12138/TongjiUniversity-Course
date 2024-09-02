`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/12 14:46:10
// Design Name: 
// Module Name: CLZ
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


module CLZ(
    input [31:0] CLZ_data_in,

    output [31:0] CLZ_cnt_out
    );


reg [31:0] cnt_reg;


assign CLZ_cnt_out = cnt_reg;


always @(*)
begin
    if(CLZ_data_in[31] == 1'b1)
    begin
        cnt_reg <= 0;
    end
    else if(CLZ_data_in[30] == 1'b1 && CLZ_data_in[31] == 0) 
    begin
        cnt_reg <= 1;
    end
    else if(CLZ_data_in[29] == 1'b1 && CLZ_data_in[31:30] == 0) 
    begin
        cnt_reg <= 2;
    end
    else if(CLZ_data_in[28] == 1'b1 && CLZ_data_in[31:29] == 0) 
    begin
        cnt_reg <= 3;
    end
    else if(CLZ_data_in[27] == 1'b1 && CLZ_data_in[31:28] == 0) 
    begin
        cnt_reg <= 4;
    end
    else if(CLZ_data_in[26] == 1'b1 && CLZ_data_in[31:27] == 0) 
    begin
        cnt_reg <= 5;
    end
    else if(CLZ_data_in[25] == 1'b1 && CLZ_data_in[31:26] == 0) 
    begin
        cnt_reg <= 6;
    end
    else if(CLZ_data_in[24] == 1'b1 && CLZ_data_in[31:25] == 0) 
    begin
        cnt_reg <= 7;
    end
    else if(CLZ_data_in[23] == 1'b1 && CLZ_data_in[31:24] == 0) 
    begin
        cnt_reg <= 8;
    end
    else if(CLZ_data_in[22] == 1'b1 && CLZ_data_in[31:23] == 0) 
    begin
        cnt_reg <= 9;
    end
    else if(CLZ_data_in[21] == 1'b1 && CLZ_data_in[31:22] == 0) 
    begin
        cnt_reg <= 10;
    end
    else if(CLZ_data_in[20] == 1'b1 && CLZ_data_in[31:21] == 0) 
    begin
        cnt_reg <= 11;
    end
    else if(CLZ_data_in[19] == 1'b1 && CLZ_data_in[31:20] == 0) 
    begin
        cnt_reg <= 12;
    end
    else if(CLZ_data_in[18] == 1'b1 && CLZ_data_in[31:19] == 0) 
    begin
        cnt_reg <= 13;
    end
    else if(CLZ_data_in[17] == 1'b1 && CLZ_data_in[31:18] == 0) 
    begin
        cnt_reg <= 14;
    end
    else if(CLZ_data_in[16] == 1'b1 && CLZ_data_in[31:17] == 0) 
    begin
        cnt_reg <= 15;
    end
    else if(CLZ_data_in[15] == 1'b1 && CLZ_data_in[31:16] == 0) 
    begin
        cnt_reg <= 16;
    end
    else if(CLZ_data_in[14] == 1'b1 && CLZ_data_in[31:15] == 0) 
    begin
        cnt_reg <= 17;
    end
    else if(CLZ_data_in[13] == 1'b1 && CLZ_data_in[31:14] == 0) 
    begin
        cnt_reg <= 18;
    end
    else if(CLZ_data_in[12] == 1'b1 && CLZ_data_in[31:13] == 0) 
    begin
        cnt_reg <= 19;
    end
    else if(CLZ_data_in[11] == 1'b1 && CLZ_data_in[31:12] == 0) 
    begin
        cnt_reg <= 20;
    end
    else if(CLZ_data_in[10] == 1'b1 && CLZ_data_in[31:11] == 0) 
    begin
        cnt_reg <= 21;
    end
    else if(CLZ_data_in[9] == 1'b1 && CLZ_data_in[31:10] == 0) 
    begin
        cnt_reg <= 22;
    end
    else if(CLZ_data_in[8] == 1'b1 && CLZ_data_in[31:9] == 0) 
    begin
        cnt_reg <= 23;
    end
    else if(CLZ_data_in[7] == 1'b1 && CLZ_data_in[31:8] == 0) 
    begin
        cnt_reg <= 24;
    end
    else if(CLZ_data_in[6] == 1'b1 && CLZ_data_in[31:7] == 0) 
    begin
        cnt_reg <= 25;
    end
    else if(CLZ_data_in[5] == 1'b1 && CLZ_data_in[31:6] == 0) 
    begin
        cnt_reg <= 26;
    end
    else if(CLZ_data_in[4] == 1'b1 && CLZ_data_in[31:5] == 0) 
    begin
        cnt_reg <= 27;
    end
    else if(CLZ_data_in[3] == 1'b1 && CLZ_data_in[31:4] == 0) 
    begin
        cnt_reg <= 28;
    end
    else if(CLZ_data_in[2] == 1'b1 && CLZ_data_in[31:3] == 0) 
    begin
        cnt_reg <= 29;
    end
    else if(CLZ_data_in[1] == 1'b1 && CLZ_data_in[31:2] == 0) 
    begin
        cnt_reg <= 30;
    end
    else if(CLZ_data_in[0] == 1'b1 && CLZ_data_in[31:1] == 0) 
    begin
        cnt_reg <= 31;
    end
    else if(CLZ_data_in == 0)
    begin
        cnt_reg <= 32;
    end
end

endmodule
