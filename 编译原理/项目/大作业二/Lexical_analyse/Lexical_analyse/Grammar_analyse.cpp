#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <stack>
#include "Word_analyse.h"
#include "Grammar_analyse.h"
using namespace std;

/*��������
* 1.���Դ򿪴�����ļ�
* 2.ʹ���˲��Ϸ��ķ���
* 3.ͷ�ļ�δ��������������
* 4.δ���б�������
* 5.���������
* 6.���������Ϊִ��
* 7.��ֵ���ʹ�ô���
* 8.While���û�����׵�do
* 9.While���֮�䶨���˱���
* 10.While���֮��ȱ���ж����
* 11.While�����ɲ�����
* 12.for���ȱ����ɳɷ�
* 13.��for���֮�ж������
* 14.�ض�����ͬ�ı���
* 15.��While���֮�ж����˱���
* 16.if�����ɲ�����
* 17.if���û�����׵�then
* 18.thenû�к������
* 19.{û�����׵�}��Ϊ��β
* 20.��ֵ���ʽû��;��Ϊ��β
* 21.�����������û��;��Ϊ��β
* 22.�������û����ȷ��д
* 23.������û����ȷ��д
* 24.������������˶����������ʽ
* 25.����������˶����������ʽ
* 26.��û�У���Ϊƥ��
* 27.�ķ�û��ִ�����
* 28.�ķ�û�ж������
* 29.ʹ����δ����ı���
* 30.�������ʽʹ��0��Ϊ��ĸ
*/


/*�﷨��
<����> ::= <ͷ�ļ�����>{<�ֳ���>}
<ͷ�ļ�����>::= #include<iostream> int main()
<�ֳ���> ::= <��������><ִ�����>
 <��������>::= <��������><��ʶ��>;{<�������� >}
<��ʶ��> ::= <��ĸ>{<��ĸ>|<����>}
<ִ�����>::=<�������>|<������>|<��ֵ���>|<�������>|<While���>|<For���>|{<ִ�����>}
<��ֵ���>::=<��ʶ��> <��ֵ�����> <���ʽ > ;
<While���>::= while(<�������>)do<ִ�����>
<For���>::= for([<��ֵ���>] <�������> ; <��ֵ���> ) <ִ�����>
<�������>::= if( <����> ) then <ִ�����> [ else <ִ�����> ]
<�������>::= <�߼�����ʽ> { || <�߼�����ʽ> }
<�߼�����ʽ>::= <�߼�����ʽ> { && <�߼�����ʽ> }
<�߼�����ʽ>::= <���ʽ><��ϵ�����><���ʽ>
<���ʽ>::= <��λ����ʽ> { |  < ��λ����ʽ>}
<��λ����ʽ>::= <��λ����ʽ> { &  < ��λ����ʽ>}
<��λ����ʽ>::= <λ�Ʊ��ʽ> { <λ�������> <λ�Ʊ��ʽ>}
<λ�Ʊ��ʽ>::= <�˳����ʽ>{<�Ӽ������> <�˳����ʽ>}
<�˳����ʽ>::= <����ʽ>{<�˳������> <����ʽ>}
<����ʽ>::=(<����ʽ>) |<��ʶ��>|<����>
<�˳������>::= * | /
<�Ӽ������>::= + | -
<λ�������>::= >> | <<
<��ϵ�����> ::= == | != | < | <= | > | >=
<��ֵ�����>::= = | += | -= | *= | /=
<�������> ::= scanf(<��ʶ��>)
<������> ::= printf(<��ʶ��>)
<��������>::= int | char | string
<��ĸ> ::= a|b|...|X|Y|Z
<����> ::= 0|1|2|...|8|9
<����> ::= [-] <����>
*/
pair<string, string> Error_type; //��������
pair<int, int> Error_pos; //����λ��
pair<string, string> IDNEED;
stack <string> outposi;//���ڱ�����ѭ��ʱ������
int tempValue = 0; //�ܾ���Ҫ��ջ
int Line_num = 100; //��תʱʹ�õ�λ�ñ��

bool hasLEF = false; //����Ƿ�ӵ��������
int forstart = 0;
int Sdeep = 0;
int times = 0;
#define VEC vector < pair< pair< string, string >, pair<int, int> > >::iterator
#define Word_result wordanalyze.Word_table
#define Error_list wordanalyze.ERROR_LIST
#define Key_word_list wordanalyze.keep
#define ERRORSET(s)  Error_type = make_pair(it->first.first, s);\
wordanalyze. ERROR_LIST.insert({ Error_type,it->second });


