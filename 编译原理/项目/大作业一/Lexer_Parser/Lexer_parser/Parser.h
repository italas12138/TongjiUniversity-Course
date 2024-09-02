#ifndef PARSER_H
#define PARSER_H


#include "unit_type.h"
#include "Lexer.h"

#include <vector>
#include <set>
#include <map>
#include <list>
#include <fstream>

using namespace std;




//文法单元，left->right 左边有一个，右边有若干
struct Grammar
{
    Unit_type left;
    vector<Unit_type> right;
};






struct TNode	//树结点
{
    Unit_type tag;	//tag值
    int p;		//指针
    list<int> childs;	//孩子结点集
};

struct PTree	//语法树
{
    vector<TNode> TNode_List;	//结点集合
    int RootNode = -1;				//根结点指针
};

enum class Action		//LR1分析时的动作枚举值
{
    shift_in,	//移进
    reduction,	//归约
    accept,		//接受
    //由于使用map存储动作,故出错状态不再存储
};

struct Movement		//LR分析表中的一项动作
{
    Action action;
    int go;		//当action为s时,表示转到的状态;当action为r时,表示使用第几个产生式归约
};








struct LR1_process
{
    int Grammar_num;
    int point_addr;

    set<Unit_type> follow;

    bool operator==(const LR1_process& gp) const {
        //重载运算符，方便项目间比较
        return this->Grammar_num == gp.Grammar_num && this->point_addr == gp.point_addr && this->follow == gp.follow;
    }

    bool operator<(const LR1_process& gp) const {
        //定义比较函数使得GrammarProject可以放入set或map中.注意必须使得排序固定
        if (this->Grammar_num < gp.Grammar_num)
            return true;
        else if (this->Grammar_num > gp.Grammar_num)
            return false;
        else if (this->point_addr < gp.point_addr)
            return true;
        else if (this->point_addr > gp.point_addr)
            return false;
        else if (this->follow.size() < gp.follow.size())
            return true;
        else if (this->follow.size() > gp.follow.size())
            return false;
        else {
            for (auto it1 = this->follow.cbegin(), it2 = gp.follow.cbegin();
                it1 != this->follow.cend();
                ++it1, ++it2) {
                if (*it1 < *it2)
                    return true;
                else if (*it1 > * it2)
                    return false;
            }
            return false;
        }
    }
};


class Parser_program
{
//private:
public:

    //读取文法产生式文件，生成文法表
    vector<Grammar> Grammar_list;

    //在LR(1)中需要生成first集和follow集
    //一个符号与若干符号对应，使用set数据结构可以防止重复
    map<Unit_type, set<Unit_type>> first_list;
    map<Unit_type, set<Unit_type>> follow_list;


    vector<set<LR1_process>> project_process_list;	//项目集
    map<int, map<Unit_type, Movement>> Action_goto;		//action表和goto表,存储在一起

    PTree pTree;



public:

    //语法分析中需要嵌入一个词法分析器
    Lexer_program lexer_program;

    Parser_program();
    ~Parser_program();

    void Init_First_list();

    int Get_grammar_byfile(string filename);
    void Get_grammar_bytext(string Text);


    void Parser_DFA(ostream& out);
    void Parser_PTree(ostream& out);

    //求CLOSURE集
    set<LR1_process> getClosure(const set<LR1_process>&);
    //查找相同的CLOSURE集
    int findSameProjectSet(const set<LR1_process>&);

    //获得Action_goto表
    int initAction_Goto();

    //语法分析
    int parser(const char*, word_unit&);
    //清除数据
    void clear_data();

};
































#endif // PARSER_H
