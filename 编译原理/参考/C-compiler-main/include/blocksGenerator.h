#ifndef BLOCKGEN_H
#define BLOCKGEN_H
// 该模块主要包含了和基本块相关的内容
// 主要完成了基本块的划分和待用及活跃信息的分析
#include "grammarParse.h" // 这里需要用到前面的中间代码所以要include
// 基本块
struct BaseBlock
{
    string block_name;         // 块名
    int nxtb[2];               // 两个出口
    vector<_Quaternary> codes; // 四元式代码
};

// 变量信息
class Info
{
public:
    int nxt;     // 下一次要用到的位置，即待用信息
    bool active; // 后续是否活跃

    Info(int nxt, bool active);
    Info(const Info &i);
    Info();
};

// 带活跃和待用信息的四元式
class InfoQuaternary
{
public:
    _Quaternary base_q;
    Info info[3]; // 分别是src1 src2 des的信息情况
    InfoQuaternary(_Quaternary base_q, Info i1, Info i2, Info i3);
    InfoQuaternary(_Quaternary base_q, Info i[]);
    InfoQuaternary(_Quaternary base_q);
};

// 带活跃和待用信息的Block
struct InfoBlock
{
    string block_name;                       // 块名
    int nxtb[2];                             // 两个出口
    vector<InfoQuaternary> codes;            // 这里带活跃和待用信息
    InfoBlock();                             // 默认构造
    InfoBlock(struct BaseBlock &base_block); // 用baseblock转化
};

// 储存两种类型的块
// 输入：中间代码
// 输出：info_fun_blocks inl_fun outl_fun
class BlocksGenerator
{
public:
    int block_cnt = 1;
    vector<_Quaternary> inter_codes;                                                          // 中间代码
    map<string, vector<BaseBlock>> base_fun_blocks;                                           // 基本函数块
    map<string, vector<InfoBlock>> info_fun_blocks;                                           // 带信息的函数块
    map<string, vector<set<string>>> fun_inls;                                                // 函数块入口活跃变量集
    map<string, vector<set<string>>> fun_outls;                                               // 函数块出口活跃变量集
    vector<pair<int, string>> fun_enters;                                                     // 函数入口
    BlocksGenerator(vector<_Quaternary> &inter_codes, vector<pair<int, string>> &fun_enters); // 构造函数
    void get_base_fun_blocks();                                                               // 获得基本块
    void get_info_fun_blocks();                                                               // 获得信息基本块
    string get_block_name();
};

#endif