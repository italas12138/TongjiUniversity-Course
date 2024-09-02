#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <fstream>
#include <sstream>
#include <list>
#include <queue>
#include <QDebug>

#include "unit_type.h"
#include "Lexer.h"
#include "Parser.h"
#include "mainwindow.h"

using namespace std;

//构造函数
Parser_program::Parser_program()
{

}

//析构函数
Parser_program::~Parser_program()
{

}


//从中获得文法产生式
int Parser_program::Get_grammar_byfile(string filename)
{
    ifstream file_in(filename, ios::in);
    string line;

    string buffer;

    Grammar_list.push_back({Unit_type::S0, {}});




    if (file_in.is_open()) {
        while (getline(file_in, line)) {
            istringstream string_in(line);
            Grammar new_Grammar;


            if (string_in >> buffer) {

                new_Grammar.left = Terminal_to_Unit_type.at(buffer);

                string_in >> buffer;

                while (string_in >> buffer) {

                    new_Grammar.right.push_back(Terminal_to_Unit_type.at(buffer));
                }

                Grammar_list.push_back(new_Grammar);
            }
        }

        // 构造拓广文法
        Grammar_list[0].right.push_back(Grammar_list[1].left);

        file_in.close();

        return 1;
    }
    else
    {
        return 0;
    }
}


//初始化first集合
void Parser_program::Init_First_list()
{
    //我们要求first集
    //对于终结符，它们的first集为自身
    for(int i = 0; i < int(Unit_type::board); ++i)
    {
        first_list[Unit_type(i)] = {Unit_type(i)};
    }

    //之后是非终结符的求法
    vector<int> right_isVN;    //右边是非终结符

    //遍历整个语法产生式集合，将上面分类
    for(int i = 0; i < int(Grammar_list.size()); ++i)
    {
        //首先考虑右边是否是的
        if(isVT(Grammar_list[i].right[0]))
            first_list[Grammar_list[i].left].insert(Grammar_list[i].right[0]);
        else
            right_isVN.push_back(i);
    }



    //一次按顺序更新可能不正确，所以需要多次扫描
    while(1)
    {
        //定义first和last用于最后出去
        int first = 0;
        int last = 0;

        //扫描前先确定生成式左侧非终结符的first中元素数量
        for(int i = 0; i < int(right_isVN.size()); ++i)
        {
            int j = right_isVN[i];
            first += first_list[Grammar_list[j].left].size();
        }

        for(int i = 0; i < int(right_isVN.size()); ++i)
        {
            //右边的首字母是终结符，要进行考虑
            //如果右边首字符无epsilon，则将其first集加入左边的
            int j = right_isVN[i]; //帮助储存

            //对产生式右侧的每个元素都判断
            for (const auto& atom : Grammar_list[j].right)
            {
                //如果是非终结符
                if(isVN(atom))
                {
                    //如果其中包含epsilon，则将其中的加入后看下一个
                    if(first_list[atom].count(Unit_type::epsilon))
                    {
                        for (const auto& atom_atom : first_list[atom])
                        {
                            first_list[Grammar_list[j].left].insert(atom_atom);
                        }
                        continue;
                    }
                    //如果其中不包含epsilon，则将其中的加入退出
                    else
                    {
                        for (const auto& atom_atom : first_list[atom])
                        {
                            first_list[Grammar_list[j].left].insert(atom_atom);
                        }
                        break;
                    }
                }
                //如果不是终结符就将其加入后退出
                else
                {
                    first_list[Grammar_list[j].left].insert(atom);
                    break;
                }

            }
        }

        //扫描后再确定生成式左侧非终结符的first中元素数量
        for(int i = 0; i < int(right_isVN.size()); ++i)
        {
            int j = right_isVN[i];
            last += first_list[Grammar_list[j].left].size();
        }

        //如果二者一样，说明不再更新了，可以退出扫描
        if(first == last)
            break;
    }

    return;
}


