`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    luiºÍsltÔËËãÄ£¿é
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module luislt(
	input [31:0] a,
	input [31:0] b,
	input [1:0] aluc,
	output reg [31:0] r
    );

	reg [2:0] compare;
	reg r_low = 1'b0;
	wire [31:0] r_lui, r_slt;
	wire [32:0] ar = {1'b0, a};
	wire [32:0] br = {1'b0, b};
	assign r_lui = {b[15:0], 16'b0};
	assign r_slt[31:1] = 0;
	assign r_slt[0] = r_low;

	always @(ar or br) begin
		if(ar > br) begin
			compare = 3'b100; end 
		else if(ar == br) begin
			compare = 3'b010; end 
		else begin
			compare = 3'b001; end
	end

	always @(*) begin
		if(aluc[0] == 1'b1) begin
			case({a[31], b[31]})
			2'b00: begin
				r_low = compare[0]; end
			2'b01: begin
				r_low = 1'b0; end
			2'b10: begin
				r_low = 1'b1; end
			2'b11: begin
				r_low = compare[0];	end
			endcase end 
		else begin
			r_low = compare[0]; end
	end

	always @(aluc or r_lui or r_slt) begin
		if(aluc[1]) begin
			r = r_slt; end 
		else begin
			r = r_lui; end
	end
endmodule