void GrammerAnalyze::Four(string s1, string s2, string s3, string s4)
{
    line.Type = s1;
    line.Number_1 = s2;
    line.Number_2 = s3;
    line.Name = s4;
    line.Posi = Line_num++;
    FourLine.push_back(line);
}

void GrammerAnalyze::FoueTemp(string s1, string s2, string s3, string s4)
{
    line_tmp.Type = s1;
    line_tmp.Number_1 = s2;
    line_tmp.Number_2 = s3;
    line_tmp.Name = s4;
    line_tmp.Posi = Line_num++;
    FourLine.push_back(line_tmp);
}

//��ʼ���﷨����
GrammerAnalyze::GrammerAnalyze(WordAnalyze& word) :wordanalyze(word) {
    Relop.insert("="); 
    Relop.insert("=="); 
    Relop.insert("<="); 
    Relop.insert("<"); 
    Relop.insert(">=");
    Relop.insert(">");  
    Relop.insert("!="); 
    Relop.insert(">");

    DEQU.insert("=");
    DEQU.insert("*=");
    DEQU.insert("/=");
    DEQU.insert("+=");
    DEQU.insert("-=");
    it = Word_result.begin();
}

//�����﷨����
bool GrammerAnalyze::grammeranalyze(){
    Program();
    if (it->first.second != "FINISH")
    {
        cout << "�﷨����ʧ��";
        return false;
    }
    Four("FINISH", "_", "_", "_");
    return true;
}

