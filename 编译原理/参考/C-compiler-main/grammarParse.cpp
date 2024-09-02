#include "grammarParse.h"
list<int> merge(list<int> &_l1, list<int> &_l2)
{
    list<int> _ret;
    _ret.assign(_l1.begin(), _l1.end());
    _ret.splice(_ret.end(), _l2);
    return _ret;
}
_Symbol::_Symbol()
{
}
_Symbol::_Symbol(const string &_content, bool _isTs)
{
    this->_content = _content;
    this->_isTs = _isTs;
}

_ID::_ID(const _Symbol &_symbol, const string &_name) : _Symbol(_symbol)
{
    this->_name = _name;
}
_NUM::_NUM(const _Symbol &_symbol, const string &_val) : _Symbol(_symbol)
{
    this->_val = _val;
}
bool operator<(const _Symbol &_s1, const _Symbol &_s2)
{
    return _s1._content < _s2._content;
}
bool operator==(const _Symbol &_s1, const _Symbol &_s2)
{
    return _s1._content == _s2._content;
}

bool operator<(const _Program &_p1, const _Program &_p2)
{
    if ((pair<int, int>(_p1._pIdx, _p1._dotPos) == pair<int, int>(_p2._pIdx, _p2._dotPos)))
    {
        string _s1 = "";
        string _s2 = "";
        for (auto &_s : _p1._outLook)
            _s1 += _s._content;

        for (auto &_s : _p2._outLook)
            _s2 += _s._content;
        return _s1 < _s2;
    }

    return pair<int, int>(_p1._pIdx, _p1._dotPos) < pair<int, int>(_p2._pIdx, _p2._dotPos);
}
bool operator==(const _Program &_p1, const _Program &_p2)
{
    return (pair<int, int>(_p1._pIdx, _p1._dotPos) == pair<int, int>(_p2._pIdx, _p2._dotPos)) && (_p1._outLook == _p2._outLook);
}
_Action::_Action(__Operator _operator, int _nextState)
{
    this->_operator = _operator;
    this->_nextState = _nextState;
}
_Action::_Action()
{
}
_GrammarParse::_GrammarParse(const string &_grammarFileName)
{
    this->_grammarFileName = _grammarFileName;
}

_Program ::_Program(int _pIdx, int _dotPos, const set<_Symbol> &_outLook)
{
    this->_pIdx = _pIdx;
    this->_dotPos = _dotPos;
    this->_outLook = _outLook;
}
_Program ::_Program()
{
}

_FunctionDeclare ::_FunctionDeclare(const _Symbol &symbol) : _Symbol(symbol) {}

_Parameter::_Parameter(const _Symbol &symbol) : _Symbol(symbol) {}

_ParameterList::_ParameterList(const _Symbol &symbol) : _Symbol(symbol) {}

_Expression::_Expression(const _Symbol &symbol) : _Symbol(symbol) {}

_M::_M(const _Symbol &symbol) : _Symbol(symbol) {}

_N::_N(const _Symbol &symbol) : _Symbol(symbol) {}

_ValueExpression::_ValueExpression(const _Symbol &symbol) : _Symbol(symbol) {}

_FomalPar::_FomalPar(const _Symbol &symbol) : _Symbol(symbol) {}

_Value::_Value(const _Symbol &symbol) : _Symbol(symbol) {}

_ArgumentList::_ArgumentList(const _Symbol &symbol) : _Symbol(symbol) {}

_SentenceBlock::_SentenceBlock(const _Symbol &symbol) : _Symbol(symbol) {}

_SentenceList::_SentenceList(const _Symbol &symbol) : _Symbol(symbol) {}

_Sentence::_Sentence(const _Symbol &symbol) : _Symbol(symbol) {}

_While::_While(const _Symbol &symbol) : _Symbol(symbol) {}

_If::_If(const _Symbol &symbol) : _Symbol(symbol) {}

_TempVar::_TempVar()
{
    _cnt = 0;
}

string _TempVar::_MakeTemp()
{
    return string("T") + to_string(_cnt++);
}

_Label::_Label()
{
    _cnt = 1;
}

string _Label::_MakeLabel()
{
    return string("Label") + to_string(_cnt++);
}

void _MediateCodeGen::_Emit(_Quaternary _quaternary)
{
    _quaCode.push_back(_quaternary);
}

void _MediateCodeGen::_BackPatch(list<int> _nextList, int _quad)
{
    list<int>::iterator _iter;
    for (_iter = _nextList.begin(); _iter != _nextList.end(); _iter++)
    {
        auto _nowPos = *_iter;
        _quaCode[_nowPos]._rd = to_string(_quad);
    }
}

int _MediateCodeGen::_GetNextQuad()
{
    return _quaCode.size();
}

_Var *_GrammarParse::_LookUpVar(string _varName)
{
    vector<_Var>::iterator _iter;
    for (_iter = _varTable.begin(); _iter != _varTable.end(); _iter++)
    {
        auto _nowName = _iter->_varName;
        if (_nowName == _varName)
            return &(*_iter);
    }
    // 找不到
    return NULL;
}
bool operator==(const _Var &_v1, const _Var &_v2)
{
    return (_v1._varName == _v2._varName) && (_v1._varLevel == _v2._varLevel);
}
_Fun *_GrammarParse::_LookUpFunc(string _funName)
{
    vector<_Fun>::iterator _iter;
    for (_iter = _funTable.begin(); _iter != _funTable.end(); _iter++)
    {
        auto _nowName = _iter->_funName;
        if (_nowName == _funName)
            return &(*_iter);
    }
    // 找不到
    return NULL;
}
bool operator==(const _Fun &_f1, const _Fun &_f2)
{
    return (_f1._funName == _f2._funName) && (_f1._parType.size() == _f2._parType.size());
}
bool _GrammarParse::_FunParMatch(list<string> &_argumentList, list<__DataType> &_parameterList)
{
    // 这里由于参数类型只有int,所以直接判断参数的个数就可以了
    // 当需要拓展的时候可以考虑一一比对类型！
    return _argumentList.size() == _parameterList.size();
}

