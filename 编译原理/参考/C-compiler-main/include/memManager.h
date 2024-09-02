#ifndef MEMMANAGER_H
#define MEMMANAGER_H
// 模块主要功能为管理寄存器和内存相关的内容
#include "common.h"
#include "blocksGenerator.h"
class MemManager
{
public:
    map<string, set<string>> R_Value; // R_Value表
    map<string, set<string>> A_Value; // A_Value表
    map<string, int> var_mem_pos;     // 变量存储在内存的位置表
    int top_ptr;                      // top栈帧
    list<string> free_regs;           // 空闲寄存器表
    MemManager();
    string get_free_reg(string &now_fun, map<string, vector<set<string>>> &fun_outls, map<string, vector<InfoBlock>> &info_fun_blocks, vector<InfoQuaternary>::iterator &cur_quaternary, vector<InfoBlock>::iterator &cur_info_block, vector<string> &object_codes);                // 获得一个空闲寄存器
    string get_reg_for_src(string &now_fun, map<string, vector<set<string>>> &fun_outls, map<string, vector<InfoBlock>> &info_fun_blocks, vector<InfoQuaternary>::iterator &cur_quaternary, vector<InfoBlock>::iterator &cur_info_block, vector<string> &object_codes, string src); // 这里需要传入变量是因为里面会有加载src到寄存器内的指令
    string get_reg_for_des(string &now_fun, map<string, vector<set<string>>> &fun_outls, map<string, vector<InfoBlock>> &info_fun_blocks, vector<InfoQuaternary>::iterator &cur_quaternary, vector<InfoBlock>::iterator &cur_info_block, vector<string> &object_codes);             // 为目标变量获取空闲寄存器
    void store_var(vector<string> &object_codes, string reg, string var);                                                                                                                                                                                                           // 把寄存器内的数据存入到内存中
    void store_outl_var(vector<string> &object_codes, set<string> &outls);                                                                                                                                                                                                          // 存储出口活跃变量到内存中
    void clear_var_reg(string var);                                                                                                                                                                                                                                                 // 清空变量所在寄存器
    void reset_mem(set<string> vars);                                                                                                                                                                                                                                               // 重置寄存器和内存信息
};

#endif