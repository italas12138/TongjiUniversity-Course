`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    alu������ģ��
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module alu(
	input [31:0] a,
	input [31:0] b,
	input [3:0] aluc,
	output reg [31:0] result
    );

	// �Ӽ����������
	wire [31:0] result_0;
	adder adder(a, b, aluc[0], result_0);

	// lui��slt�������
	wire [31:0] result_1;
	luislt luislt(a, b, aluc[1:0], result_1);

	// result�������
	always @(*) begin
		case(aluc[3:2])
		2'b00: begin //�Ӽ�����
			result = result_0; end
		2'b10: begin //lui��slt����
			result = result_1; end
		2'b01: begin //������ǻ����
			case(aluc[1:0])
            2'b00: begin	//��
				result = a & b; end
            2'b01: begin	//��
				result = a | b; end
            2'b10: begin	//���
				result = a ^ b; end
            default: begin	//�ǻ�
				result = ~(a | b); end
			endcase end
		2'b11: begin //��λ����
			case(aluc[1:0])
            2'b00: begin	//��������
                result = a[0]? {b[31], b[31:1]}: b;
                result = a[1]? {{2{result[31]}}, result[31:2]}: result;
                result = a[2]? {{4{result[31]}}, result[31:4]}: result;
                result = a[3]? {{8{result[31]}}, result[31:8]}: result;
                result = a[4]? {{16{result[31]}}, result[31:16]}: result; end 
            2'b01: begin	//�߼�����
                result = a[0]? {1'b0, b[31:1]}: b;
                result = a[1]? {2'b0, result[31:2]}: result;
                result = a[2]? {4'b0, result[31:4]}: result;
                result = a[3]? {8'b0, result[31:8]}: result;
                result = a[4]? {16'b0, result[31:16]}: result; end
            default: begin	//����
                result = a[0]? {b[30:0], 1'b0}: b;
                result = a[1]? {result[29:0], 2'b0}: result;
                result = a[2]? {result[27:0], 4'b0}: result;
                result = a[3]? {result[23:0], 8'b0}: result;
                result = a[4]? {result[15:0], 16'b0}: result; end
			endcase end
		default: begin
			result = 32'b0; end
		endcase
	end
endmodule