//�﷨�����ľ������
void GrammerAnalyze::Program()
{//ͷ�ļ����
    int i = 0;
    //���P��first����
    for (it = Word_result.begin(); it != Word_result.begin() + 10; it++, i++)
    {
        switch (i)
        {
            case 0:
                if (it->first.second != "BEGIN")
                {
                    Error_type = make_pair(it->first.second, "Should start with a '#'");
                    cout << "Should start with a '#'";
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    exit(0);
                }
                break;
            case 1:
                if (it->first.second != "INCLUDE")
                {
                    Error_type = make_pair(it->first.first, "Expect for 'include'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "Expect for 'include'";
                    exit(0);
                }
                break;
            case 2:
                if (it->first.first != "<")
                {
                    Error_type = make_pair(it->first.first, "please ues '<'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "please ues '<'";
                    exit(0);
                }
                break;
            case 3:
                if (it->first.second != "IOSTREAM")
                {
                    Error_type = make_pair(it->first.first, "Expect for 'iostream' to start");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "Expect for 'iostream' to start";
                    exit(0);
                }
                break;
            case 4:
                if (it->first.first != ">")
                {
                    Error_type = make_pair(it->first.first, "please use '>'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "please use '>'";
                    exit(0);
                }
                break;
            case 5:
                if (it->first.first != "int")
                {
                    Error_type = make_pair(it->first.first, "need 'int'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "need '{'";
                    exit(0);
                }
                break;
            case 6:
                if (it->first.first != "main")
                {
                    Error_type = make_pair(it->first.first, "need '{'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "need '{'";
                    exit(0);
                }
                break;
            case 7:
                if (it->first.first != "(")
                {
                    Error_type = make_pair(it->first.first, "need '{'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "need '{'";
                    exit(0);
                }
                break;
            case 8:
                if (it->first.first != ")")
                {
                    Error_type = make_pair(it->first.first, "need '{'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "need '{'";
                    exit(0);
                }
                break;
            case 9:
                if (it->first.first != "{")
                {
                    Error_type = make_pair(it->first.first, "need '{'");
                    wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
                    cout << "need '{'";
                    exit(0);
                }
                break;
        }
    }

    //���������������
    Define();
    Start();

    if (it->first.first != "}")
    {
        cout << "ȱ��}��Ϊ��β" << "λ�� " << it->second.first << "��" << it->second.second << "��";
        exit(0);
    }
    else
    {
        it++;
    }
}


//����������У���������������������Ƿ�������
void GrammerAnalyze::Define()
{
    //����int������
    if (it->first.second == "INT")
    {
        it++;
        if (it->first.second == "ID")
        {

            if (IDList.find(it->first.first) != IDList.end())//����ض���
            {
                cout << "�ض���Ԫ�أ�" << "     ����λ��" << it->second.first << "��" << it->second.second << "��";
                exit(0);
            }
            IDNEED = make_pair(it->first.first, "_");
            IDList.insert(IDNEED);
            string TempID = it->first.first;
            it++;
            if (it->first.first == ";")
            {
                it++;
                //����D����Ԫʽ
                Four("INT", TempID, "_", "_");
            }
            else
            {
                cout << "ȱ��;��β" << "     ����λ��"
                    << it->second.first << "��" << it->second.second << "��";
                exit(0);
            }
        }
        else
        {
            Error_type = make_pair(it->first.second, "D()fail");
            cout << "ȱ�������ı����� " << "     ����λ��"
                << it->second.first << "��" << it->second.second << "��";
            wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
            exit(0);
        }
        if (it->first.second == "INT" || it->first.second == "CHAR" || it->first.second == "STRING")
            Define();
    }
    //����char������
    else if (it->first.second == "CHAR")
    {
        it++;
        if (it->first.second == "ID")
        {

            if (IDList.find(it->first.first) != IDList.end())//����ض���
            {
                cout << "�ض���Ԫ�أ�" << "     ����λ��" << it->second.first << "��" << it->second.second << "��";
                exit(0);
            }
            IDNEED = make_pair(it->first.first, "_");
            IDList.insert(IDNEED);
            string TempID = it->first.first;
            it++;
            if (it->first.first == ";")
            {
                it++;
                //����D����Ԫʽ
                Four("CHAR", TempID, "_", "_");
            }
            else
            {
                cout << "ȱ��;��β" << "     ����λ��"
                    << it->second.first << "��" << it->second.second << "��";
                exit(0);
            }

        }
        else
        {
            Error_type = make_pair(it->first.second, "D()fail");
            cout << "ȱ�������ı����� " << "     ����λ��"
                << it->second.first << "��" << it->second.second << "��";
            wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
            exit(0);
        }
        if (it->first.second == "INT" || it->first.second == "CHAR" || it->first.second == "STRING")
            Define();
    }
    //����string������
    else if (it->first.second == "STRING")
    {
        it++;
        if (it->first.second == "ID")
        {

            if (IDList.find(it->first.first) != IDList.end())//����ض���
            {
                cout << "�ض���Ԫ�أ�" << "     ����λ��" << it->second.first << "��" << it->second.second << "��";
                exit(0);
            }
            IDNEED = make_pair(it->first.first, "_");
            IDList.insert(IDNEED);
            string TempID = it->first.first;
            it++;
            if (it->first.first == ";")
            {
                it++;
                //����D����Ԫʽ
                Four("STR", TempID, "_", "_");
            }
            else
            {
                cout << "ȱ��;��β" << "     ����λ��"
                    << it->second.first << "��" << it->second.second << "��";
                exit(0);
            }
        }
        else
        {
            Error_type = make_pair(it->first.second, "D()fail");
            cout << "ȱ�������ı����� " << "     ����λ��"
                << it->second.first << "��" << it->second.second << "��";
            wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
            exit(0);
        }
        if (it->first.second == "INT" || it->first.second == "CHAR" || it->first.second == "STRING")
            Define();
    }
    //������������
    else
    {
        Error_type = make_pair(it->first.second, "�����������");
        cout << "Please Check where ID was define" << "     ����λ��"
            << it->second.first << "��" << it->second.second << "��";
        wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
        exit(0);
    }
}

//������ÿһ�����Ŀ�ʼ���Ž��м��
//�������ĳһ�еĿ�ʼ�ַ���if������ת��if��Ӧ�ļ�麯��
void GrammerAnalyze::Start()
{
    //��������if��ʼ
    if (it->first.second == "IF")
    {
        forstart++;
        //ת��If����Ӧ�ļ�麯��
        IFCHECK();
        forstart--;
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            //�������м�����Ŀ�ʼ
            Start();
        }
    }
    //��������while��ʼ
    else if (it->first.second == "WHILE")
    {
        forstart++;
        WhileCheck();
        forstart--;
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    //��������{��ʼ
    else if (it->first.first == "{")
    {
        it++;
        jumpsymbol = false;
        Start();
        if (it->first.first == "}")
        {
            it++;
            if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
                it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
                || it->first.second == "{" || it->first.second == "SCANF"))
            {
                if (forstart == Sdeep && (forstart != 0) && (Sdeep != 0))
                    return;
                Start();
            }
        }
        else
        {
            cout << "���Ų�ƥ��" << it->second.first << "," << it->second.second;
            exit(0);
        }

    }
    //�������Ա��������ͱ�����ʼ
    else if (it->first.second == "ID" ||
        it->first.second == "CHAR" ||
        it->first.second == "STRING")
    {
        while (it->first.second == "ID" || it->first.second == "CHAR" || it->first.second == "STRING")
        {
            Numgiven();
            if (jumpsymbol == true)
            {
                jumpsymbol = false;
                break;
            }
        }
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    //��������for��ʼ
    else if (it->first.second == "FOR")
    {
        forstart++;
        FORCHECK();
        forstart--;
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            // if (forstart == Sdeep && (forstart != 0) && (Sdeep != 0))
                 //return;
            Start();
        }
    }
    //��������printf��ʼ
    else if (it->first.second == "PRINTF")
    {
        Printout();
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID"
            || it->first.second == "FOR" || it->first.second == "PRINTF" || it->first.second == "{"
            || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    //��������scanf��ʼ
    else if (it->first.second == "SCANF")
    {
        Scanfin();
        if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
            it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
            || it->first.second == "{" || it->first.second == "SCANF"))
        {
            Start();
        }
    }
    //��������������ſ�ʼ���ͻᱨ��
    else
    {
        cout << "ȱ��ִ����䣡" << "    ����λ��:" << it->second.first << "��"
            << it->second.second << "��";;
        exit(0);
    }
}

//if���ļ������
void GrammerAnalyze::IFCHECK()
{
    if (it->first.second == "IF")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            string temp_if = Bool();
            int trueout, falseout;
            Four("JNZ", temp_if, "_", "");//�����
            trueout = Line_num;
            FoueTemp("JEZ", temp_if, "_", "");//�ٳ���
            falseout = Line_num;
            if (it->first.second == ")")
            {
                it++;
                if (it->first.second == "THEN")
                {
                    it++;
                    FourLine[trueout - 101].Name = to_string(Line_num);
                    jumpsymbol = true;
                    Sdeep++;
                    Start();
                    Sdeep--;
                    //��������ת���
                    Foursentence Jtemp;
                    Jtemp.Type = "J";
                    Jtemp.Number_1 = Jtemp.Number_2 = "_";
                    Jtemp.Name = "";
                    Jtemp.Posi = Line_num++;
                    int jump_posi = Line_num;
                    FourLine.push_back(Jtemp);
                    if (it->first.second == "ELSE")
                    {
                        it++;
                        FourLine[falseout - 101].Name = to_string(Line_num);
                        jumpsymbol = true;
                        Sdeep++;
                        Start();
                        Sdeep--;
                    }
                    FourLine[jump_posi - 101].Name = to_string(Line_num);
                }
                else
                {
                    cout << "ȱ��Then" << "    ����λ��:" << it->second.first << "��"
                        << it->second.second << "��";
                    exit(0);
                }
            }
            else
            {
                cout << "ȱ�٣�" << "    ����λ��:" << it->second.first << "��"
                    << it->second.second << "��";
                exit(0);
            }

        }
        else
        {
            cout << "ȱ��IF" << "    ����λ��:" << it->second.first << "��"
                << it->second.second << "��";
            wordanalyze.ERROR_LIST.insert({ Error_type,it->second });
            exit(0);
        }
    }
}

//while���ļ������
void GrammerAnalyze::WhileCheck()
{
    if (it->first.second == "WHILE")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            int jumpback = Line_num;
            outposi.push(to_string(jumpback));//����ջ�� ������ѭ��ʹ��
            string temp_while = Bool();
            int trueout, falseout;
            Four("JNZ", temp_while, "_", "");//�����
            trueout = Line_num;
            FoueTemp("JEZ", temp_while, "_", "");//�ٳ���
            falseout = Line_num;
            if (it->first.second == ")")
            {
                it++;
                if (it->first.second == "DO")
                {
                    it++;
                    FourLine[trueout - 101].Name = to_string(Line_num); //�����ʽΪ���ʱ�� ����Ҫ����ִ��Do�����
                    jumpsymbol = true;
                    Sdeep++;
                    Start();
                    Sdeep--;
                    //��һ��ѭ���������֮�� ����Ҫ�����ص�while��ͷ�����ٴν����ж�
                    Foursentence Jtemp;
                    Jtemp.Type = "J";
                    Jtemp.Number_1 = Jtemp.Number_2 = "_";
                    Jtemp.Name = outposi.top();
                    outposi.pop();//��ѭ��ִ�����֮�� ����Ҫ���������ǰ��
                    Jtemp.Posi = Line_num++;
                    FourLine.push_back(Jtemp);
                    //������ ���Ǿ���֪�� �ٳ�����������
                    FourLine[falseout - 101].Name = to_string(Line_num);
                }
                else
                {
                    cout << "ȱ��Do" << "    ����λ��:" << it->second.first << "��"
                        << it->second.second << "��";;
                    exit(0);
                }
            }
            else {
                cout << "ȱ�٣�" << "    ����λ��:" << it->second.first << "��"
                    << it->second.second << "��";
                exit(0);
            }
        }
        else
        {
            cout << "ȱ��(" << "    ����λ��:" << it->second.first << "��"
                << it->second.second << "��";;
            exit(0);
        }
    }
}

