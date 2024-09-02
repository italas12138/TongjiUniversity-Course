
#include "wordParse.h"

/************


一些初始化操作


***********/

// 初始化该映射
void wordParse::parseMapInit()
{
    // 对于可能出现的符号直接写死

    parseMap["int"] = INT;
    parseMap["void"] = VOID;
    parseMap["return"] = RETURN;
    parseMap["if"] = IF;
    parseMap["main"] = ID;
    parseMap["while"] = WHILE;
    parseMap["else"] = ELSE;
    parseMap[";"] = SEMICO;
    parseMap[","] = COMMA;
    parseMap["$"] = ENDFILE;
    // 运算符
    parseMap["+"] = ADD;
    parseMap["-"] = MINUS;
    parseMap["*"] = MULT;
    parseMap["/"] = DIV;
    parseMap["="] = ASSIGN;
    // 比较符
    parseMap[">"] = GT;
    parseMap["<"] = LT;
    parseMap[">="] = GTE;
    parseMap["<="] = LTE;
    parseMap["=="] = EQU;

    // 括号
    parseMap["{"] = LBRACE;
    parseMap["}"] = RBRACE;
    parseMap["("] = LPAREN;
    parseMap[")"] = RPAREN;
}

// 由于后续要用到查询 比较等操作，所以直接用集合存储比较方便
void wordParse::initSet()
{
    // 初始化关键词集合
    for (int i = 0; i < KEYWORD_NUMS; i++)
    {
        keyWordsSet.insert(keyWords[i]);
    }
    // 初始化单目运算符集合
    for (int i = 0; i < MO_NUMS; i++)
    {
        moOperatorSet.insert(moOperator[i]);
    }
    // 初始化双目运算符集合
    for (int i = 0; i < BO_NUMS; i++)
    {
        biOperatorSet.insert(biOperator[i]);
    }
    // 初始化界符集合
    for (int i = 0; i < DELIMITER_NUMS; i++)
    {
        delimiterSet.insert(delimiter[i]);
    }
}

// 用于判断某个字符是否属于合法的字母
bool wordParse::isLetter(char x)
{
    return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z');
}

// 用于判断输入字符是否为关键词
// 这里选择直接用set存储就行了

bool wordParse::isKeyWord(string str)
{
    return keyWordsSet.count(str) != 0;
}

bool wordParse::isBiOperator(string str)
{
    return biOperatorSet.count(str) != 0;
}

bool wordParse::isMoOperator(char x)
{
    return moOperatorSet.count(x) != 0;
}

bool wordParse::isDelimiter(char x)
{
    return delimiterSet.count(x) != 0;
}

bool wordParse::isState(char x)
{
    return stateSet.count(x) != 0;
}

bool wordParse::isFinalState(char x)
{
    return finalStateSet.count(x) != 0;
}

bool wordParse::isExist(char x, NFAstateSet now)
{
    return now.states.count(x) != 0;
}
void wordParse::NFABuild(string grammarFileName)
{
    int proNums;    // 产生式数量
    char left;      // 产生式左部
    string right;   // 产生式右部
    char separator; //->分隔符
    ifstream in;
    // 用流对象读入
    in.open(grammarFileName);
    in >> proNums; // 读入产生式数量

    for (int i = 0; i < proNums; i++)
    {
        in >> left >> separator >> separator >> right;
        if (i == 0)
        {
            DFAStartState = left;
        }
        if (!isState(left))
            stateSet.insert(left);
        if (!isFinalState(right[0]))
            finalStateSet.insert(right[0]);
        if (right.length() > 1)
        {
            stateTransfer[left][right[0]].states.insert(right[1]);
        }
        else
        {
            stateTransfer[left][right[0]].states.insert(FINAL_STATE);
        }
    }
}

// 判断和已有的newset是否存在重复
int wordParse::isExsitState(NFAstateSet now)
{
    //
    for (int i = 0; i < DFAStateSetNums; i++)
    {
        if (now.states == DFAStateSet[i].states)
            return i;
    }
    return -1;
}

//

void wordParse::getExpandClosure(NFAstateSet &now)
{

    // 这里需要计算到闭包，考虑到闭包是个不断扩充的过程，所以这里需要转换
    char tmpSet[STATE_NUMS];
    int cnt = 0;
    for (auto t : now.states)
    {
        tmpSet[cnt++] = t;
    }
    // 这里主要是由于要一直闭包，所以才这样
    for (int i = 0; i < cnt; i++)
    {
        for (auto t : stateTransfer[tmpSet[i]][EPSILON_SIGNAL].states)
        {
            if (!isExist(t, now))
            {
                tmpSet[cnt++] = t;
                now.states.insert(t);
            }
        }
    }
}

// 判断是否是终态
bool wordParse::isFinalState(NFAstateSet now)
{
    return now.states.count(FINAL_STATE) != 0;
}