void _GrammarParse::_Work()
{
    _ReadProductions();
    _GetFirstSet();
    _BuildDfa();
}
void _GrammarParse::_ReadProductions()
{
    // 读入产生式，记下标号
    int _idx = 0; // 从0开始
    // 打开文件
    ifstream _fin;
    _fin.open(_grammarFileName);
    if (!_fin.is_open())
    {
        _errorLog("文件打开失败");
        exit(-1);
    }
    // 读入产生式，这里要注意空行的处理
    char _readBuf[_READBUFLEN];
    while (_fin >> _readBuf)
    {
        _Production _tmp;
        _tmp._left = _Symbol{string(_readBuf)};
        _tmp._idx = _idx++;
        // 读入右边 因为要小心空串的原因，所以读整行
        _fin >> _readBuf;
        if (!strcmp(_readBuf, "->"))
        {
        }
        _fin.getline(_readBuf, _READBUFLEN);
        // 这里利用stringstream 非常方便
        stringstream _tmpStream(_readBuf);
        while (_tmpStream >> _readBuf)
        {
            _tmp._right.push_back(_Symbol{_readBuf, _IsTs(_readBuf)});
        }
        _productions.push_back(_tmp); // 压入
    }

    // 打印观察产生式是否读取成功
    /*
    for (auto &e : _productions)
    {
        cout << "产生式编号" << e._idx << "\_n";
        cout << e._idx << " -> ";
        for (auto &_m : e._right)
        {
            cout << _m._content << '(' << _m._isTs << ')' << " ";
        }
        cout << "\_n";
    }*/
    /*
    //为每个非终结符建立一个first集
    vector<_Production>::iterator _productionIter;
    for (_productionIter = _productions.begin(); _productionIter != _productions.end(); _productionIter++)
    {
        auto _nowLeftSymbol = _productionIter->_left;
        if (_firstSet.count(_nowLeftSymbol) == 0)
            _firstSet[_nowLeftSymbol] = set<_Symbol>();
    }*/
}
void _GrammarParse::_GetFirstSet()
{
    // 对于每个非终结符求其first集

    // 设置一个终止变量
    bool _finishFlag = 0; // 默认值为0
    while (!_finishFlag)
    {
        _finishFlag = 1;
        // 遍历所有的产生式
        vector<_Production>::iterator _productionIter;
        for (_productionIter = _productions.begin(); _productionIter != _productions.end(); _productionIter++)
        {
            // 获得当前产生式ID，左边和右边符号
            auto _nowIdx = _productionIter->_idx;
            auto _leftSymbol = _productionIter->_left;
            auto _rightSymbol = _productionIter->_right;

            // 考虑遍历产生式右部
            vector<_Symbol>::iterator _rightSymbolIter;
            for (_rightSymbolIter = _rightSymbol.begin(); _rightSymbolIter != _rightSymbol.end(); _rightSymbolIter++)
            {
                // 判断一下是终结符还是非终结符
                auto _nowSymbol = *_rightSymbolIter;
                if (_nowSymbol._isTs)
                {
                    // 是终结符，直接加入

                    if (_firstSet[_leftSymbol].count(_nowSymbol) == 0)
                    {
                        _firstSet[_leftSymbol].insert(_nowSymbol);
                        _finishFlag = 0;
                    }
                    break;
                }
                else
                {
                    // 非终结符需要遍历该终结符的first集
                    // 再设置一个结束遍历
                    bool _finishTranverse = 1;
                    set<_Symbol>::iterator _nowSFS; // nowSymbolFirstSet
                    for (_nowSFS = _firstSet[_nowSymbol].begin(); _nowSFS != _firstSet[_nowSymbol].end(); _nowSFS++)
                    {
                        // 如果有epsilon才继续
                        auto _tmpSymbol = *_nowSFS;
                        if (_tmpSymbol._content == "epsilon")
                        {
                            _finishTranverse = 0;
                        }
                        else
                        {
                            if (_firstSet[_leftSymbol].count(_tmpSymbol) == 0)
                            {
                                _firstSet[_leftSymbol].insert(_tmpSymbol);
                                _finishFlag = 0;
                            }
                        }
                    }
                    if (_finishTranverse)
                        break;
                }
            }

            // 如果走到了产生式的尾部那么说明需要插入epsilon
            if (_rightSymbolIter == _rightSymbol.end())
            {
                _Symbol _epsilon("epsilon", 1);
                if (_firstSet[_leftSymbol].count(_epsilon) == 0)
                {
                    _firstSet[_leftSymbol].insert(_epsilon);
                    _finishFlag = 0;
                }
            }
        }
        // cout << _firstSet.size() << "\_n";
        // 打印观察first集是否正确
        /*
        for (auto &_m : _firstSet)
        {
            cout << "非终结符:" << _m.first._content << "\_n";
            for (auto &e : _m.second)
                cout << e._content << " ";
            cout << "\_n";
        }
        cout << "here"
             << "\_n";*/
    }
    /*
    for (auto &_m : _firstSet)
    {
        cout << "非终结符:" << _m.first._content << "\_n";
        for (auto &e : _m.second)
            cout << e._content << " ";
        cout << "\_n";
    }

    */
}

