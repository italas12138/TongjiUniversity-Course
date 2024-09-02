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
    "IF"};

typedef pair<string, WordType>
    Word;
const int INF = 0x3f3f3f3f;
void _errorLog(string _info);
bool if_control(string op);       // 判断是否为return call  语句
bool if_no_var(string op);        // 判断是否为call j 语句
bool if_valid_var(string var);    // 判断是否为变量
bool if_valid_num(string num);    // 判断是否为数字
bool need_store_first(string op); // return call j 需要先store
#endif                            // !COMMON_H
