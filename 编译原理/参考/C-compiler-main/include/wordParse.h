#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "common.h"
#define KEYWORD_NUMS 30    // 关键词数量
#define MO_NUMS 15         // 单目运算符数量
#define BO_NUMS 15         // 双目运算符数量
#define DELIMITER_NUMS 9   // 界符数量
#define STATE_NUMS 150     // 可能状态数
#define FINAL_STATE 'Y'    // 终结状态
#define STACK_SIGNAL '$'   // 栈底符号
#define EPSILON_SIGNAL '@' // epsilon
// 考虑到调用语法分析器要调用词法分析器的问题，所以进行封装

// 关键字 类型：string
const string keyWords[KEYWORD_NUMS] = {"break", "case", "char", "continue", "do", "default", "double",
                                       "else", "float", "for", "if", "int", "include", "long", "main", "return", "switch", "typedef", "void", "unsigned", "while", "iostream"};
// 双目运算符 类型: string
const string biOperator[BO_NUMS] = {"++", "--", "&&", "||", "<=", "!=", "==", ">=", "+=", "-=", "*=", "/="};
// 单目运算符 类型： char
const char moOperator[MO_NUMS] = {'+', '-', '*', '/', '!', '%', '~', '&', '|', '^', '=', '>', '<'};
// 界符 类型: char
const char delimiter[DELIMITER_NUMS] = {',', '(', ')', '{', '}', ';', '#', '$'};

struct NFAstateSet
{
    set<char> states;
};

// 将输出的词法分析结果作为二元组输出

class wordParse
{

private:
    FILE *fileSource = NULL;
    ifstream inputFile;
    ofstream outputFile;
    ofstream resultFile;
    // 定义顺序
    // 关键字：1
    // 标识符：2
    // 常数:   3
    // 运算符: 4
    // 界符:   5

    // 这里将词法分析结果映射为关键词
    map<string, WordType> parseMap;
    list<Word> parseResult;
    set<string> keyWordsSet;
    set<string> biOperatorSet;
    set<char> delimiterSet;
    set<char> moOperatorSet;
    set<char> stateSet;
    set<char> finalStateSet;
    char DFAStartState; // 初始态
    bool DFAIsFinal[STATE_NUMS];
    NFAstateSet stateTransfer[STATE_NUMS][STATE_NUMS];
    NFAstateSet DFAStateSet[STATE_NUMS];
    int DFAStateSetNums = 0;
    int dfaTransfer[STATE_NUMS][STATE_NUMS];

public:
    void parseMapInit();
    void initSet();
    bool isLetter(char x);
    bool isKeyWord(string str);
    bool isBiOperator(string str);
    bool isMoOperator(char x);
    bool isDelimiter(char x);
    bool isState(char x);
    bool isFinalState(char x);
    bool isExist(char x, NFAstateSet now);
    void NFABuild(string grammarFileName);
    int isExsitState(NFAstateSet now);
    void getExpandClosure(NFAstateSet &now);
    bool isFinalState(NFAstateSet now);
    void NFAConvert();
    bool judge(string str);
    bool judgeConst(string num);
    void scanFile();
    void work(string file_name);
    void outputParseResult(string file_name);
    list<Word> &getWordParseResult();
};