//�����Ͷ�Ӧ���������������
void GrammerAnalyze::Numgiven()
{
    if (IDList.find(it->first.first) == IDList.end())//����Ƿ񱻶���
    {
        cout << "δ������ı���" << it->first.first << "    ����λ��:" << it->second.first << "��"
            << it->second.second << "��";
        exit(0);
    }
    else
    {
        string tempID = it->first.first;//��¼��ǰID��
        it++;
        if (it->first.first == "*=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("*", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_");
        }
        else if (it->first.first == "+=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("+", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_");
        }
        else if (it->first.first == "-=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("-", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_");
        }
        else if (it->first.first == "/=")
        {
            it++;
            string stringpoint = "t" + to_string(++tempValue);
            Four("/", tempID, OR(), stringpoint);
            FoueTemp("=", tempID, stringpoint, "_");
        }
        else if (it->first.first == "=")
        {
            it++;
            Four("=", tempID, OR(), "_");
        }
        else
        {
            it++;
            cout << "��֪���ĸ�ֵ����" << it->first.first << "    ����λ��:" << it->second.first << "��"
                << it->second.second << "��";
            exit(0);
        }
    }
    if (it->first.first == ")" && forstart > 0)
    {
        return;
    }
    if (it->first.first != ";")
    {
        cout << "ȱ�� ';' " << "    ����λ��:" << it->second.first << "��"
            << it->second.second << "��";;
        exit(0);
    }
    else
    {
        it++;
    }
}

