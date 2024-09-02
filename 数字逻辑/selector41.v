

module selector41(iC0,iC1,iC2,iC3,iS1,iS0,oZ);
    input [3:0] iC0,iC1,iC2,iC3;
    input iS1,iS0;
    output [3:0] oZ;
    
    assign oZ=(iS1==1)?(iS0==1?iC3:iC2):(iS0==1?iC1:iC0);
endmodule
