#ifndef COMMON_H
#define COMMON_H
#include <bits/stdc++.h>
using namespace std;
// 词法类型
typedef enum
{
    LCOMMENT, // 行注释
    SCOMMENT, // 段注释
    NEXTLINE, // 空白行
    ENDFILE,  // 文件结束
    ERROR,    // 错误码
    ID,
    NUM,
    // 关键字
    INT,
    VOID,
    RETURN,
    WHILE,
    ELSE,
    // 界符
    SEMICO,
    COMMA,
    // 运算符
    ADD,
    MINUS,
    MULT,
    DIV,
    ASSIGN,
    // 比较符
    GT,
    LT,
    GTE,
    LTE,
    EQU,
    // 括号
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    IF
} WordType;
const string WordTypeName[] = {
    "LCOMMENT", // 行注释
    "SCOMMENT", // 段注释
    "NEXTLINE", // 空白行
    "ENDFILE",  // 文件结束
    "ERROR",    // 错误码
    "ID",
    "NUM",
    // 关键字
    "INT",
    "VOID",
    "RETURN",
    "WHILE",
    "ELSE",
    // 界符
    "SEMICO",
    "COMMA",
    // 运算符
    "ADD",
    "MINUS",
    "MULT",
    "DIV",
    "ASSIGN",
    // 比较符
    "GT",
    "LT",
    "GTE",
    "LTE",
    "EQU",
    // 括号
    "LBRACE",
    "RBRACE",
    "LPAREN",
    "RPAREN",
    "IF" };

typedef pair<string, WordType> Word;     //{内容，种类}
typedef pair<int,string> FuncEnterTable; //{函数入口，函数名称}

// 四元组
struct _Quaternary
{
    string _opt; // 操作符
    string _rs;  // rs 操作数
    string _rt;  // rt 操作数
    string _rd;  // rd 操作数
    _Quaternary& operator=(_Quaternary q){
        this->_opt = q._opt;
        this->_rs = q._rs;
        this->_rt = q._rt;
        this->_rd = q._rd;
        return *this;
    }
    friend ostream& operator<<(ostream& out, _Quaternary q){
        out << "(" << q._opt << "," << q._rs << "," << q._rt << "," << q._rd << ")";
        return out;
    }
    friend bool operator!=(_Quaternary q1, _Quaternary q2){
        return !(q1._opt == q2._opt && q1._rs == q2._rs && q1._rt == q2._rt && q1._rd == q2._rd);
    }
};

// 基本块
struct _BaseBlock
{
    string _blockName;            // 块名
    int _nextBlockA;              // 下一块连接块
    int _nextBlockB;              // 下一块连接块
    vector<_Quaternary> _quaCode; // 四元式代码
};

// 四元式或中间变量的label
class _Label
{
public:
    int _cnt; // 计数器
    _Label();
    string _MakeLabel();
};


void _errorLog(string _info);


#endif // !COMMON_H