//for���ļ������
void GrammerAnalyze::FORCHECK()
{
    it++;
    if (it->first.second == "(")
    {
        it++;
        if (it->first.second == "INT")
        {
            cout << "�����ʼ���������� " << "     ����λ��"
                << it->second.first << "��" << it->second.second << "��";
            exit(0);
        }
        else if (it->first.second == "ID")
        {//��ʼ������
            jumpsymbol = true;
            Numgiven();
        }
        else if (it->first.second == ";")
            it++;
        else
        {
            cout << "��֧����˶���for���" << "     ����λ��"
                << it->second.first << "��" << it->second.second << "��";
            exit(0);
        }
    }
    else
    {
        cout << "for���ȱ��(" << "     ����λ��"
            << it->second.first << "��" << it->second.second << "��";
        exit(0);
    }
    //for���˳����
    string jumpback = to_string(Line_num);
    outposi.push(jumpback);//����ջ�� ������ѭ��ʹ��
    string temp_if = Bool(); //������ɵ����˳����
    int trueout, falseout;
    Four("JNZ", temp_if, "_", "");//�����
    trueout = Line_num;
    FoueTemp("JEZ", temp_if, "_", "");//�ٳ���
    falseout = Line_num;

    if (it->first.second == ";")
    {
        it++;
        //FourLine[trueout - 101].Name = to_string(Line_num);
        jumpsymbol = true;
        string numback = to_string(Line_num);
        Numgiven();
        //outposi.push(to_string(Line_num - 1 ));//���뵱ǰ����λ��

        Foursentence Jtemp;
        Jtemp.Type = "J";
        Jtemp.Number_1 = Jtemp.Number_2 = "_";
        Jtemp.Name = outposi.top();
        outposi.pop();//��ѭ��ִ�����֮�� ����Ҫ����While�����ǰ��
        Jtemp.Posi = Line_num++;
        FourLine.push_back(Jtemp);
        outposi.push(numback);

        FourLine[trueout - 101].Name = to_string(Line_num);

        if (it->first.second == ")")
        {
            it++;
            if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID" ||
                it->first.second == "CHAR" || it->first.second == "FOR" || it->first.second == "PRINTF"
                || it->first.second == "{" || it->first.second == "SCANF")
                )
            {
                Sdeep++;
                Start();
                Sdeep--;
            }
            //��һ��ѭ���������֮�� ����Ҫ�����ص�for��ͷ�����ٴν����ж�
            Foursentence Jtemp;
            Jtemp.Type = "J";
            Jtemp.Number_1 = Jtemp.Number_2 = "_";
            Jtemp.Name = outposi.top();
            outposi.pop();//��ѭ��ִ�����֮�� ����Ҫ����While�����ǰ��
            Jtemp.Posi = Line_num++;
            FourLine.push_back(Jtemp);
            //������ ���Ǿ���֪�� �ٳ�����������
            FourLine[falseout - 101].Name = to_string(Line_num);
        }
        else
        {
            cout << "for���ȱ�٣�" << "    ����λ��:" << it->second.first << "��"
                << it->second.second << "��";
            exit(0);
        }
    }
    else
    {
        cout << "for���ȱ�� ; " << "     ����λ��"
            << it->second.first << "��" << it->second.second << "��";
        exit(0);
    }
}