//获得Closure集
set<LR1_process> Parser_program::getClosure
(const set<LR1_process>& project_process)
{
    set<LR1_process> Closure(project_process);			//project_set自身的所有项目都在闭包中
    set<LR1_process> help_project(project_process);	//辅助集合
    set<LR1_process> new_project;

    bool flag;
    while (true) {
        flag = false;

        for (const auto& atom : help_project) {	//扫描上一次产生的所有项目
            if (Grammar_list[atom.Grammar_num].right.size() > atom.point_addr &&
                    isVN(Grammar_list[atom.Grammar_num].right[atom.point_addr]))
            {
                //A->α.Bβ型
                Unit_type VN = Grammar_list[atom.Grammar_num].right[atom.point_addr];

                //求出first(βa)
                set<Unit_type> first_BA;
                if (atom.point_addr + 1 < Grammar_list[atom.Grammar_num].right.size())
                {
                    first_BA = first_list[Grammar_list[atom.Grammar_num].right[atom.point_addr + 1]];
                    auto p = first_BA.find(Unit_type::epsilon);
                    if (p != first_BA.cend()) {
                        //如果含有epsilon,则删除epsilon并把原项目的follows加入
                        first_BA.erase(p);
                        for (const auto& foll : atom.follow)
                            first_BA.insert(foll);
                    }
                }
                else {
                    for (const auto& foll : atom.follow)
                        first_BA.insert(foll);
                }

                for (int k = 0; k < Grammar_list.size(); k++) {
                    //扫描所有B->γ型的产生式
                    if (Grammar_list[k].left == VN) {

                        //若CLOSURE中不存在{B->γ,firstba},则加入
                        bool have = false;
                        for (auto it = Closure.begin(); it != Closure.end(); ++it) {
                            if (it->Grammar_num == k && it->point_addr == 0) {
                                //项目在集合
                                have = true;
                                if (it->follow != first_BA) {
                                    //若follows不完整,则插入新的follows
                                    flag = true;
                                    //由于集合元素的值无法修改,故只能覆盖之
                                    auto ngp = *it;
                                    for (Unit_type atom_first_BA : first_BA)
                                        ngp.follow.insert(atom_first_BA);
                                    Closure.erase(it);
                                    Closure.insert(ngp);
                                    new_project.insert(ngp);
                                }
                                break;
                            }
                        }
                        if (!have) {
                            //否则插入新项目
                            flag = true;
                            Closure.insert({ k,0,first_BA });
                            new_project.insert({ k,0,first_BA });
                        }
                    }
                }
            }
        }
        if (!flag)	//不再增加,则返回
            break;
        help_project = new_project;	//对新添加项目进行下一轮扫描
        new_project.clear();
    }

    return Closure;
}


//求Action_goto表
int Parser_program::initAction_Goto()
{
    project_process_list.clear();

    //初始状态为CLOSURE({S0->.program,#})
    set<LR1_process> time_process_set;
    LR1_process time_process;
    time_process.Grammar_num = 0;
    time_process.point_addr = 0;
    time_process.follow = {Unit_type::the_end};
    time_process_set.insert(time_process);


    project_process_list.push_back(getClosure(time_process_set));



    int pro_num = 0;		//新项目集下标
    for(pro_num = 0; pro_num < int(project_process_list.size()); ++pro_num) {

        set<LR1_process>& process_pro_num = project_process_list[pro_num];
        map<Unit_type, set<LR1_process>> new_process_map;		//当前项目集可以产生的新项目集

        //扫描所有项目
        for (const auto& atom : process_pro_num) {
            if (atom.point_addr < Grammar_list[atom.Grammar_num].right.size()) {
                //不是归约项目
                LR1_process A_project;
                A_project.Grammar_num = atom.Grammar_num;
                A_project.point_addr = atom.point_addr + 1;
                A_project.follow = atom.follow;
                new_process_map[Grammar_list[atom.Grammar_num].right[atom.point_addr]].insert(A_project);
            }
            else {
                //是归约项目
                if (atom.Grammar_num == 0 && atom.point_addr == 1 && atom.follow.size() == 1
                        && *atom.follow.cbegin() == Unit_type::the_end)
                {
                    Movement A_movement;
                    A_movement.action = Action::accept;
                    A_movement.go = atom.Grammar_num;
                    Action_goto[pro_num][Unit_type::the_end] = A_movement;	//可接受状态
                }
                else {
                    for (const auto& foll : atom.follow) {
                        if (Action_goto[pro_num].count(foll))
                            return 0;	//如果转移表该项已经有动作,则产生多重入口,不是LR(1)文法,报错
                        else
                        {
                            Movement B_movement;
                            B_movement.action = Action::reduction;
                            B_movement.go = atom.Grammar_num;
                            Action_goto[pro_num][foll] = B_movement;	//用该产生式归约
                        }
                    }
                }
            }
        }

        //生成新closure集，填写转移表
        for (const auto& atom : new_process_map) {
            set<LR1_process> new_Closure = getClosure(atom.second);	//生成新closure集
            int a = findSameProjectSet(new_Closure);				//查重
            if (a == -1) {
                project_process_list.push_back(new_Closure);

                Movement C_movement;
                C_movement.action = Action::shift_in;
                C_movement.go = int(project_process_list.size()) - 1;
                Action_goto[pro_num][atom.first] = C_movement;	//移进
            }
            else {

                Movement D_movement;
                D_movement.action = Action::shift_in;
                D_movement.go = a;
                Action_goto[pro_num][atom.first] = D_movement;	//移进
            }
        }
    }

    return 1;
}


