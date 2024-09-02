#ifndef GRAMMARPARSE_H
#define GRAMMARPARSE_H
#include "common.h"
const int _READBUFLEN = 1024;

// 枚举操作类型
enum __Operator
{
    shift,  // 移进
    reduct, // 规约
    acc,    // 接受
    error   // 报错
};

// 枚举声明语句的类型(变量声明和函数声明)
enum __DeclareType
{
    FUN,
    VAR
};

// 枚举数据类型
enum __DataType
{
    DINT,
    DVOID
};

class _Symbol
{
public:
    bool _isTs;      // 是否为终结符
    string _content; // 内容
    _Symbol();
    _Symbol(const string &_content, bool _isTs = false); // 构造函数
    friend bool operator<(const _Symbol &_s1, const _Symbol &_s2);
    friend bool operator==(const _Symbol &_s1, const _Symbol &_s2);
};

// ID和常量类继承

class _ID : public _Symbol
{
public:
    string _name;
    _ID(const _Symbol &_symbol, const string &_name);
};

class _NUM : public _Symbol
{
public:
    string _val;
    _NUM(const _Symbol &_symbol, const string &_val);
};

// 变量和函数

class _Var
{
public:
    string _varName;     // 变量名称
    __DataType _varType; // 变量类型
    int _varLevel;       // 变量层级
    friend bool operator==(const _Var &_v1, const _Var &_v2);
};

class _Fun
{
public:
    string _funName;           // 函数名称
    __DataType _returnType;    // 返回值类型
    list<__DataType> _parType; // 所有参数的类型
    int _enterPoint;           // 函数入口
    friend bool operator==(const _Fun &_f1, const _Fun &_f2);
};

// 拓展符号
// ===========================
class _FunctionDeclare : public _Symbol
{
public:
    list<__DataType> _parList;
    _FunctionDeclare(const _Symbol &_symbol);
};

class _Parameter : public _Symbol
{
public:
    list<__DataType> _parList;
    _Parameter(const _Symbol &_symbol);
};

class _ParameterList : public _Symbol
{
public:
    list<__DataType> _parList;
    _ParameterList(const _Symbol &_symbol);
};

class _SentenceBlock : public _Symbol
{
public:
    list<int> _nextList;
    _SentenceBlock(const _Symbol &_symbol);
};

class _SentenceList : public _Symbol
{
public:
    list<int> _nextList;
    _SentenceList(const _Symbol &_symbol);
};

class _Sentence : public _Symbol
{
public:
    list<int> _nextList;
    _Sentence(const _Symbol &_symbol);
};

class _ValueExpression : public _Symbol
{
public:
    string _expName;
    _ValueExpression(const _Symbol &_symbol);
};

class _FomalPar : public _Symbol
{
public:
    string _expName;
    _FomalPar(const _Symbol &_symbol);
};

class _Value : public _Symbol
{
public:
    string _expName;
    _Value(const _Symbol &_symbol);
};

class _ArgumentList : public _Symbol
{
public:
    list<string> _argList;
    _ArgumentList(const _Symbol &_symbol);
};

class _While : public _Symbol
{
public:
    list<int> _nextList;
    _While(const _Symbol &_symbol);
};

class _If : public _Symbol
{
public:
    list<int> _nextList;
    _If(const _Symbol &_symbol);
};

class _Expression : public _Symbol
{
public:
    string _expName;
    list<int> _falseList;
    _Expression(const _Symbol &_symbol);
};

class _M : public _Symbol
{
public:
    int _quad;
    _M(const _Symbol &_symbol);
};

class _N : public _Symbol
{
public:
    list<int> _nextList;
    _N(const _Symbol &_symbol);
};

// 扩展符号结束
// ===========================

class _TempVar
{
public:
    int _cnt; // 记录一下当前的下标
    _TempVar();
    string _MakeTemp();
};

class _Production
{
public:
    _Symbol _left;          // 产生式左部
    vector<_Symbol> _right; // 产生式右部
    int _idx;               // 标号
};

class _Program
{
public:
    int _pIdx;             // 对应的产生式标号
    int _dotPos;           // dot所处的位置
    set<_Symbol> _outLook; // 展望符集合
    _Program();
    _Program(int _pIdx, int _dotPos, const set<_Symbol> &_outLook);
    friend bool operator<(const _Program &_s1, const _Program &_s2);
    friend bool operator==(const _Program &_s1, const _Program &_s2);
};

class _ProgramSet
{
public:
    set<_Program> _program;
};

typedef pair<int, _Symbol> _GoTo; // 面临的状态

class _Action
{
public:
    __Operator _operator; // 操作
    int _nextState;       // 下一个状态
    _Action();
    _Action(__Operator _operator, int _nextState);
};

typedef map<_GoTo, _Action> _ActionGoTo;

class _Dfa
{
public:
    list<_ProgramSet> _stateSet; // 状态集
    map<_GoTo, int> _trans;      // 状态转移
};

class _Label
{
public:
    int _cnt; // 计数器
    _Label();
    string _MakeLabel();
};

// 四元组
struct _Quaternary
{
    string _opt; // 操作符
    string _rs;  // rs 操作数
    string _rt;  // rt 操作数
    string _rd;  // rd操作数
};

// 基本块
struct _BaseBlock
{
    string _blockName;            // 块名
    int _nextBlockA;              // 下一块连接块
    int _nextBlockB;              // 下一块连接块
    vector<_Quaternary> _quaCode; // 四元式代码
};

class _MediateCodeGen
{
public:
    _Label _label;                              // 下一个位置
    map<string, vector<_BaseBlock>> _funBlocks; // 函数块
    vector<_Quaternary> _quaCode;               // 四元式代码

    void _Emit(_Quaternary _quaternary);             // 将四元式插入到code中
    void _BackPatch(list<int> _nextList, int _quad); // 回填操作
    int _GetNextQuad();                              // 获取下一条语句的编号
};

class _GrammarParse
{
public:
    string _grammarFileName;                                                          // 文法文件名称
    vector<_Production> _productions;                                                 // 产生式
    map<_Symbol, set<_Symbol>> _firstSet;                                             // first集合
    _Dfa _dfa;                                                                        // DFA
    _ActionGoTo _actionGoTo;                                                          // LR1文法对应的actionGoTo表
    stack<_Symbol *> _symStack;                                                       // 符号栈
    stack<int> _staStack;                                                             // 状态栈
    _TempVar _tempVar;                                                                // 中间临时变量
    vector<_Var> _varTable;                                                           // 变量表
    vector<_Fun> _funTable;                                                           // 函数表
    _MediateCodeGen _code;                                                            // 中间代码
    vector<pair<int, string>> fun_enter;                                              // 函数入口
    _Var *_LookUpVar(string _varName);                                                // 查找变量
    _Fun *_LookUpFunc(string _funName);                                               // 查找函数
    int _nowLevel;                                                                    // 当前层级
    bool _FunParMatch(list<string> &_argumentList, list<__DataType> &_parameterList); // 判断函数参数是否匹配

    _Symbol *_PopStack();              // 出栈
    void _PushStack(_Symbol *_symbol); // 压栈

    _GrammarParse(const string &_grammarFileName);
    void _Work();                                      // 总体逻辑控制
    void _ReadProductions();                           // 读取产生式文法
    void _GetFirstSet();                               // 获得非终结符的First集
    void _BuildDfa();                                  // 创建DFA
    void _SemanticAnalyse(list<Word> &_lexicalResult); // 分析过程
    _ProgramSet _GetClosure(_Program &_program);       // 获得闭包
    bool _IsTs(const string &_symbol);                 // 判断是否为终结符
};
#endif