//printf���ļ������
void GrammerAnalyze::Printout()
{
    if (it->first.second == "PRINTF")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            if ((it->first.second == "ID") && ((it + 1)->first.second == ")"))
            {
                string outtemp = OR();
                Four("OUT", outtemp, "_", "_");
                if (it->first.second == ")")
                {
                    it++;
                    /*
                    if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID"
                        || it->first.second == "FOR" || it->first.second == "PRINTF" ||it->first.second == "{"
                        || it->first.second == "SCANF")
                        && ((Sdeep - forstart) != 1 || forstart == 0))
                    {
                        Start();
                    }*/
                }
                else
                {
                    cout << "Print���ȱ�� )" << "    λ��:" << it->second.first << "��"
                        << it->second.second << "��";
                    exit(0);
                }
            }
            else
            {
                cout << "printf���ÿ��ֻ�����һ��������" << "    ����λ��:" << it->second.first << "��"
                    << it->second.second << "��";
                exit(0);
            }
        }
        else
        {
            cout << "Print���ȱ�� (" << "    λ��:" << it->second.first << "��"
                << it->second.second << "��";
            exit(0);
        }
    }
}

//scanf���ļ������
void GrammerAnalyze::Scanfin()
{
    if (it->first.second == "SCANF")
    {
        it++;
        if (it->first.second == "(")
        {
            it++;
            if ((it->first.second == "ID")
                && ((it + 1)->first.second == ")"))
            {
                string outtemp = OR();
                Four("IN", outtemp, "_", "_");
                if (it->first.second == ")")
                {
                    it++;
                    /*if ((it->first.second == "IF" || it->first.second == "WHILE" || it->first.second == "ID"
                        || it->first.second == "FOR" || it->first.second == "PRINTF" ||it->first.second == "{")
                        && ((Sdeep - forstart) != 1 || forstart == 0))
                    {
                        Start();
                    }*/
                }
                else
                {
                    cout << "sacnf���ȱ�� )" << "    ����λ��:" << it->second.first << "��"
                        << it->second.second << "��";
                    exit(0);
                }
            }
            else
            {
                cout << "scanf���ÿ��ֻ�ܸ�ֵһ��������" << "    ����λ��:" << it->second.first << "��"
                    << it->second.second << "��";
                exit(0);
            }
        }
        else
        {
            cout << "scanf���ȱ�� (" << "    ����λ��:" << it->second.first << "��"
                << it->second.second << "��";
            exit(0);
        }
    }
}