//对过程中生成的相同的项目集查找
int Parser_program::findSameProjectSet(const set<LR1_process>& new_process)
{
    for (int i = 0; i < int(project_process_list.size()); i++) {
        if (project_process_list[i].size() != new_process.size())	//长度不同一定不重复
            continue;
        else {
            bool flag = true;

            auto A1 = new_process.cbegin();
            auto A2 = project_process_list[i].cbegin();

            while (A1 != new_process.cend() && A2 != project_process_list[i].cend()) {
                if (!(*A1 == *A2)) {
                    flag = false;
                    break;
                }
                ++A1;
                ++A2;
            }

            if (flag)
                return i;
        }
    }
    return -1;	//未找到
}


//语法分析函数，对源程序进行分析
int Parser_program::parser(const char* filename, word_unit& err_word_unit)
{
    lexer_program.line = 1;
    lexer_program.col = 0;

    if (!this->lexer_program.analyse_file(filename))
        return 0;

    stack<int> StateStack;	//状态栈
    stack<Unit_type> InputStack;	//输入栈
    stack<int> NStack;  //树结点栈，存放树节点下标

    StateStack.push(0);		//初始化
    InputStack.push(Unit_type::the_end);	//初始化
    //NStack.push(-1);			//初始化

    bool use_lastToken = false;	//判断是否使用上次的token
    word_unit now_word_unit;	//当前token

    int now_state;		//当前state
    Movement now_movement;	//当前动作

    while (true) {
        //需要新获取一个token
        if (!use_lastToken) {
            err_word_unit = this->lexer_program.getNext_word_unit();

            qDebug()<<QString::fromStdString(err_word_unit.unit_value);

            if (err_word_unit.unit_state == Error)
            {
                qDebug()<<"5";
                return 0;
            }
        }
        now_state = StateStack.top();						//获取当前状态

        if (Action_goto.count(now_state) == 0 || Action_goto[now_state].count(now_word_unit.unit_type) == 0) {
            //若对应表格项为空,则出错


            err_word_unit = now_word_unit;
            qDebug()<<"6";

            return 0;
        }
        now_movement = Action_goto[now_state][now_word_unit.unit_type];	//获取当前动作
            //移进


        if (now_movement.action == Action::shift_in) {
            StateStack.push(now_movement.go);
            InputStack.push(now_word_unit.unit_type);

            TNode node_in;	//移进的树结点
            node_in.tag = now_word_unit.unit_type;	//初始化tag值
            node_in.p = pTree.TNode_List.size();	//指定树节点在TNode_List中的下标
            pTree.TNode_List.push_back(node_in);	//移进树结点
            NStack.push(node_in.p);					//将树节点下标移进树栈（保证栈内结点和TNode_List中的结点一一对应）

            use_lastToken = false;
        }	//归约
        else if (now_movement.action == Action::reduction) {

            int len = Grammar_list[now_movement.go].right.size();	//产生式右部长度

            TNode node_left;								//产生式左部
            node_left.tag = Grammar_list[now_movement.go].left;	//产生式左部tag
            node_left.p = pTree.TNode_List.size();			//移进树结点

            //移出栈
            while (len-- > 0) {
                StateStack.pop();
                InputStack.pop();

                node_left.childs.push_front(NStack.top());	//创建子结点链表
                NStack.pop();
            }

            pTree.TNode_List.push_back(node_left);			//移进树栈

            now_state = StateStack.top();	//更新当前状态
            if (Action_goto.count(now_state) == 0 ||
                Action_goto[now_state].count(node_left.tag) == 0) {
                //若对应表格项为空,则出错



                err_word_unit = now_word_unit;
                return 0;
            }

            now_movement = Action_goto[now_state][node_left.tag];	//更新当前动作
            //入栈操作
            StateStack.push(now_movement.go);
            InputStack.push(node_left.tag);
            NStack.push(node_left.p);

            use_lastToken = true;
        }
        else //接受
        {


            pTree.RootNode = pTree.TNode_List.size() - 1;	//根结点即为最后一个移进树结点集的结点
            return 0;		//accept
        }
    }
}



