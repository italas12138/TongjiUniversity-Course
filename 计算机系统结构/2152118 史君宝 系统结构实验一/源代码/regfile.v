`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    ��32���Ĵ�����regfileģ��
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module regfile(
    input clk,
    input rst,
    input write,
    input [4:0] rna,
    input [4:0] rnb,
    input [4:0] waddr,
    input [31:0] idata,
    output [31:0] odata1,
    output [31:0] odata2
    );
    
    integer i;
    reg [31:0] array_reg[31:0];
     
    //�ӼĴ�����ȡ����
    assign odata1 = (rna)?array_reg[rna]:0;
    assign odata2 = (rnb)?array_reg[rnb]:0;
    
    //������д��Ĵ���
    always@(posedge clk or posedge rst) begin
        if(rst==1) begin
            for(i=0;i<32;i=i+1) begin
                array_reg[i]<=0; end
        end
        else if(waddr != 0 && write) begin
            array_reg[waddr]<=idata;
        end
    end
//    // ������Ҫ����ʱ������   
//    always@(posedge clk or posedge rst) begin
//        if(rst==1) begin
//            for(i=0;i<6;i=i+1) begin
//                array_reg[i]<=0; end
//            for(i=8;i<32;i=i+1) begin
//                array_reg[i]<=0; end
//            array_reg[6] <= 2;
//            array_reg[7] <= 6;
//        end
//        else if(waddr != 0 && write) begin
//            array_reg[waddr]<=idata;
//        end
//    end
endmodule