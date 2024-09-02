#pragma once
#include "common.h"

const int FUNCTION_SIZE = 2e9;
/*基本块*/
struct Block
{
    string name;                    //名称
    vector<_Quaternary> quaters;    //四元式
    int next1;                      //出口一
    int next2;                      //出口二
};

/*带有待用信息/活跃信息的基本块*/
struct ActiveInfo
{
    int next { -1 };                            //待用四元式的编号，初始为-1，表示非待用
    bool active { 0 };                          //是否活跃，初始化为否
    ActiveInfo(){}
    ActiveInfo(int next, bool active){
        this->next = next;
        this->active = active;
    }
    ActiveInfo(const ActiveInfo& activeInfo){
        this->next = activeInfo.next;
        this->active = activeInfo.active;
    }
};
struct QuaternaryWithInfo
{
    _Quaternary q;                              //四元式代码
    ActiveInfo lop;                             //左操作数活跃信息
    ActiveInfo rop;                             //右操作数活跃信息
    ActiveInfo des;                             //目的操作数活跃信息
    QuaternaryWithInfo(){}
    QuaternaryWithInfo(_Quaternary q){
        this->q = q;
    }
    friend ostream& operator<<(ostream& out, QuaternaryWithInfo qWithInfo){
        out << "    ";
        out << qWithInfo.q;
        out << " 左操作数：(" << (qWithInfo.lop.next == -1? "^" : to_string(qWithInfo.lop.next)) << "," << (qWithInfo.lop.active == 1? 'y' : '^') << ") "
               " 右操作数：(" << (qWithInfo.rop.next == -1? "^" : to_string(qWithInfo.rop.next)) << "," << (qWithInfo.rop.active == 1? 'y' : '^') << ") "
               " 目的操作数:(" << (qWithInfo.des.next == -1? "^" : to_string(qWithInfo.des.next)) << "," << (qWithInfo.des.active == 1? 'y' : '^') << ") ";
        return out;
    }
};

//带有四元式及变量活跃信息的基本块
struct BlockWithInfo
{
    string name;
    vector<QuaternaryWithInfo> quatersWithInfo;
    int next1;
    int next2;
    BlockWithInfo(Block block){
        this->name = block.name;
        this->next1 = block.next1;
        this->next2 = block.next2;
        quatersWithInfo.resize(block.quaters.size());
    }
};


class ObjectCodeGen
{
private:
    _Label label;                                           //基本块名称
    vector<string> objectCodes;                             //目标代码
    map<string, vector<Block>> funcBlocks;                  //函数基本块
    map<string, vector<BlockWithInfo>> funcBlocksWithInfo;  //带有待用/活跃信息的函数基本块
    map<string, list<ActiveInfo>> activeInfos;              //待用/活跃信息表,用{函数名+变量名，活跃信息列表} 作为映射
    map<string, vector<set<string>>> outStillActive;        //各函数出了基本块仍然活跃的变量
    set<string> inActive;                                   //当前块的入口活跃变量
    map<string, int> offsets;                               //变量偏移位置，用于生成汇编代码变量在内存中存储时的偏移量
    map<string, set<string>> RVALUE;                        //某个寄存器存放了哪些变量
    map<string, set<string>> AVALUE;                        //某个变量存放的位置（寄存器名）
    string curFuncName;                                     //当前分析的函数
    vector<BlockWithInfo>::iterator curIBlock;              //当前分析的基本块
    vector<QuaternaryWithInfo>::iterator curQuater;         //当前分析的四元式 
    list<string> freeRegs;                                  //空闲寄存器
    int top;                                                //栈顶指针
    map<string,bool> flag;                                  //递归控制标识
public:
    void divideBlock(vector<FuncEnterTable>* funcEnterTable, vector<_Quaternary> interMediateCode);
    void handleBlock();
    bool isVar(string symbol);
    bool getActive(Block startBlock, string var, vector<Block> blocks);
    bool getActiveInfo(Block block, string var, vector<Block> blocks);
    void outputFuncBlock(const string fileName);
    void outputFuncBlockWithInfo(const string fileName);
    void gen();
    void genForFuncBlock(int curBlockIdx);
    void storeActiveVar(set<string> outActiveVars);
    void getInActiveVar(int funcBlockIndx, vector<BlockWithInfo> blocks);
    bool isControl(string op);
    void genForQuaternary(vector<QuaternaryWithInfo>::iterator iter, int& arg_num, int& par_num, list<pair<string,bool>>& par_list);
    string getReg(string var);
    string getReg();
    void storeVar(string reg, string var);
    void releaseReg(string var);
    string getRegForRd();
    void outputObjectCode(string fileName);
    void outputOutActiveVar(string fileName);
};