_ProgramSet _GrammarParse::_GetClosure(_Program &_program)
{
    // 这个过程中最重要的点后的符号
    auto _nowDotPos = _program._dotPos;
    auto _nowRightSymbol = _productions[_program._pIdx]._right;
    auto _nowRightLen = _productions[_program._pIdx]._right.size();

    _ProgramSet _retProgramSet;
    _retProgramSet._program.insert(_program); // 当前的先加入
    if (_nowDotPos == _nowRightLen)
    {
        // 规约项目，不需要再继续求闭包
        return _retProgramSet;
    }

    // 考虑后一个符号是什么
    auto _nextSymbol = _nowRightSymbol[_nowDotPos];
    if (_nextSymbol._isTs)
    {
        // 下一个是终结符也返回
        return _retProgramSet;
    }
    // 这里要考虑后面的first集，要先判断一下还有没有下一个符号的下一个符号
    set<_Symbol> _nextNextFirstSet;
    bool _hasEpsilon = false; // 是否有epsilon

    /*这里求first集应当是求后面整体的first集，也就是说如果遇到了epsilon还需要继续往下，那么就是一个遍历的过程*/

    bool _continueFlag = true;
    if (_nowDotPos == _nowRightLen - 1)
    {
        _hasEpsilon = 1;
    }
    else
    {
        for (auto _nowDotPosIter = _nowDotPos; _nowDotPosIter != _nowRightLen - 1 && _continueFlag; _nowDotPosIter++)
        {
            _continueFlag = false;

            auto _nextNextSymbol = _nowRightSymbol[_nowDotPosIter + 1];
            /*
            if (_nextNextSymbol._content == ",")
            {
                cout << "go there"
                     << "\_n";
            }*/
            // 加入下一个符号
            if (_nextNextSymbol._isTs)
            {
                _nextNextFirstSet.insert(_nextNextSymbol);
            }
            else
            {
                for (auto &_s : _firstSet[_nextNextSymbol])
                {
                    if (_s._content == "epsilon")
                    {
                        _continueFlag = true;
                        if (_nowDotPosIter == _nowRightLen - 2)
                        {
                            _nextNextFirstSet.insert(_s);
                            _hasEpsilon = 1;
                        }
                    }
                    else
                        _nextNextFirstSet.insert(_s);
                }
            }
        }
    }

    /*
    if (_nowDotPos != _nowRightLen - 1)
    {
        auto _nextNextSymbol = _nowRightSymbol[_nowDotPos + 1];
        // 加入下一个符号
        if (_nextNextSymbol._isTs)
        {
            _nextNextFirstSet.insert(_nextNextSymbol);
        }
        else
        {
            for (auto &_s : _firstSet[_nextNextSymbol])
            {
                _nextNextFirstSet.insert(_s);
                if (_s._content == "epsilon")
                    _hasEpsilon = true;
            }
        }
    }
    else
        _hasEpsilon = true; // 如果在尾部那说明也是添加和之前的展望符

        */
    if (_hasEpsilon)
    {
        for (auto &_s : _program._outLook)
        {
            _nextNextFirstSet.insert(_s);
        }
    }
    // 求好了展望符之后，下面遍历所有产生式
    vector<_Production>::iterator _productionIter;
    for (_productionIter = _productions.begin(); _productionIter != _productions.end(); _productionIter++)
    {
        // 获得当前产生式ID，左边和右边符号
        auto _nowIdx = _productionIter->_idx;
        auto _leftSymbol = _productionIter->_left;
        auto _rightSymbol = _productionIter->_right;
        if (_leftSymbol == _nextSymbol)
        {
            // 该产生式可用
            _Program _tmpProgram;
            _tmpProgram._dotPos = 0;
            _tmpProgram._pIdx = _nowIdx;
            _tmpProgram._outLook = _nextNextFirstSet;
            auto _tmpRetProgramSet = _GetClosure(_tmpProgram);
            for (auto &_tmpProgram : _tmpRetProgramSet._program)
            {
                _retProgramSet._program.insert(_tmpProgram);
            }
        }
    }
    return _retProgramSet;
}

void _GrammarParse::_BuildDfa()
{
    // 初始条件下产生式的第一条，这个时候展望符为$
    int _stateIdx = 0; // 状态标号
    list<_ProgramSet>::iterator _stateIter;
    _Program _firstPro;
    _firstPro._dotPos = 0;
    _firstPro._outLook.insert({"$", _IsTs("$")});
    _firstPro._pIdx = 0;
    _dfa._stateSet.push_back({_GetClosure(_firstPro)});
    // 将第一个产生式的闭包作为第一个状态

    // 开始遍历状态
    for (_stateIter = _dfa._stateSet.begin(); _stateIter != _dfa._stateSet.end(); _stateIter++, _stateIdx++)
    {
        // 遍历这个状态集中的所有项目
        auto _nowProgramSet = _stateIter->_program;
        set<_Program>::iterator _programIter;
        for (_programIter = _nowProgramSet.begin(); _programIter != _nowProgramSet.end(); _programIter++)
        {
            // 判断当前是否为归约项目
            auto _nowDotPos = _programIter->_dotPos;
            auto _nowProduction = _productions[_programIter->_pIdx];
            if (_nowDotPos == _nowProduction._right.size())
            {
                // 归约项目，那么需要归约所有的outlook
                auto _nowOutLook = _programIter->_outLook;
                for (auto &_s : _nowOutLook)
                {
                    // 这些符号可以进行归约
                    _Action _action({reduct, _programIter->_pIdx});
                    if (_programIter->_pIdx == 0)
                    {
                        _action._operator = acc;
                    }
                    if (_actionGoTo.count(_GoTo{_stateIdx, _s}) != 0)
                    {
                        _errorLog("文法不是LR1文法!");
                        exit(-1);
                    }
                    _actionGoTo[_GoTo{_stateIdx, _s}] = _action;
                }
                continue;
            }
            else
            {
                // 移进
                auto _nextSymbol = _nowProduction._right[_nowDotPos];
                // 如果已经处理过了这个符号，那么便不再处理
                if (_actionGoTo.count(_GoTo{_stateIdx, _nextSymbol}) != 0)
                    continue;

                // 当前产生式的闭包
                _Program _nextPro(_programIter->_pIdx, _programIter->_dotPos + 1, _programIter->_outLook);
                auto _nextProSet = _GetClosure(_nextPro);

                // 接下来要遍历下面的产生式是否也可以移动该符号

                set<_Program>::iterator _otherProgramIter = _programIter;
                _otherProgramIter++; // 下移动一个
                for (; _otherProgramIter != _nowProgramSet.end(); _otherProgramIter++)
                {
                    auto _nowDotPos = _otherProgramIter->_dotPos;
                    auto _nowProduction = _productions[_otherProgramIter->_pIdx];
                    if (_nowDotPos != _nowProduction._right.size() && _nowProduction._right[_nowDotPos] == _nextSymbol)
                    {
                        // 这种情况说明应当可以
                        _Program _nextPro(_otherProgramIter->_pIdx, _otherProgramIter->_dotPos + 1, _otherProgramIter->_outLook);
                        auto _tmpNextProSet = _GetClosure(_nextPro);
                        for (auto &_s : _tmpNextProSet._program)
                        {
                            _nextProSet._program.insert(_s);
                        }
                    }
                }

                // 这里需要考虑新得到的状态集是否是已经得到过的

                bool _hasThisState = 0;

                int _curStateIdx = 0;
                _GoTo _goTo(_stateIdx, _nextSymbol);
                list<_ProgramSet>::iterator _curStateIter;
                for (_curStateIter = _dfa._stateSet.begin(); _curStateIter != _dfa._stateSet.end(); _curStateIter++, _curStateIdx++)
                {
                    if (_curStateIter->_program == _nextProSet._program)
                    {
                        // 说明已经存在了
                        _dfa._trans[_goTo] = _curStateIdx;
                        if (_actionGoTo.count(_goTo) != 0)
                        {
                            _errorLog("LR1发生冲突!");
                            exit(-1);
                        }
                        _Action _action({shift, _curStateIdx});
                        _actionGoTo[_goTo] = _action;
                        _hasThisState = 1;
                        break;
                    }
                }

                if (!_hasThisState)
                {
                    // 新增一个状态
                    _dfa._stateSet.push_back(_nextProSet);
                    _dfa._trans[_goTo] = _dfa._stateSet.size() - 1;
                    if (_actionGoTo.count(_goTo) != 0)
                    {
                        _errorLog("LR1发生冲突!");
                        exit(-1);
                    }
                    _Action _action({shift, _dfa._trans[_goTo]});
                    _actionGoTo[_goTo] = _action;
                }
            }
        }
    }
    // 打印一下dfa 以及action_goto表

    // 打印dfa
    /*
    cout << "dfa状态数为: " << _dfa._stateSet.size() << "\_n";
    int cnt = 0;
    for (auto &e : _dfa._stateSet)
    {
        cout << "DFA:" << cnt++ << "\_n";
        for (auto &_m : e._program)
        {
            cout << "pIdx: " << _m._pIdx << " dotPos: " << _m._dotPos << "\_n";
            cout << _productions[_m._pIdx]._left._content << "->";
            for (auto &mm : _productions[_m._pIdx]._right)
                cout << mm._content << " ";
            cout << "\_n";
            cout << "outLooks:"
                 << "\_n";
            for (auto &_n : _m._outLook)
            {
                cout << _n._content << " ";
            }
            cout << "\_n";
        }
    }
    */
    /*

    // 打印action_goto
    cout << _actionGoTo.size() << "\_n";
    for (auto &e : _actionGoTo)
    {
        auto _goTo = e.first;
        auto _action = e.second;
        cout << "state: " << _goTo.first << " _Symbol: " << _goTo.second._content << "\_n";
        cout << "action: " << _action._operator << " nextState: " << _action._nextState << "\_n";
    }*/
}
_Symbol *_GrammarParse::_PopStack()
{
    auto _ret = _symStack.top();
    _symStack.pop();
    _staStack.pop();
    return _ret;
}
void _GrammarParse::_PushStack(_Symbol *_symbol)
{
    // 这里还需要压入一个状态
    _symStack.push(_symbol);
    // 判断一下转移是否存在
    if (_actionGoTo.count(_GoTo{_staStack.top(), *_symbol}) == 0)
    {
        _errorLog("归约错误,出现了不期待的符号！");
        exit(-1);
    }
    _staStack.push(_actionGoTo[_GoTo{_staStack.top(), *_symbol}]._nextState);
}

