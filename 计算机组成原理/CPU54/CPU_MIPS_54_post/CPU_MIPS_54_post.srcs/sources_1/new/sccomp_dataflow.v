`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 22:58:39
// Design Name: 
// Module Name: sccomp_dataflow
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


`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/07/10 15:11:57
// Design Name: 
// Module Name: sccomp_dataflow
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

module sccomp_dataflow(
    input clk_in,       //ʱ���ź�
    input reset,        //��λ�ź�
    output [7:0] o_seg,
    output [7:0] o_sel
    );

/* IMEM�� */
wire [31:0] IMEM_addr_in;      //11λ��ָ���ַ����IMEM�ж�ָ��
wire [31:0] IMEM_inst_out;     //������32λָ��

/* DMEM�� */
wire DMEM_ena;                 //DMEMʹ���ź�
wire DMEM_r;                   //��ָ��
wire DMEM_w;                   //дָ��
wire [31:0] DMEM_addr;         //��Ҫ�õ���DMEM��ַ
wire [31:0] DMEM_data_out;     //��DMEM����������
wire [31:0] DMEM_data_in;      //��DMEMд�������

wire sb_ena;               //��ǰд��ָ���Ƿ���SB����
wire sh_ena;               //��ǰд��ָ���Ƿ���SH����
wire sw_ena;               //��ǰд��ָ���Ƿ���SW����
wire lb_ena;               //��ǰ��ȡָ���Ƿ���LB����
wire lh_ena;               //��ǰ��ȡָ���Ƿ���LH����
wire lbu_ena;              //��ǰ��ȡָ���Ƿ���LBU����
wire lhu_ena;              //��ǰ��ȡָ���Ƿ���LHU����
wire lw_ena;               //��ǰ��ȡָ���Ƿ���LW����

/* CPU�� */
wire [31:0] pc_out;            //�����PC��ַ��ͨ��ת�������뵽IMEM�У�ȡָ��
wire [31:0] DMEM_addr_temp;    //�����DMEM��ʱ��ַ��ͨ��ת�������뵽DMEM��

//IMEM ָ���ַת����ʽ
assign IMEM_addr_in = pc_out - 32'h00400000;

//DMEM ���ݵ�ַת����ʽ
assign DMEM_addr = (DMEM_addr_temp -  32'h10010000)/4;

/* �Լ�ʹ�� */
assign pc = pc_out;
assign inst = IMEM_inst_out;

wire clk_cpu;

/* IMEMָ��洢������ */
IMEM Imem(
    .IMEM_addr_in(IMEM_addr_in[12:2]),  //11λ��ָ���ַ����IMEM�ж�ָ��
    .IMEM_inst_out(IMEM_inst_out)       //������32λָ��
    );

/* DMEM���ݴ洢������ */
DMEM Dmem(                          
    .DMEM_clk(clk_cpu),                //DMEMʱ���źţ�ֻ��д����ʱʹ��
    .DMEM_ena(DMEM_ena),                //ʹ���źŶˣ��ߵ�ƽ��Ч����Чʱ���ܶ�ȡ/д������
    .DMEM_r(DMEM_r),                    //read���źţ���ȡʱ����
    .DMEM_w(DMEM_w),                    //writeд�źţ�д��ʱ����
    .DMEM_addr(DMEM_addr[6:0]),         //7λ��ַ��Ҫ��ȡ/д��ĵ�ַ
    .DMEM_data_in(DMEM_data_in),         //д��ʱҪд�������
    .DMEM_data_out(DMEM_data_out),       //��ȡʱ��ȡ��������    
    
    .sb_ena(sb_ena),              //��ǰд��ָ���Ƿ���SB����
    .sh_ena(sh_ena),              //��ǰд��ָ���Ƿ���SH����
    .sw_ena(sw_ena),              //��ǰд��ָ���Ƿ���SW����
    .lb_ena(lb_ena),              //��ǰ��ȡָ���Ƿ���LB����
    .lh_ena(lh_ena),              //��ǰ��ȡָ���Ƿ���LH����
    .lbu_ena(lbu_ena),            //��ǰ��ȡָ���Ƿ���LBU����
    .lhu_ena(lhu_ena),            //��ǰ��ȡָ���Ƿ���LHU����
    .lw_ena(lw_ena)              //��ǰ��ȡָ���Ƿ���LW����

    );

/* CPU���� */
cpu sccpu(
    .CPU_clk(clk_cpu),                   //CPU��ʱ���ź�
    .CPU_ena(1'b1),                     //CPU��ʹ���ź�
    .CPU_rst(reset),                    //CPU�ĸ�λ�ź�
    .inst_in(IMEM_inst_out),            //��ǰִ�е�ָ��
    .DMEM_data_out(DMEM_data_out),      //��DMEM�ж�ȡ������
    .DMEM_ena(DMEM_ena),                //DMEM��ʹ���ź�
    .DMEM_r(DMEM_r),                    //DMEM�Ķ��ź�
    .DMEM_w(DMEM_w),                    //DMEM��д�ź�

    .pc_out(pc_out),                    //��һ��Ҫִ�е�ָ���ַ
    .DMEM_addr_temp(DMEM_addr_temp),    //Ҫ�õ���DMEM��ַ����ת��
    .DMEM_data_in(DMEM_data_in),          //Ҫд��DMEM������ 
    
    .sb_ena(sb_ena),              //��ǰд��ָ���Ƿ���SB����
    .sh_ena(sh_ena),              //��ǰд��ָ���Ƿ���SH����
    .sw_ena(sw_ena),              //��ǰд��ָ���Ƿ���SW����
    .lb_ena(lb_ena),              //��ǰ��ȡָ���Ƿ���LB����
    .lh_ena(lh_ena),              //��ǰ��ȡָ���Ƿ���LH����
    .lbu_ena(lbu_ena),            //��ǰ��ȡָ���Ƿ���LBU����
    .lhu_ena(lhu_ena),            //��ǰ��ȡָ���Ƿ���LHU����
    .lw_ena(lw_ena)               //��ǰ��ȡָ���Ƿ���LW����
    
    );

seg7x16 seg7x16_inst(
    .clk(clk_in),
	.reset(reset),
	.cs(1'b1),
	.i_data(IMEM_inst_out),
	.o_seg(o_seg),
	.o_sel(o_sel)
    );

Divider Divider_inst(
    .clk(clk_in),                   //ϵͳʱ��
    .rst_n(~reset),                 //��λ�ź�
    .clk_out(clk_cpu)               //�������CPU��ʱ��
    );


endmodule