string GrammerAnalyze::Bool()
{
    string tempstring_pos1 = DoubleAnd();
    string tempstring_pos2;
    while (it->first.second == "||")
    {
        it++;
        tempstring_pos2 = DoubleAnd();
        string stringpoint = "t" + to_string(++tempValue);
        Four("|", tempstring_pos1, tempstring_pos2, stringpoint);
        string stringpoint_2 = "t" + to_string(++tempValue);
        FoueTemp("!=", stringpoint, "0", stringpoint_2);
        tempstring_pos1 = stringpoint_2;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::DoubleAnd() // &&
{
    string tempstring_pos1 = Relopcheck();
    string tempstring_pos2;
    while (it->first.second == "&&")
    {
        it++;
        tempstring_pos2 = Relopcheck();
        string stringpoint = "t" + to_string(++tempValue);
        Four("&", tempstring_pos1, tempstring_pos2, stringpoint);
        string stringpoint_2 = "t" + to_string(++tempValue);
        FoueTemp("!=", stringpoint, "0", stringpoint_2);
        tempstring_pos1 = stringpoint_2;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::Relopcheck()
{
    string tempstring_pos1 = OR();
    string tempstring_pos2;
    while (Relop.find(it->first.second) != Relop.end()) //�ж�>= > <= < != == 
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = OR();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::OR()
{
    string tempstring_pos1 = AND();
    string tempstring_pos2;
    while (it->first.second == "|")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = AND();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::AND()
{
    string tempstring_pos1 = Drift();
    string tempstring_pos2;
    while (it->first.second == "&")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = Drift();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::Drift()
{
    string tempstring_pos1 = ADDCheck();
    string tempstring_pos2;
    while (it->first.second == ">>" || it->first.second == "<<")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = ADDCheck();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::ADDCheck()//E��T { +T | -T }
{
    string tempstring_pos1 = MulCheck();
    string tempstring_pos2;
    while (it->first.second == "+" || it->first.second == "-")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = MulCheck();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::MulCheck()
{
    string tempstring_pos1 = Equal();
    string tempstring_pos2;
    while (it->first.second == "*" || it->first.second == "/")
    {
        string temp_type = it->first.first;
        it++;
        tempstring_pos2 = Equal();
        string stringpoint = "t" + to_string(++tempValue);
        Four(temp_type, tempstring_pos1, tempstring_pos2, stringpoint);
        tempstring_pos1 = stringpoint;
    }
    return tempstring_pos1;
}
string GrammerAnalyze::Equal()
{
    if (it->first.second == "(")
    {
        it++;
        string tempstring = OR();
        if (it->first.second == ")")
        {
            it++;
            return tempstring;
        }
        else
        {
            cout << "���Ϲ���������" << it->second.first << "��" << it->second.second << "��";
            exit(0);
        }
    }
    else if (it->first.second == "NUM")
    {
        it++;
        return (it - 1)->first.first;//���س���ֵ
    }
    else if (it->first.second == "ID")
    {
        if (IDList.find(it->first.first) == IDList.end())//����Ƿ񱻶���
        {
            cout << "δ������ı���" << it->first.first << " λ��   " << it->second.first
                << "��" << it->second.second << "��";
            exit(0);
        }
        it++;
        return (it - 1)->first.first; //���ر�����
    }
    else
    {
        cout << "���Ϲ�����" << it->first.first << " λ��   " << it->second.first
            << "��" << it->second.second << "��";
        exit(0);
    }
}

//����Ԫ�Ľ�����뵽�ļ���
void GrammerAnalyze::outputfour(ofstream& file)
{
    vector <Foursentence>::iterator itout = FourLine.begin();
    cout << endl << "������Ԫʽ�����" << endl << endl;
    for (; itout != FourLine.end(); itout++)
    {
        file << itout->Posi << " ";
        file << "(" << setiosflags(ios::left) << setw(4) << itout->Type << " ," << setw(4) << itout->Number_1;
        file << "," << setw(4) << itout->Number_2 << "," << setw(4) << itout->Name << ")" << endl;

        cout << itout->Posi << " ";
        cout << "(" << setiosflags(ios::left) << setw(4) << itout->Type << " ," << setw(4) << itout->Number_1;
        cout << "," << setw(4) << itout->Number_2 << "," << setw(4) << itout->Name << ")" << endl;
    }
}
