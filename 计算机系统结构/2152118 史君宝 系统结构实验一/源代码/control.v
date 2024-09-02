`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    ID¿ØÖÆÆ÷Ä£¿é
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

module control(
	input [5:0] op,
    input [5:0] func,
    input [31:0] rs_data,
    input [31:0] rt_data,

    output jump,
    output DM_w_ID,
    output write_ID,
    output [3:0] aluc_ID,

    output [1:0] mux_pc, 
    output mux_alua_ID,
    output [1:0] mux_alub_ID, 
    output [1:0] mux_waddr_ID,
    output [1:0] mux_wdata_ID
    );
    
    wire r_type = ~(op[5]|op[4]|op[3]|op[2]|op[1]|op[0]);
    wire ADD = r_type&func[5]&~func[4]&~func[3]&~func[2]&~func[1]&~func[0];
    wire ADDU = r_type&func[5]&~func[4]&~func[3]&~func[2]&~func[1]&func[0];
    wire SUB = r_type&func[5]&~func[4]&~func[3]&~func[2]&func[1]&~func[0];
    wire SUBU = r_type&func[5]&~func[4]&~func[3]&~func[2]&func[1]&func[0];
    wire AND = r_type&func[5]&~func[4]&~func[3]&func[2]&~func[1]&~func[0];
    wire OR = r_type&func[5]&~func[4]&~func[3]&func[2]&~func[1]&func[0];
    wire XOR = r_type&func[5]&~func[4]&~func[3]&func[2]&func[1]&~func[0];
    wire NOR = r_type&func[5]&~func[4]&~func[3]&func[2]&func[1]&func[0];
    wire SLT = r_type&func[5]&~func[4]&func[3]&~func[2]&func[1]&~func[0];
    wire SLTU = r_type&func[5]&~func[4]&func[3]&~func[2]&func[1]&func[0];
    wire SLL = r_type&~func[5]&~func[4]&~func[3]&~func[2]&~func[1]&~func[0];
    wire SRL = r_type&~func[5]&~func[4]&~func[3]&~func[2]&func[1]&~func[0];
    wire SRA = r_type&~func[5]&~func[4]&~func[3]&~func[2]&func[1]&func[0];
    wire SLLV = r_type&~func[5]&~func[4]&~func[3]&func[2]&~func[1]&~func[0];
    wire SRLV = r_type&~func[5]&~func[4]&~func[3]&func[2]&func[1]&~func[0];
    wire SRAV = r_type&~func[5]&~func[4]&~func[3]&func[2]&func[1]&func[0];
    wire JR = r_type&~func[5]&~func[4]&func[3]&~func[2]&~func[1]&~func[0];
    wire ADDI = ~op[5]&~op[4]&op[3]&~op[2]&~op[1]&~op[0];
    wire ADDIU = ~op[5]&~op[4]&op[3]&~op[2]&~op[1]&op[0];
    wire ANDI = ~op[5]&~op[4]&op[3]&op[2]&~op[1]&~op[0];
    wire ORI = ~op[5]&~op[4]&op[3]&op[2]&~op[1]&op[0];
    wire XORI = ~op[5]&~op[4]&op[3]&op[2]&op[1]&~op[0];
    wire LUI = ~op[5]&~op[4]&op[3]&op[2]&op[1]&op[0];
    wire LW = op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0];
    wire SW = op[5]&~op[4]&op[3]&~op[2]&op[1]&op[0];
    wire BEQ = ~op[5]&~op[4]&~op[3]&op[2]&~op[1]&~op[0];
    wire BNE = ~op[5]&~op[4]&~op[3]&op[2]&~op[1]&op[0];
    wire SLTI = ~op[5]&~op[4]&op[3]&~op[2]&op[1]&~op[0];
    wire SLTIU = ~op[5]&~op[4]&op[3]&~op[2]&op[1]&op[0];
    wire J = ~op[5]&~op[4]&~op[3]&~op[2]&op[1]&~op[0];
    wire JAL = ~op[5]&~op[4]&~op[3]&~op[2]&op[1]&op[0];

	assign DM_w_ID = SW;
	assign write_ID = ~(JR|SW|BEQ|BNE|J);
	assign jump = JR|J|JAL|(BEQ&(rs_data == rt_data))|(BNE&(rs_data != rt_data));
    
    assign aluc_ID[3] = LUI|SLL|SLLV|SLT|SLTI|SLTIU|SLTU|SRA|SRAV|SRL|SRLV;
    assign aluc_ID[2] = AND|ANDI|NOR|OR|ORI|SLL|SLLV|SRA|SRAV|SRL|SRLV|XOR|XORI;
    assign aluc_ID[1] = ADD|ADDI|BEQ|BNE|LW|NOR|SLL|SLLV|SLT|SLTI|SLTIU|SLTU|SUB|SW|XOR|XORI;
    assign aluc_ID[0] = BEQ|BNE|NOR|OR|ORI|SLT|SLTI|SRL|SRLV|SUB|SUBU;

	assign mux_pc = (J|JAL)?2'b00:(JR)?2'b01:(BNE|BEQ)?2'b11:2'bxx;
	assign mux_alua_ID = SLL|SRA|SRL;
	assign mux_alub_ID[1] = ~(ADDI|ADDIU|LUI|LW|SLTI|SW|ANDI|ORI|SLTIU|XORI);
	assign mux_alub_ID[0] = ANDI|ORI|SLTIU|XORI;
	assign mux_wdata_ID[1] = JAL;
	assign mux_wdata_ID[0] = LW;
	assign mux_waddr_ID[1] = JAL;
	assign mux_waddr_ID[0] = ~(ADDI|ADDIU|ANDI|LUI|LW|ORI|SLTI|SLTIU|XORI|JAL);
endmodule