void wordParse::NFAConvert()
{
    DFAStateSetNums = 0;
    NFAstateSet nowStateSet;
    NFAstateSet nextStateSet;
    nowStateSet.states.insert(DFAStartState);
    stack<NFAstateSet> s;
    getExpandClosure(nowStateSet);
    s.push(nowStateSet);
    DFAStateSet[DFAStateSetNums++] = nowStateSet;
    memset(dfaTransfer, -1, sizeof dfaTransfer);
    for (int i = 0; i < 150; ++i)
        DFAIsFinal[i] = false;
    if (isFinalState(nowStateSet))
        DFAIsFinal[DFAStateSetNums - 1] = true;

    // 开始扩充
    while (!s.empty())
    {
        nowStateSet = s.top();
        s.pop();
        for (auto finalSignal : finalStateSet)
        {
            for (auto NTSignal : nowStateSet.states)
            {
                for (auto nextState : stateTransfer[NTSignal][finalSignal].states)
                {
                    if (!isExist(nextState, nextStateSet))
                        nextStateSet.states.insert(nextState);
                }
            }
            getExpandClosure(nextStateSet);
            if (nextStateSet.states.size() > 0 && isExsitState(nextStateSet) == -1)
            {
                s.push(nextStateSet);
                DFAStateSet[DFAStateSetNums++] = nextStateSet;
                if (isFinalState(nextStateSet))
                    DFAIsFinal[DFAStateSetNums - 1] = true;
            }
            if (nextStateSet.states.size() > 0 && isExsitState(nextStateSet) != -1 && finalSignal != EPSILON_SIGNAL)
            {
                dfaTransfer[isExsitState(nowStateSet)][finalSignal] = isExsitState(nextStateSet);
            }
            nextStateSet.states.clear();
        }
    }
}

// 判断是否DFA可以走到终态
bool wordParse::judge(string str)
{
    char curState = 0;
    for (int i = 0; i < str.length(); i++)
    {
        curState = dfaTransfer[curState][str[i]];
        if (curState == -1)
            return false;
    }
    if (DFAIsFinal[curState] == true)
        return true;
    return false;
}

bool wordParse::judgeConst(string num)
{
    // 统计一下有多少个点和数字即可
    bool flag = 1;
    int comma_cnt = 0;
    for (auto &s : num)
    {
        if (!(s >= '0' && s <= '9' || s == '.'))
        {
            flag = 0;
            break;
        }
        if (s == '.')
            comma_cnt++;
    }
    if (!flag || comma_cnt >= 2)
        return false;
    else
        return true;
}

string charToString(char ch)
{
    char t[10];
    t[0] = ch;
    t[1] = '\0';
    return t;
}

void wordParse::scanFile()
{
    char info[100];
    char ch;
    int ptr;
    int keyFlag;
    ch = fgetc(fileSource);
    bool overFlag = false;
    while (!overFlag)
    {
        keyFlag = -1;
        ptr = 0;
        if (isdigit(ch)) // 多一个ch
        {
            keyFlag = 1;
            info[ptr++] = ch;
            ch = fgetc(fileSource);
            while (isLetter(ch) || isdigit(ch) || ch == '.' || ch == '+' || ch == '-')
            {
                keyFlag = 1;
                info[ptr++] = ch;
                ch = fgetc(fileSource);
            }
            info[ptr] = '\0';
        }
        if (keyFlag == 1)
        {
            if (judgeConst(info))
            {
                // NUM
                parseResult.push_back({info, NUM});
                // outputFile << 3;
            }
            else
            {
                cout << info << " "
                     << "出错，不是常量" << endl;
            }
            ptr = 0;
            keyFlag = -1;
        }
        if (isLetter(ch))
        {
            keyFlag = 2;
            info[ptr++] = ch;
            ch = fgetc(fileSource);
            while (isLetter(ch) || isdigit(ch))
            {
                keyFlag = 2;
                info[ptr++] = ch;
                ch = fgetc(fileSource);
            }
            info[ptr] = '\0';
        }
        if (keyFlag == 2)
        {
            if (isKeyWord(info))
            {
                parseResult.push_back({info, parseMap[info]});
            }
            else
            {
                if (judge(info))
                {
                    parseResult.push_back({info, ID});
                }
                else
                {
                    cout << info << " "
                         << "出错，不是标识符" << endl;
                }
            }
            ptr = 0;
            keyFlag = -1;
        }
        if (isDelimiter(ch))
        {
            string tmp = charToString(ch);
            parseResult.push_back({tmp, parseMap[tmp]});
            if ((ch = fgetc(fileSource)) == EOF)
            {
                overFlag = true;
                break;
            }
        }
        if (isMoOperator(ch))
        {
            info[ptr++] = ch;
            if ((ch = fgetc(fileSource)) == EOF)
            {
                overFlag = true;
            }
            info[ptr++] = ch;
            info[ptr] = '\0';
            if (overFlag == false && isBiOperator(info))
            {
                parseResult.push_back({info, parseMap[info]});
                ch = fgetc(fileSource);
            }
            else
            {
                string tmp = charToString(info[0]);
                parseResult.push_back({tmp, parseMap[tmp]});
            }
            ptr = 0;
        }
        if (ch == '\n')
        {
            // ch = fgetc(fileSource);
            parseResult.push_back({"nextLine", NEXTLINE});
        }
        if (ch == ' ' || ch == '\n' || ch == '\t')
        {
            if ((ch = fgetc(fileSource)) == EOF)
            {
                overFlag = true;
                break;
            }
            continue;
        }
    }
}

void wordParse::outputParseResult(string file_name)
{
    // 将获得的列表输出打印到文件中
    ofstream out;
    out.open(file_name);
    if (!out.is_open())
    {
        cout << "文件" << file_name << "打开失败！"
             << "\n";
        return;
    }

    for (auto &e : parseResult)
    {
        out << e.first << " " << WordTypeName[e.second] << "\n";
    }
    out.close(); // 关闭文件
}
list<Word> &wordParse::getWordParseResult()
{
    return parseResult;
}
void wordParse::work(string file_name)
{
    parseMapInit();
    initSet();
    NFABuild("WordParseGrammar.txt");
    NFAConvert();
    fileSource = fopen(file_name.c_str(), "r+");
    // outputFile.open("output.txt");
    // resultFile.open("ParseAnalyseResult.txt");
    scanFile();
    fclose(fileSource);
    // outputFile.close();
}