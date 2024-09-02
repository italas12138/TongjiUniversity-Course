vlib work
vlib activehdl

vlib activehdl/dist_mem_gen_v8_0_10
vlib activehdl/xil_defaultlib

vmap dist_mem_gen_v8_0_10 activehdl/dist_mem_gen_v8_0_10
vmap xil_defaultlib activehdl/xil_defaultlib

vlog -work dist_mem_gen_v8_0_10 -v2k5 \
"../../../ipstatic/dist_mem_gen_v8_0_10/simulation/dist_mem_gen_v8_0.v" \

vlog -work xil_defaultlib -v2k5 \
"../../../../CPU_MIPS_54.srcs/sources_1/ip/dist_mem_gen_0/sim/dist_mem_gen_0.v" \


vlog -work xil_defaultlib "glbl.v"

