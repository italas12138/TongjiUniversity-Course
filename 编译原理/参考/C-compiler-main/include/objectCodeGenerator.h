#ifndef OBJECTCODEGEN_H
#define OBJECTCODEGEN_H
#include "common.h"
#include "blocksGenerator.h"
#include "memManager.h"

class ObjectCodeGenerator
{
    // 目标代码生成类，这里只需要包含一个信息块类和一个内存管理类即可
public:
    BlocksGenerator block_generator;
    MemManager mem_manager;
    string now_fun;                                  // 当前函数名称
    vector<InfoQuaternary>::iterator cur_quaternary; // 当前四元式
    vector<InfoBlock>::iterator cur_info_block;      // 当前基本块
    vector<string> object_codes;                     // 目标代码
    ObjectCodeGenerator(vector<_Quaternary> &inter_codes, vector<pair<int, string>> &fun_enters);
    void generate_code();                                                                                                   // 一层循环
    void generate_code_for_fun(map<string, vector<InfoBlock>>::iterator fun_iter);                                          // 二层循环：基于函数块
    void generate_code_for_block(int block_idx);                                                                            // 三层循环：基于基本块
    void generate_code_for_quaternary(int block_idx, int &var_nums, int &paras_nums, list<pair<string, bool>> &paras_list); // 四层循环：基于四元式
    void check_valid(string &op, string &rs, string &rt);                                                                   // 判断变量是否合法
};

#endif