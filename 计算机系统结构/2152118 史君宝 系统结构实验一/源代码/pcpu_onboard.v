`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Usage    下板所需模块汇总
// Vision   1.0
// Auther   Ray 
//////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// (0) 下版模块：选择输出pc,inst,drop_cnt,egg_cnt,last_result
module pcpu_onboard(
    input clk_in,   // 板子时钟
    input reset,    // 复位信号
    input ready,    // 数据完备信号
    input id,       // 数据输入选择信号
    input yes,      // 数据输入确定信号
    input [7:0] choose, // 输入数据 0-255
    output working,
    output [7:0] o_seg,
    output [7:0] o_sel
    );
    wire [31:0] i_data;
    wire [31:0] eggs,floor;
    wire [31:0] inst,pc,drop_cnt,egg_cnt,last_result;
    wire clk_cpu;
    wire clk_seg;
    
    /////////////////////////////////////////////////////////////////////
    // CPU 运行前输入 eggs,floor
    reg state = 0;  // 状态机:0-输入 1-运行
    reg [31:0] temp;
    // 状态机变化
    always@(posedge ready or posedge reset) begin
        if(reset==1) begin
            state <= 0; end
        else if(ready==1) begin
            state <= 1; end
        else begin
            state <= state; end
    end
    // 数据保存
    always@(posedge clk_in or posedge reset) begin
        if(reset==1) begin
            temp <= 0; end
        else begin
            temp <= {24'b0,choose}; end
    end
    // 实际赋值
    assign eggs  = (state) ? eggs:((id==0) ? ((yes)?temp:eggs):eggs);
    assign floor = (state) ? floor:((id==1) ? ((yes)?temp:floor):floor);
    
    /////////////////////////////////////////////////////////////////////
    // 选择显示 eggs,floor,inst,pc,drop_cnt,egg_cnt,last_result
    assign working = state;
    assign i_data = (state==0) ? ((id==0) ? eggs:floor):(
                    (choose[6]) ? eggs : (
                    (choose[5]) ? floor :(
                    (choose[4]) ? inst :(
                    (choose[3]) ? pc :(
                    (choose[2]) ? drop_cnt :(
                    (choose[1]) ? egg_cnt :(
                    (choose[0]) ? last_result : 32'b0)))))));
    
    divider #(10000) div_cpu(clk_in,reset,clk_cpu);
    divider #(4) div_seg(clk_in,reset,clk_seg);
    seg7x16(clk_seg,reset,i_data,o_seg,o_sel);
    
    /////////////////////////////////////////////////////////////////////
    // pcpu_onboard 顶层部分
   	wire [31:0] a;
	wire clk_real;
	assign a = pc - 32'h00400000;
    assign clk_real = (state) ? ((inst != 32'b0) ? clk_cpu : 1'b0) : 1'b0;
	imem imem(a[12:2], inst);	
   	pcpu_board sccpu(clk_real,reset,inst,pc,drop_cnt,egg_cnt,last_result,eggs,floor);
endmodule

/////////////////////////////////////////////////////////
// (1) regfile_board 模块
module regfile_board (
    input clk,
    input rst,
    input write,
    input [4:0] rna,
    input [4:0] rnb,
    input [4:0] waddr,
    input [31:0] idata,
    output [31:0] odata1,
    output [31:0] odata2,
	output [31:0] reg_3,reg_4,reg_5,
	
	input [31:0] eggs,floor
    );
    
    integer i;
    reg [31:0] array_reg[31:0];
     
    //从寄存器读取数据
    assign reg_3 = array_reg[3];
    assign reg_4 = array_reg[4];
    assign reg_5 = array_reg[5];
    assign odata1 = (rna)?array_reg[rna]:0;
    assign odata2 = (rnb)?array_reg[rnb]:0;

    //将数据写入寄存器
    always@(posedge clk or posedge rst) begin
        if(rst==1) begin
            for(i=0;i<6;i=i+1) begin
                array_reg[i]<=0; end
            for(i=8;i<32;i=i+1) begin
                array_reg[i]<=0; end
            array_reg[6] <= eggs;
            array_reg[7] <= floor; end
        else if(waddr != 0 && write) begin
            array_reg[waddr]<=idata;
            array_reg[6] <= eggs;
            array_reg[7] <= floor; end
    end
endmodule

/////////////////////////////////////////////////////////
// (2) pcpu_board 模块
module pcpu_board(
    input clk,
    input rst,
    input [31:0] inst,
    output [31:0] pc,
    output [31:0] reg_3,reg_4,reg_5,
    
	input [31:0] eggs,floor
    );
    
    //#########################################################################
    //# IF 部分
    //#########################################################################
    wire jump;          // 跳转信号
    wire stall;         // 延迟信号
    wire [31:0] npc_IF; // 始终是npc=pc+4
    wire [31:0] pc_ID;  // ID段返回的pc值
    wire [31:0] pc_IF;  // IF段流出的pc
    wire [31:0] inst_IF;// IF段流出的inst
    assign npc_IF   = pc + 32'd4;
    assign pc_IF    = (stall) ? pc : (jump) ? pc_ID : npc_IF;
    assign inst_IF  = (stall||jump)? 32'b0 : inst;
    PC PC(clk,rst,pc_IF,pc);
    
    //#########################################################################
    //# Pipe_IF_ID 流水线寄存器
   //#########################################################################
    wire [31:0] inst_ID;
    wire [31:0] npc_ID;
    
    Pipe_IF_ID Pipe_IF_ID(
        .clk(clk),
        .rst(rst),
        .npc_IF(npc_IF),
        .inst_IF(inst_IF),
        .npc_ID(npc_ID),
        .inst_ID(inst_ID)
    );
    
    //#########################################################################
    //# ID 部分
    //#########################################################################
    //-inst_ID指令解码
    wire [5:0] op,func;
    wire [4:0] rs,rt,rd,sa;
    wire [15:0] imm16;
    wire [25:0] index;
    wire [31:0] sa32_ID;       // sa 拓展
    wire [31:0] uimm32_ID;     // imme(offset) 无符号拓展
    wire [31:0] simm32_ID;     // imme(offset) 有符号拓展
    wire [31:0] offset32;   // offset << 2 拓展
    assign func     = inst_ID[5:0];
    assign sa       = inst_ID[10:6];
    assign imm16    = inst_ID[15:0];
    assign index    = inst_ID[25:0];
    assign op       = inst_ID[31:26];
    assign rs       = inst_ID[25:21];
    assign rt       = inst_ID[20:16];
    assign rd       = inst_ID[15:11];
    assign sa32_ID     = {27'b0, sa};
    assign uimm32_ID   = {16'b0, imm16};
    assign simm32_ID   = {{16{imm16[15]}}, imm16};
    assign offset32 = {{14{imm16[15]}}, imm16, 2'b0};
    
    //-pc_ID选择
    wire [31:0] rs_data;    // 寄存器rs中的数据
    wire [31:0] rt_data;    // 寄存器rt中的数据
    wire [1:0] mux_pc;      // pc_ID 选择信号
    wire [31:0] pc_add;     // beq bne bgez: pc + offset32
    wire [31:0] pc_jjal;    // j jal: pc[31:28]和index
    wire [31:0] pc_rs;      // jr: reg中rs位置存储的数据
    assign pc_rs  = rs_data;
    assign pc_add  = pc + offset32;
    assign pc_jjal  = {npc_IF[31:28], index, 2'b0};
    assign pc_ID = (mux_pc[1]) ? pc_add : (mux_pc[0]) ? pc_rs : pc_jjal;
    
    //-控制信号产生
    wire DM_w_ID;           // ID段DMEM写信号
    wire write_ID;          // ID段写信号
    wire [3:0] aluc_ID;     // ID段aluc
    wire [4:0] waddr_ID;    // ID段写地址
    wire mux_alua_ID;           // ID段alua来源选择信号
    wire [1:0] mux_alub_ID;     // ID段alub来源选择信号
    wire [1:0] mux_waddr_ID;    // ID段写地址选择信号
    wire [1:0] mux_wdata_ID;    // ID段写数据选择信号
    assign waddr_ID = (mux_waddr_ID[1]) ? 5'd31 : (mux_waddr_ID[0]) ? rd : rt;
    control control(
        .op(op),
        .func(func),
        .rs_data(rs_data),
        .rt_data(rt_data),
        .jump(jump),
        .DM_w_ID(DM_w_ID), 
        .write_ID(write_ID), 
        .aluc_ID(aluc_ID), 
        .mux_pc(mux_pc), 
        .mux_alua_ID(mux_alua_ID), 
        .mux_alub_ID(mux_alub_ID), 
        .mux_waddr_ID(mux_waddr_ID), 
        .mux_wdata_ID(mux_wdata_ID)
    );
    
    //-冲突处理
    wire write_EXE;         // EXE段写信号
    wire write_MEM;         // MEM段写信号
    wire [4:0] waddr_EXE;   // EXE段写地址
    wire [4:0] waddr_MEM;   // MEM段写地址
    conflict conflict(
        .jump(jump),
        .inst(inst),
        .write_ID(write_ID),
        .write_EXE(write_EXE),
        .write_MEM(write_MEM),
        .waddr_ID(waddr_ID),
        .waddr_EXE(waddr_EXE),
        .waddr_MEM(waddr_MEM),
        .stall(stall)
    );
    
    //#########################################################################
    //# Pipe_ID_EXE 流水线寄存器
    //#########################################################################
    wire [31:0] sa32_EXE;
    wire [31:0] simm32_EXE;
    wire [31:0] uimm32_EXE;
    wire [31:0] rs_data_EXE;
    wire [31:0] rt_data_EXE;
    wire [3:0] aluc_EXE;
    wire [31:0] npc_EXE;
    wire [31:0] DM_wdata_EXE;
    wire DM_w_EXE;
    wire mux_alua_EXE;
    wire [1:0] mux_alub_EXE;
    wire [1:0] mux_wdata_EXE;
    
    Pipe_ID_EXE Pipe_ID_EXE(
        .clk(clk),
        .rst(rst),
    
        .DM_w_ID(DM_w_ID),
        .write_ID(write_ID),
        .mux_alua_ID(mux_alua_ID),
        .mux_alub_ID(mux_alub_ID),
        .mux_wdata_ID(mux_wdata_ID),
        .aluc_ID(aluc_ID),
        .npc_ID(npc_ID),
        .waddr_ID(waddr_ID),
        .sa32_ID(sa32_ID),
        .simm32_ID(simm32_ID),
        .uimm32_ID(uimm32_ID),
        .rs_data_ID(rs_data),
        .rt_data_ID(rt_data),
        .DM_wdata_ID(rt_data),
    
        .DM_w_EXE(DM_w_EXE),
        .write_EXE(write_EXE),
        .mux_wdata_EXE(mux_wdata_EXE),
        .mux_alua_EXE(mux_alua_EXE),
        .mux_alub_EXE(mux_alub_EXE),
        .aluc_EXE(aluc_EXE),
        .npc_EXE(npc_EXE),
        .waddr_EXE(waddr_EXE),
        .sa32_EXE(sa32_EXE),
        .simm32_EXE(simm32_EXE),
        .uimm32_EXE(uimm32_EXE),
        .rs_data_EXE(rs_data_EXE),
        .rt_data_EXE(rt_data_EXE),
        .DM_wdata_EXE(DM_wdata_EXE)
    );
    
    //#########################################################################
    //# EXE 部分
    //#########################################################################
    wire [31:0] alua;
    wire [31:0] alub;
    wire [31:0] alu_EXE;
    assign alua = mux_alua_EXE ? sa32_EXE : rs_data_EXE;
    assign alub = (mux_alub_EXE[1]) ? rt_data_EXE : (mux_alub_EXE[0]) ? uimm32_EXE : simm32_EXE;
    alu alu(alua,alub,aluc_EXE,alu_EXE);
    
    //#########################################################################
    //# Pipe_EXE_MEM 流水线寄存器
    //#########################################################################
    wire DM_w_MEM;
    wire [1:0] mux_wdata_MEM;
    wire [31:0] alu_MEM;
    wire [31:0] npc_MEM;
    wire [31:0] DM_wdata_MEM;
    
    Pipe_EXE_MEM Pipe_EXE_MEM(
        .clk(clk),
        .rst(rst),
    
        .DM_w_EXE(DM_w_EXE),
        .write_EXE(write_EXE),
        .waddr_EXE(waddr_EXE),
        .mux_wdata_EXE(mux_wdata_EXE),
        .npc_EXE(npc_EXE),
        .alu_EXE(alu_EXE),
        .DM_wdata_EXE(DM_wdata_EXE),
    
        .DM_w_MEM(DM_w_MEM),
        .write_MEM(write_MEM),
        .waddr_MEM(waddr_MEM),
        .mux_wdata_MEM(mux_wdata_MEM),
        .npc_MEM(npc_MEM),
        .alu_MEM(alu_MEM),
        .DM_wdata_MEM(DM_wdata_MEM)
    );
    
    //#########################################################################
    //# MEM 部分
    //#########################################################################
    wire DM_w;
    wire [31:0] DM_addr;
    wire [31:0] DM_rdata;
    wire [31:0] DM_wdata;
    assign DM_addr = alu_MEM;
    assign DM_wdata = DM_wdata_MEM;
    assign DM_w = DM_w_MEM;
    dmem dmem(~clk,rst,DM_w,DM_addr,DM_wdata,DM_rdata);
    
    //#########################################################################
    //# Pipe_MEM_WB 流水线寄存器
    //#########################################################################
    wire write_WB;
    wire [4:0] waddr_WB;
    wire [31:0] alu_WB;
    wire [31:0] DM_rdata_WB;
    wire [1:0] mux_wdata_WB;
    wire [31:0] npc_WB;
    
    Pipe_MEM_WB Pipe_MEM_WB(
        .clk(clk),
        .rst(rst),
        
        .write_MEM(write_MEM),
        .waddr_MEM(waddr_MEM),
        .mux_wdata_MEM(mux_wdata_MEM),
        .alu_MEM(alu_MEM),
        .npc_MEM(npc_MEM),
        .DM_rdata_MEM(DM_rdata),
    
        .write_WB(write_WB),
        .waddr_WB(waddr_WB),
        .mux_wdata_WB(mux_wdata_WB),
        .alu_WB(alu_WB),
        .npc_WB(npc_WB),
        .DM_rdata_WB(DM_rdata_WB)
    );
    
    //#########################################################################
    //# WB 部分
    //#########################################################################
    wire [31:0] wdata_WB;
    assign wdata_WB = (mux_wdata_WB[1]) ? npc_WB : (mux_wdata_WB[0]) ? DM_rdata_WB : alu_WB;
    regfile_board regfile_board(clk,rst,write_WB,rs,rt,waddr_WB,wdata_WB,rs_data,rt_data,reg_3,reg_4,reg_5,eggs,floor);

endmodule