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
    input clk_in,       //时钟信号
    input reset,        //复位信号
    output [7:0] o_seg,
    output [7:0] o_sel
    );

/* IMEM用 */
wire [31:0] IMEM_addr_in;      //11位的指令地址，从IMEM中读指令
wire [31:0] IMEM_inst_out;     //读出的32位指令

/* DMEM用 */
wire DMEM_ena;                 //DMEM使能信号
wire DMEM_r;                   //读指令
wire DMEM_w;                   //写指令
wire [31:0] DMEM_addr;         //需要用到的DMEM地址
wire [31:0] DMEM_data_out;     //从DMEM读出的数据
wire [31:0] DMEM_data_in;      //向DMEM写入的数据

wire sb_ena;               //当前写入指令是否是SB发出
wire sh_ena;               //当前写入指令是否是SH发出
wire sw_ena;               //当前写入指令是否是SW发出
wire lb_ena;               //当前读取指令是否是LB发出
wire lh_ena;               //当前读取指令是否是LH发出
wire lbu_ena;              //当前读取指令是否是LBU发出
wire lhu_ena;              //当前读取指令是否是LHU发出
wire lw_ena;               //当前读取指令是否是LW发出

/* CPU用 */
wire [31:0] pc_out;            //输出的PC地址，通过转化后输入到IMEM中，取指令
wire [31:0] DMEM_addr_temp;    //输出的DMEM临时地址，通过转化后输入到DMEM中

//IMEM 指令地址转化方式
assign IMEM_addr_in = pc_out - 32'h00400000;

//DMEM 数据地址转化方式
assign DMEM_addr = (DMEM_addr_temp -  32'h10010000)/4;

/* 自己使用 */
assign pc = pc_out;
assign inst = IMEM_inst_out;

wire clk_cpu;

/* IMEM指令存储器调用 */
IMEM Imem(
    .IMEM_addr_in(IMEM_addr_in[12:2]),  //11位的指令地址，从IMEM中读指令
    .IMEM_inst_out(IMEM_inst_out)       //读出的32位指令
    );

/* DMEM数据存储器调用 */
DMEM Dmem(                          
    .DMEM_clk(clk_cpu),                //DMEM时钟信号，只在写数据时使用
    .DMEM_ena(DMEM_ena),                //使能信号端，高电平有效，有效时才能读取/写入数据
    .DMEM_r(DMEM_r),                    //read读信号，读取时拉高
    .DMEM_w(DMEM_w),                    //write写信号，写入时拉高
    .DMEM_addr(DMEM_addr[6:0]),         //7位地址，要读取/写入的地址
    .DMEM_data_in(DMEM_data_in),         //写入时要写入的数据
    .DMEM_data_out(DMEM_data_out),       //读取时读取到的数据    
    
    .sb_ena(sb_ena),              //当前写入指令是否是SB发出
    .sh_ena(sh_ena),              //当前写入指令是否是SH发出
    .sw_ena(sw_ena),              //当前写入指令是否是SW发出
    .lb_ena(lb_ena),              //当前读取指令是否是LB发出
    .lh_ena(lh_ena),              //当前读取指令是否是LH发出
    .lbu_ena(lbu_ena),            //当前读取指令是否是LBU发出
    .lhu_ena(lhu_ena),            //当前读取指令是否是LHU发出
    .lw_ena(lw_ena)              //当前读取指令是否是LW发出

    );

/* CPU调用 */
cpu sccpu(
    .CPU_clk(clk_cpu),                   //CPU的时钟信号
    .CPU_ena(1'b1),                     //CPU的使能信号
    .CPU_rst(reset),                    //CPU的复位信号
    .inst_in(IMEM_inst_out),            //当前执行的指令
    .DMEM_data_out(DMEM_data_out),      //从DMEM中读取的数据
    .DMEM_ena(DMEM_ena),                //DMEM的使能信号
    .DMEM_r(DMEM_r),                    //DMEM的读信号
    .DMEM_w(DMEM_w),                    //DMEM的写信号

    .pc_out(pc_out),                    //下一条要执行的指令地址
    .DMEM_addr_temp(DMEM_addr_temp),    //要用到的DMEM地址，需转换
    .DMEM_data_in(DMEM_data_in),          //要写入DMEM的数据 
    
    .sb_ena(sb_ena),              //当前写入指令是否是SB发出
    .sh_ena(sh_ena),              //当前写入指令是否是SH发出
    .sw_ena(sw_ena),              //当前写入指令是否是SW发出
    .lb_ena(lb_ena),              //当前读取指令是否是LB发出
    .lh_ena(lh_ena),              //当前读取指令是否是LH发出
    .lbu_ena(lbu_ena),            //当前读取指令是否是LBU发出
    .lhu_ena(lhu_ena),            //当前读取指令是否是LHU发出
    .lw_ena(lw_ena)               //当前读取指令是否是LW发出
    
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
    .clk(clk_in),                   //系统时钟
    .rst_n(~reset),                 //复位信号
    .clk_out(clk_cpu)               //输出适配CPU的时钟
    );


endmodule