//画树
void Parser_program::Parser_PTree(ostream& out)
{
    if (pTree.RootNode == -1)	//没有根节点，树都不存在，没得画咯
        return;
    queue<int> Q;
    out << "digraph parser_tree{" << endl;
    out << "rankdir=TB;" << endl;

    //初始化结点
    for (int i = 0; i < int(pTree.TNode_List.size()); i++)
    {
        out << "node_" << i << "[label=\"" << GetStringFromUnitType(Terminal_to_Unit_type, pTree.TNode_List[i].tag) << "\" ";
        out << "shape=\"";
        if (isVT(pTree.TNode_List[i].tag)) //终结符，蓝色字体，无圆框
            out << "none\" fontcolor=\"blue\"];" << endl;
        else                               //非终结符，黑色字体，有圆框
            out << "box\" fontcolor=\"black\"];" << endl;
    }
    out << endl;

    Q.push(pTree.RootNode);	//根节点入队列，即将开始BFS输出语法树
    while (!Q.empty())
    {
        TNode node = pTree.TNode_List[Q.front()];	//取第一个结点，对其进行画树
        Q.pop();

        if (node.childs.size() == 0)	//若无子结点，不用画他的子树
            continue;
        //若有子结点，则画其子树
        for (auto it = node.childs.cbegin(); it != node.childs.cend(); it++)	//声明连接关系
        {
            out << "node_" << node.p << "->node_" << *it << ";" << endl;
            Q.push(*it);
        }
    }

    out << "}" << endl;
    return;
}

//画DFA
void Parser_program::Parser_DFA(ostream& out)
{
    out << "digraph{" << endl;
    out << "rankdir=LR;" << endl;
    //声明每一个项目集
    for (int i = 0; i < project_process_list.size(); i++)
    {
        out << "node_" << i << "[label=\"";
        //输出项目集中的每一个项目
        for (const auto& atom : project_process_list[i])
        {
            //输出产生式
            out << GetStringFromUnitType(Terminal_to_Unit_type, Grammar_list[atom.Grammar_num].left) << "->";
            int p;
            for (p = 0; p < Grammar_list[atom.Grammar_num].right.size(); p++)
            {
                if (atom.point_addr == p)
                    out << ".";
                out << GetStringFromUnitType(Terminal_to_Unit_type, Grammar_list[atom.Grammar_num].right[p]);
            }
            if (atom.point_addr == p)
                out << ".";
            out << ", ";
            //输出follows
            for (auto it = atom.follow.cbegin(); it != atom.follow.cend(); it++)
            {
                if (it != atom.follow.cbegin())
                    out << "/";
                out << GetStringFromUnitType(Terminal_to_Unit_type,*it);
            }
            out << "\n";
        }
        //声明结点属性
        out << "\" shape=\"box\"];" << endl;
    }

    //声明转移关系
    for (int i = 0; i < project_process_list.size(); i++)
    {
        for (const auto& tag_mov : Action_goto[i])
        {
            //只有移进才会转移
            if (tag_mov.second.action != Action::shift_in)
                continue;
            else
                out << "node_" << i << "->node_" << tag_mov.second.go << "[label=\"" << GetStringFromUnitType(Terminal_to_Unit_type, tag_mov.first) << "\"];" << endl;
        }
    }

    out << "}" << endl;
    return;
}

//清除语法分析器中的数据
void Parser_program::clear_data()
{
    //清除文法产生式
    this->Grammar_list.clear();

    //清除first集
    this->first_list.clear();

    //清除求CLOSURE集过程中的表
    this->project_process_list.clear();

    //清除产生的Action_goto表
    this->Action_goto.clear();

    //清空Lexer_program中的数据
    this->lexer_program.clear_word();


    //清空树
    this->pTree.TNode_List.clear();
    this->pTree.RootNode = -1;


}