void _GrammarParse::_SemanticAnalyse(list<Word> &_lexicalResult)
{
    // 在这里面进行自下而上的分析
    bool _acc = false; // 是否接受
    int _linesCnt = 1; // 记录行数
    // 初始化符号栈和状态栈
    _symStack.push(new _Symbol{"$", 1});
    _staStack.push(0);

    list<Word>::iterator _scanIter; // 扫描程序
    for (_scanIter = _lexicalResult.begin(); _scanIter != _lexicalResult.end();)
    {
        // 获得当前词的类型和符号
        auto _nowSymbol = _scanIter->first;
        auto _nowType = _scanIter->second;
        // cout << _nowSymbol << "\_n";
        if (_nowType == LCOMMENT || _nowType == SCOMMENT)
        {
            continue;
        }
        if (_nowType == NEXTLINE)
        {
            _scanIter++;
            _linesCnt++;
            continue;
        }
        _Symbol *_curSymbol;
        if (_nowType == ID)
        {
            _curSymbol = new _ID(_Symbol{"ID", true}, _nowSymbol);
        }
        else if (_nowType == NUM)
        {
            _curSymbol = new _NUM(_Symbol{"NUM", true}, _nowSymbol);
        }
        else
        {
            _curSymbol = new _Symbol(_nowSymbol, true);
        }

        // 下面就是根据符号来进行移进或者归约

        auto _nowSta = _staStack.top();
        // 查表获得下一步
        _GoTo _goTo(_nowSta, *_curSymbol);
        if (_actionGoTo.count(_goTo) == 0)
        {
            _errorLog("归约失败,找不到对应的action");
            exit(-1);
        }

        auto _nowAction = _actionGoTo[_goTo];
        if (_nowAction._operator == shift)
        {
            // 移进
            _symStack.push(_curSymbol);
            _staStack.push(_nowAction._nextState);
            _scanIter++;
        }
        else if (_nowAction._operator == acc)
        {
            _acc = true;
            break;
        }
        else
        {
            //======================================//
            // 语法分析部分可以简单用下列方式实现

            // 归约需要弹出，另外再压入一个左边即可
            /*
            auto _nowPro = _productions[_nowAction._nextState];
            auto _popTimes = _nowPro._right.size();
            while (_popTimes--)
            {
                _popTimes;
                _PopStack();
            }
            // 压入产生式左边
            _PushStack(_nowPro._left);
            */

            //======================================//

            //======================================//
            // 下面要实现语义分析，那么就需要对所有产生式进行分别操作
            auto _nowProIdx = _nowAction._nextState; // 获得当前产生式的ID
            auto _nowPro = _productions[_nowAction._nextState];
            auto _popTimes = _nowPro._right.size();
            switch (_nowProIdx)
            {
            case 1:
            {
                // P -> N declare_block
                // 这里首先需要弹出 N 和 declare_block
                // 然后需要将main的入口回填到N
                _Fun *_f = _LookUpFunc("main");
                _PopStack();
                _N *_n = (_N *)_PopStack();
                _code._BackPatch(_n->_nextList, _f->_enterPoint);
                _Symbol *_p = new _Symbol(_nowPro._left);
                _PushStack(_p);
                break;
            }
            case 2:
            {
                // value_expression -> sub_expression
                // 这里弹出子表达式
                // 然后将变量名字复制
                _ValueExpression *_add_expression = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_expName = _add_expression->_expName;
                _PushStack(_expression);
                break;
            }
            case 3:
            {
                // value_expression -> sub_expression > sub_expression
                // 这里首先需要弹出两个子表达式
                // 由于这是个布尔表达式，那么归约之后的假出口是这条语句,而下面跳转的是不知道，所以置-1标记
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *_gt = _PopStack();
                _ValueExpression *_add_expression1 = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_falseList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j<=", _add_expression1->_expName, _add_expression2->_expName, "-1"});
                _PushStack(_expression);
                break;
            }
            case 4:
            {
                // value_expression -> sub_expression < sub_expression
                // 这里基本同上，不再赘述
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *_lt = _PopStack();
                _ValueExpression *_add_expression1 = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_falseList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j>=", _add_expression1->_expName, _add_expression2->_expName, "-1"});
                _PushStack(_expression);
                break;
            }
            case 5:
            {
                // value_expression -> sub_expression == sub_expression
                // 这里基本同上，不再赘述
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *_eq = _PopStack();
                _ValueExpression *_add_expression1 = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_falseList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j!=", _add_expression1->_expName, _add_expression2->_expName, "-1"});
                _PushStack(_expression);
                break;
            }
            case 6:
            {
                // value_expression -> sub_expression >= sub_expression
                // 这里基本同上，不再赘述
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *_get = _PopStack();
                _ValueExpression *_add_expression1 = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_falseList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j<", _add_expression1->_expName, _add_expression2->_expName, "-1"});
                _PushStack(_expression);
                break;
            }
            case 7:
            {
                // value_expression -> sub_expression <= sub_expression
                // 这里基本同上，不再赘述
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *_let = _PopStack();
                _ValueExpression *_add_expression1 = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_falseList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j>", _add_expression1->_expName, _add_expression2->_expName, "-1"});
                _PushStack(_expression);
                break;
            }
            case 8:
            {
                // value_expression -> sub_expression != sub_expression
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *neq = _PopStack();
                _ValueExpression *_add_expression1 = (_ValueExpression *)_PopStack();
                _Expression *_expression = new _Expression(_nowPro._left);
                _expression->_falseList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j==", _add_expression1->_expName, _add_expression2->_expName, "-1"});
                _PushStack(_expression);
                break;
            }
            case 9:
            {
                // sub_expression -> subsub_expression
                // 将子句归约，变量名称复制
                _FomalPar *_item = (_FomalPar *)_PopStack();
                _ValueExpression *_add_expression = new _ValueExpression(_nowPro._left);
                _add_expression->_expName = _item->_expName;
                _PushStack(_add_expression);
                break;
            }
            case 10:
            {
                // sub_expression -> subsub_expression + sub_expression
                // 这里做运算操作，需要额外申请一个临时变量来存放变量的值
                // 最后把中间变量的名称复制，压入符号栈内
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *add = _PopStack();
                _FomalPar *_item = (_FomalPar *)_PopStack();
                _ValueExpression *_add_expression1 = new _ValueExpression(_nowPro._left);
                _add_expression1->_expName = _tempVar._MakeTemp();
                _code._Emit(_Quaternary{"+", _item->_expName, _add_expression2->_expName, _add_expression1->_expName});
                _PushStack(_add_expression1);
                break;
            }
            case 11:
            {
                // sub_expression -> subsub_expression - sub_expression
                // 这里基本同上，不再赘述
                _ValueExpression *_add_expression2 = (_ValueExpression *)_PopStack();
                _Symbol *_sub = _PopStack();
                _FomalPar *_item = (_FomalPar *)_PopStack();
                _ValueExpression *_add_expression1 = new _ValueExpression(_nowPro._left);
                _add_expression1->_expName = _tempVar._MakeTemp();
                _code._Emit(_Quaternary{"-", _item->_expName, _add_expression2->_expName, _add_expression1->_expName});
                _PushStack(_add_expression1);
                break;
            }
            case 12:
            {
                // subsub_expression -> value
                // 子句归约，变量名复制
                _Value *_value = (_Value *)_PopStack();
                _FomalPar *_item = new _FomalPar(_nowPro._left);
                _item->_expName = _value->_expName;
                _PushStack(_item);
                break;
            }
            case 13:
            {
                // subsub_expression -> value * subsub_expression
                // 这里基本同上，不再赘述
                _FomalPar *_item2 = (_FomalPar *)_PopStack();
                _Symbol *_mul = _PopStack();
                _Value *_value = (_Value *)_PopStack();
                _FomalPar *_item1 = new _FomalPar(_nowPro._left);
                _item1->_expName = _tempVar._MakeTemp();
                _code._Emit(_Quaternary{"*", _value->_expName, _item2->_expName, _item1->_expName});
                _PushStack(_item1);
                break;
            }
            case 14:
            {
                // subsub_expression -> value / subsub_expression
                // 这里基本同上，不再赘述
                _FomalPar *_item2 = (_FomalPar *)_PopStack();
                _Symbol *_div = _PopStack();
                _Value *_value = (_Value *)_PopStack();
                _FomalPar *_item1 = new _FomalPar(_nowPro._left);
                _item1->_expName = _tempVar._MakeTemp();
                _code._Emit(_Quaternary{"/", _value->_expName, _item2->_expName, _item1->_expName});
                _PushStack(_item1);
                break;
            }

            case 15:
            {
                // value -> NUM
                // 子句归约，变量名复制
                _NUM *_num = (_NUM *)_PopStack();
                _Value *_value = new _Value(_nowPro._left);
                _value->_expName = _num->_val;
                _PushStack(_value);
                break;
            }
            case 16:
            {
                // value -> ( value_expression )
                // 这里与上面的区别是需要把括号弹出来
                _Symbol *_rparen = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _Value *_value = new _Value(_nowPro._left);
                _value->_expName = _expression->_expName;
                _PushStack(_value);
                break;
            }
            case 17:
            {
                // value -> ID ( argument_list )
                // 首先需要把参数和括号弹出来
                // 然后这里需要查函数表， 进行两项检查
                // 第一是检查函数是否有定义
                // 第二是检查函数的参数是否对应
                // 接着打印参数列表，将返回值用中间变量存储
                _Symbol *_rparen = _PopStack();
                _ArgumentList *_argument_list = (_ArgumentList *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _ID *_name = (_ID *)_PopStack();
                _Value *_value = new _Value(_nowPro._left);
                _Fun *_f = _LookUpFunc(_name->_name);
                if (!_f)
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt - 1) + "行，未声明的函数" + _name->_name);
                    exit(-1);
                }
                else if (!_FunParMatch(_argument_list->_argList, _f->_parType))
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，函数" + _name->_name + "参数不匹配");
                    exit(-1);
                }
                else
                {
                    for (list<string>::iterator _iter = _argument_list->_argList.begin(); _iter != _argument_list->_argList.end(); _iter++)
                    {
                        _code._Emit(_Quaternary{"para", *_iter, "_", "_"});
                    }
                    _value->_expName = _tempVar._MakeTemp();
                    _code._Emit(_Quaternary{"call", _name->_name, "_", "_"});
                    _code._Emit(_Quaternary{"=", "RETURN", "_", _value->_expName});
                    _PushStack(_value);
                }
                break;
            }
            case 18:
            {
                // value -> ID
                // 检查变量是否声明
                _ID *_name = (_ID *)_PopStack();
                if (_LookUpVar(_name->_name) == NULL)
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt - 1) + "行，变量" + _name->_name + "未声明");
                    exit(-1);
                }
                _Value *_value = new _Value(_nowPro._left);
                _value->_expName = _name->_name;
                _PushStack(_value);
                break;
            }
            case 19:
            {
                // argument_list ->
                // 直接压入
                _ArgumentList *_argument_list = new _ArgumentList(_nowPro._left);
                _PushStack(_argument_list);
                break;
            }
            case 20:
            {
                // argument_list -> value_expression
                // 将参数压入参数列表中
                _Expression *_expression = (_Expression *)_PopStack();
                _ArgumentList *_argument_list = new _ArgumentList(_nowPro._left);
                _argument_list->_argList.push_back(_expression->_expName);
                _PushStack(_argument_list);
                break;
            }
            case 21:
            {
                // argument_list -> value_expression , argument_list
                // 将参数复制入参数列表中
                _ArgumentList *argument_list2 = (_ArgumentList *)_PopStack();
                _Symbol *_comma = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _ArgumentList *argument_list1 = new _ArgumentList(_nowPro._left);
                argument_list2->_argList.push_front(_expression->_expName);
                argument_list1->_argList.assign(argument_list2->_argList.begin(), argument_list2->_argList.end());
                _PushStack(argument_list1);
                break;
            }
            case 24:
            {
                // declare_expression -> int ID M L declare_fun
                // 将函数添加到函数列表中
                // 这里其实可以再实现一个函数查重的操作，可以支持重载，但是不能函数名和参数列表都一致！
                _FunctionDeclare *_functionDeclare = (_FunctionDeclare *)_PopStack();
                _Symbol *_L = _PopStack();
                _M *_m = (_M *)_PopStack();
                _ID *_name = (_ID *)_PopStack();
                _Symbol *_int = _PopStack();
                // 加入函数表内
                vector<_Fun>::iterator _tmpIter = find(_funTable.begin(), _funTable.end(), _Fun{_name->_name, DINT, _functionDeclare->_parList, _m->_quad});
                if (_tmpIter != _funTable.end())
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，重定义的函数" + _name->_name);
                    exit(-1);
                }
                _funTable.push_back(_Fun{_name->_name, DINT, _functionDeclare->_parList, _m->_quad});
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 25:
            {
                // declare_expression -> int ID declare_var
                // 将变量加入到变量表中
                // 这里可以实现一个变量查重的操作，也就是一个级别下不能有相同的变量
                _Symbol *_varDeclare = _PopStack();
                _ID *_name = (_ID *)_PopStack();
                _Symbol *_int = _PopStack();
                vector<_Var>::iterator _tmpIter = find(_varTable.begin(), _varTable.end(), _Var{_name->_name, DINT, _nowLevel});
                if (_tmpIter != _varTable.end())
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，重定义的变量" + _name->_name);
                    exit(-1);
                }
                _varTable.push_back(_Var{_name->_name, DINT, _nowLevel});
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 26:
            {
                // declare_expression -> void ID M L declare_fun
                // 同上，仅类型不同
                _FunctionDeclare *_functionDeclare = (_FunctionDeclare *)_PopStack();
                _Symbol *_L = _PopStack();
                _M *_m = (_M *)_PopStack();
                _ID *_name = (_ID *)_PopStack();
                _Symbol *_void = _PopStack();
                vector<_Fun>::iterator _tmpIter = find(_funTable.begin(), _funTable.end(), _Fun{_name->_name, DVOID, _functionDeclare->_parList, _m->_quad});
                if (_tmpIter != _funTable.end())
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，重定义的函数" + _name->_name);
                    exit(-1);
                }
                _funTable.push_back(_Fun{_name->_name, DVOID, _functionDeclare->_parList, _m->_quad});
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 28:
            {
                // declare_fun -> ( fun_parameter ) sentence_block
                // 将参数压入函数的参数列表中
                _SentenceBlock *_sentence_block = (_SentenceBlock *)_PopStack();
                _Symbol *_rparen = _PopStack();
                _Parameter *_paramter = (_Parameter *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _FunctionDeclare *_functionDeclare = new _FunctionDeclare(_nowPro._left);
                _functionDeclare->_parList.assign(_paramter->_parList.begin(), _paramter->_parList.end());
                _PushStack(_functionDeclare);
                break;
            }
            case 29:
            {
                // fun_parameter -> parameter_list
                // 将参数压入函数的参数列表中
                _ParameterList *_parameter_list = (_ParameterList *)_PopStack();
                _Parameter *_paramter = new _Parameter(_nowPro._left);
                _paramter->_parList.assign(_parameter_list->_parList.begin(), _parameter_list->_parList.end());
                _PushStack(_paramter);
                break;
            }
            case 30:
            {
                // fun_parameter -> void
                // 参数类型为VOID
                _Symbol *_void = _PopStack();
                _Parameter *_paramter = new _Parameter(_nowPro._left);
                _PushStack(_paramter);
                break;
            }
            case 31:
            {
                // parameter_list -> param
                // 参数类型为INT
                _Symbol *_param = _PopStack();
                _ParameterList *_parameter_list = new _ParameterList(_nowPro._left);
                _parameter_list->_parList.push_back(DINT);
                _PushStack(_parameter_list);
                break;
            }
            case 32:
            {
                // parameter_list -> param , parameter_list
                // 将参数压入函数列表中
                _ParameterList *_parameter_list2 = (_ParameterList *)_PopStack();
                _Symbol *_comma = _PopStack();
                _Symbol *_param = _PopStack();
                _ParameterList *_parameter_list1 = new _ParameterList(_nowPro._left);
                _parameter_list2->_parList.push_front(DINT);
                _parameter_list1->_parList.assign(_parameter_list2->_parList.begin(), _parameter_list2->_parList.end());
                _PushStack(_parameter_list1);
                break;
            }
            case 33:
            {
                // param -> int ID
                //  定义形参
                _ID *_name = (_ID *)_PopStack();
                _Symbol *_int = _PopStack();
                vector<_Var>::iterator _tmpIter = find(_varTable.begin(), _varTable.end(), _Var{_name->_name, DINT, _nowLevel});
                if (_tmpIter != _varTable.end())
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，重定义的变量" + _name->_name);
                    exit(-1);
                }
                _varTable.push_back(_Var{_name->_name, DINT, _nowLevel});
                _code._Emit(_Quaternary{"get", "_", "_", _name->_name});
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 34:
            {
                // sentence_block -> { partial_declare sentence_list }
                // 这里注意要使得当前的nextList指向最后语句的nextList
                // 这里还需要情况当前层级下的变量
                _Symbol *_rbrace = _PopStack();
                _SentenceList *_sentence_list = (_SentenceList *)_PopStack();
                _Symbol *_inner_declare = _PopStack();
                _Symbol *_lbrace = _PopStack();
                _SentenceBlock *_sentence_block = new _SentenceBlock(_nowPro._left);
                _sentence_block->_nextList = _sentence_list->_nextList;
                _nowLevel--;
                int _popNum = 0;
                for (vector<_Var>::reverse_iterator _riter = _varTable.rbegin(); _riter != _varTable.rend(); _riter++)
                {
                    if (_riter->_varLevel > _nowLevel)
                        _popNum++;
                    else
                        break;
                }
                for (int i = 0; i < _popNum; i++)
                {
                    _varTable.pop_back();
                }
                _PushStack(_sentence_block);
                break;
            }
            case 37:
            {
                // partial_declare_var -> int ID extend
                // 压入变量
                _Value *_value = (_Value *)_PopStack();
                _ID *_name = (_ID *)_PopStack();
                _Symbol *_int = _PopStack();
                _PushStack(new _Symbol(_nowPro._left));
                vector<_Var>::iterator _tmpIter = find(_varTable.begin(), _varTable.end(), _Var{_name->_name, DINT, _nowLevel});
                if (_tmpIter != _varTable.end())
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，重定义的变量" + _name->_name);
                    exit(-1);
                }
                _name->_content = _value->_expName;
                _varTable.push_back(_Var{_name->_name, DINT, _nowLevel});
                break;
            }
            case 38:
            {
                // sentence_list -> sentence M sentence_list
                // 这里需要回填sentence的nextList
                _SentenceList *_sentence_list2 = (_SentenceList *)_PopStack();
                _M *_m = (_M *)_PopStack();
                _Sentence *_sentence = (_Sentence *)_PopStack();
                _SentenceList *_sentence_list1 = new _SentenceList(_nowPro._left);
                _sentence_list1->_nextList = _sentence_list2->_nextList;
                _code._BackPatch(_sentence->_nextList, _m->_quad);
                _PushStack(_sentence_list1);
                break;
            }
            case 39:
            {
                // sentence_list -> _sentence
                // 基本同上
                _Sentence *_sentence = (_Sentence *)_PopStack();
                _SentenceList *_sentence_list = new _SentenceList(_nowPro._left);
                _sentence_list->_nextList = _sentence->_nextList;
                _PushStack(_sentence_list);
                break;
            }
            case 40:
            {
                // sentence -> if_sentence
                // 基本同上
                _If *_if_sentence = (_If *)_PopStack();
                _Sentence *_sentence = new _Sentence(_nowPro._left);
                _sentence->_nextList = _if_sentence->_nextList;
                _PushStack(_sentence);
                break;
            }
            case 41:
            {
                // sentence -> while_sentence
                // 基本同上
                _While *while_sentence = (_While *)_PopStack();
                _Sentence *_sentence = new _Sentence(_nowPro._left);
                _sentence->_nextList = while_sentence->_nextList;
                _PushStack(_sentence);
                break;
            }
            case 42:
            {
                // sentence -> return_sentence
                // 基本同上
                _Symbol *return_sentence = _PopStack();
                _Sentence *_sentence = new _Sentence(_nowPro._left);
                _PushStack(_sentence);
                break;
            }
            case 43:
            {
                // sentence -> assign_sentence
                // 基本同上
                _Symbol *assign_sentence = _PopStack();
                _Sentence *_sentence = new _Sentence(_nowPro._left);
                _PushStack(_sentence);
                break;
            }
            case 44:
            {
                // assign_sentence -> ID = value_expression ;
                // 赋值运算
                _Symbol *_comma = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _Symbol *_assign = _PopStack();
                _ID *_name = (_ID *)_PopStack();
                if (_LookUpVar(_name->_name) == NULL)
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt - 1) + "行，变量" + _name->_name + "未声明");
                    exit(-1);
                }
                _Symbol *assign_sentence = new _Symbol(_nowPro._left);
                _code._Emit(_Quaternary{"=", _expression->_expName, "_", _name->_name});
                _PushStack(assign_sentence);
                break;
            }
            case 45:
            {
                // return_sentence -> return ;
                // 返回语句
                _Symbol *_comma = _PopStack();
                _Symbol *_return = _PopStack();
                _code._Emit(_Quaternary{"return", "_", "_", "_"});
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 46:
            {
                // return_sentence -> return value_expression ;
                // 存在返回值的返回语句
                _Symbol *_comma = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _Symbol *_return = _PopStack();
                _code._Emit(_Quaternary{"return", _expression->_expName, "_", "_"});
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 47:
            {
                // while_sentence -> while M ( value_expression ) L sentence_block
                // 这里需要对sentence_block进行回填
                _SentenceBlock *_sentence_block = (_SentenceBlock *)_PopStack();
                _Symbol *_L = _PopStack();
                _Symbol *_rparen = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _M *_m = (_M *)_PopStack();
                _Symbol *_while = _PopStack();
                _While *_while_sentence = new _While(_nowPro._left);
                _code._BackPatch(_sentence_block->_nextList, _m->_quad);
                _while_sentence->_nextList = _expression->_falseList;
                _code._Emit(_Quaternary{"j", "_", "_", to_string(_m->_quad)});
                _PushStack(_while_sentence);
                break;
            }
            case 48:
            {
                // if_sentence -> if ( value_expression ) L sentence_block
                // 基本同上
                _SentenceBlock *_sentence_block = (_SentenceBlock *)_PopStack();
                _Symbol *_L = _PopStack();
                _Symbol *_rparen = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _Symbol *_if = _PopStack();
                _If *_if_sentence = new _If(_nowPro._left);
                _expression->_falseList.splice(_expression->_falseList.begin(), _sentence_block->_nextList);
                _if_sentence->_nextList = _expression->_falseList;
                _PushStack(_if_sentence);
                break;
            }
            case 49:
            {
                // if_sentence -> if ( value_expression ) L sentence_block N else M L sentence_block
                _SentenceBlock *_sentence_block2 = (_SentenceBlock *)_PopStack();
                _Symbol *_L2 = _PopStack();
                _M *_m = (_M *)_PopStack();
                _Symbol *_else = _PopStack();
                _N *_n = (_N *)_PopStack();
                _SentenceBlock *_sentence_block1 = (_SentenceBlock *)_PopStack();
                _Symbol *_L1 = _PopStack();
                _Symbol *_rparen = _PopStack();
                _Expression *_expression = (_Expression *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _Symbol *_if = _PopStack();
                _If *_if_sentence = new _If(_nowPro._left);
                _code._BackPatch(_expression->_falseList, _m->_quad);
                _if_sentence->_nextList = merge(_sentence_block1->_nextList, _sentence_block2->_nextList);
                _if_sentence->_nextList = merge(_if_sentence->_nextList, _n->_nextList);
                _PushStack(_if_sentence);
                break;
            }
            case 50:
            {
                // L ->
                _nowLevel++;
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            case 51:
            {
                // M ->
                _M *_m = new _M(_nowPro._left);
                _m->_quad = _code._GetNextQuad();
                _PushStack(_m);
                break;
            }
            case 52:
            {
                // N ->
                _N *_n = new _N(_nowPro._left);
                _n->_nextList.push_back(_code._GetNextQuad());
                _code._Emit(_Quaternary{"j", "_", "_", "-1"});
                _PushStack(_n);
                break;
            }
            case 53:
            {
                // extend -> = value
                _Value *_value = (_Value *)_PopStack();
                _Symbol *_assign = _PopStack();
                _Value *extend_sentence = new _Value(_nowPro._left);
                extend_sentence->_expName = _value->_expName;
                //_code._Emit(_Quaternary{"=", _expression->_expName, "_", _name->_name});
                _PushStack(extend_sentence);
                break;
            }
            case 54:
            {
                // extend ->
                _Value *_value = new _Value(_nowPro._left);
                _value->_expName = "0";
                _PushStack(_value);
                break;
            }
            case 55:
            {
                // sentence -> fun_sentence
                // 基本同上
                _Symbol *fun_sentence = _PopStack();
                _Sentence *_sentence = new _Sentence(_nowPro._left);
                _PushStack(_sentence);
                break;
            }
            case 56:
            {
                // value -> ID ( argument_list ) ;
                // 首先需要把参数和括号弹出来
                // 然后这里需要查函数表， 进行两项检查
                // 第一是检查函数是否有定义
                // 第二是检查函数的参数是否对应
                // 接着打印参数列表，将返回值用中间变量存储
                _PopStack(); // comma
                _Symbol *_rparen = _PopStack();
                _ArgumentList *_argument_list = (_ArgumentList *)_PopStack();
                _Symbol *_lparen = _PopStack();
                _ID *_name = (_ID *)_PopStack();
                _Fun *_f = _LookUpFunc(_name->_name);
                if (!_f)
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt - 1) + "行，未声明的函数" + _name->_name);
                    exit(-1);
                }
                else if (!_FunParMatch(_argument_list->_argList, _f->_parType))
                {
                    _errorLog(string("语法错误：第") + to_string(_linesCnt) + "行，函数" + _name->_name + "参数不匹配");
                    exit(-1);
                }
                else
                {
                    for (list<string>::iterator _iter = _argument_list->_argList.begin(); _iter != _argument_list->_argList.end(); _iter++)
                    {
                        _code._Emit(_Quaternary{"para", *_iter, "_", "_"});
                    }
                    _code._Emit(_Quaternary{"call", _name->_name, "_", "_"});
                    _PushStack(new _Symbol(_nowPro._left));
                }
                break;
            }
            case 57:
            {
                _PopStack(); // 分号
                _Value *_value = (_Value *)_PopStack();
                _Symbol *_assign = _PopStack();
                _Value *extend_sentence = new _Value(_nowPro._left);
                extend_sentence->_expName = _value->_expName;
                //_code._Emit(_Quaternary{"=", _expression->_expName, "_", _name->_name});
                _PushStack(extend_sentence);
                break;
            }
            case 58:
            {
                _Parameter *_paramter = new _Parameter(_nowPro._left);
                _PushStack(_paramter);
                break;
            }
            default:
            {
                // S -> P
                // P -> N declare_block
                // declare_block -> declare_expression declare_block
                // declare_block -> declare_expression
                // partial_declare ->
                // partial_declare -> partial_declare_var ; partial_declare
                // declare_var -> ;
                for (int i = 0; i < _popTimes; i++)
                {
                    _PopStack();
                }
                _PushStack(new _Symbol(_nowPro._left));
                break;
            }
            }
        }
    }

    if (!_acc)
    {
        _errorLog("分析失败，程序存在语法错误");
        exit(-1);
    }
    else
    {
        /*
        cout << "语法分析成功"
             << "\n";
        */
        // 输出语义分析内容
        /*
        for (auto &_e : _code._quaCode)
        {
            cout << "( " << _e._opt << " , " << _e._rs << " , " << _e._rt << " , " << _e._rd << " )\n";
        }*/
        // 这里需要初始化一下函数入口

        for (vector<_Fun>::iterator iter = _funTable.begin(); iter != _funTable.end(); iter++)
        {
            fun_enter.push_back(pair<int, string>(iter->_enterPoint, iter->_funName));
        }
        sort(fun_enter.begin(), fun_enter.end());
    }
}

bool _GrammarParse::_IsTs(const string &_symbol)
{
    if (_symbol == "int" || _symbol == "void" || _symbol == "if" || _symbol == "while" || _symbol == "else" || _symbol == "return" || _symbol == "$")
    {
        return true;
    }
    if (_symbol == "+" || _symbol == "-" || _symbol == "*" || _symbol == "/" || _symbol == "=" || _symbol == "==" || _symbol == ">" || _symbol == "<" || _symbol == "!=" || _symbol == ">=" || _symbol == "<=")
    {
        return true;
    }
    if (_symbol == ";" || _symbol == "," || _symbol == "(" || _symbol == ")" || _symbol == "{" || _symbol == "}" || _symbol == "ID" || _symbol == "NUM")
    {
        return true;
    }
    return false;
}