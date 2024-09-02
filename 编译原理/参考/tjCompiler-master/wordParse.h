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
const string keyWords[KEYWORD_NUMS] = { "break", "case", "char", "continue", "do", "default", "double",
                                       "else", "float", "for", "if", "int", "include", "long", "main", "return", "switch", "typedef", "void", "unsigned", "while", "iostream" };

const string biOperator[BO_NUMS] = { "++", "--", "&&", "||", "<=", "!=", "==", ">=", "+=", "-=", "*=", "/=" };

const char moOperator[MO_NUMS] = { '+', '-', '*', '/', '!', '%', '~', '&', '|', '^', '=', '>', '<' };
// 界符 类型: char
const char delimiter[DELIMITER_NUMS] = { ',', '(', ')', '{', '}', ';', '#', '$' };

struct NFAstateSet
{
    multiset<char> states;
};

// 将输出的词法分析结果作为二元组输出

class wordParse
{

private:
    FILE* fileSource = NULL;
    ifstream inputFile;
    ofstream outputFile;
    ofstream resultFile;
  
    // 定义顺序
    // 关键字：1
    // 标识符：2
    // 常数:   3
    // 运算符: 4
    // 界符:   5
    string testFileName;            //测试程序名称
    map<string, WordType> parseMap; // 字符串到词类型的映射
    list<Word> parseResult;         // 所有<值，属性>的键值对序列，其中Word定义为pair<string, WordType>
    set<string> keyWordsSet;        // 关键字集合
    set<string> biOperatorSet;      // 运算符（多符号运算符，如>=,!=,==等）集合
    set<char> moOperatorSet;        // 运算符（单符号运算符，如=,>,<等）集合
    set<char> delimiterSet;         // 界符结合
    set<char> stateSet;             // DFA状态集合
    set<char> finalStateSet;        // DFA终态集合
    char DFAStartState;             // 初始态
    bool DFAIsFinal[STATE_NUMS];    // DFAIsFinal[i]=1表示第i个状态是终态

    /*NFA的状态转移，stateTransfer[A][a]表示A状态在接受a字符后可能达到的状态
      NFAstateSet定义为：
      struct NFAstateSet
    {
        multiset<char> states;
    };
    states存放所有可能到达的状态*/
    NFAstateSet stateTransfer[STATE_NUMS][STATE_NUMS];
    NFAstateSet DFAStateSet[STATE_NUMS];    //存储由NFA转化成的DFA的状态
    int DFAStateSetNums = 0;                //DFA状态数量
    int dfaTransfer[STATE_NUMS][STATE_NUMS];   //DFA的状态转移，dfaTransfer[i][j]表示第i个状态接受字符j到达的状态

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
    void getExpandClosure(NFAstateSet& now);
    bool isFinalState(NFAstateSet now);
    void NFAConvert();
    bool judge(string str);
    bool judgeConst(string num);
    void scanFile();
    void work();
    void outputParseResult(string file_name);
    list<Word>& getWordParseResult();
};