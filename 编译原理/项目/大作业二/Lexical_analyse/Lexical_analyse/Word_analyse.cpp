#include <iostream>
using namespace std;


#include <map>
#include <string>
#include <set>
#include <iomanip>
#include <stack>
#include "Word_analyse.h"


//��������ַ�
char wait_ch;

bool need_read = true;

int tmp_Digit = 0;
string tmp_ABC = "";
string tmp_string = "";

int col = 1;
int row = 1;

pair <string, string> word_content;
pair <int, int> word_pos;
pair <pair<string, string>, pair<int, int>> word_indi;

//�Ƿ��ǿհ׷�
bool isWhitespace(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

//�Ƿ�������
bool isDigit(char ch) {
    return (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' ||
        ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9');
}

//�Ƿ���Ӣ���ַ�
bool isABC(char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}





WordAnalyze::WordAnalyze(ifstream& file) : sourceFile(file) {
    key_words.insert({ "int","INT" });
    key_words.insert({ "while", "WHILE" });
    key_words.insert({ "do","DO" });
    key_words.insert({ "else","ELSE" });
    key_words.insert({ "if","IF" });
    key_words.insert({ "then","THEN" });
    key_words.insert({ "scanf","SCANF" });
    key_words.insert({ "printf","PRINTF" });
    key_words.insert({ "include","INCLUDE" });
    key_words.insert({ "iostream","IOSTREAM" });
    key_words.insert({ "for","FOR" });
    key_words.insert({ "char","CHAR" });
    key_words.insert({ "string","STRING" });
    key_words.insert({ "main","MAIN" });
    //����ؼ���
}


void WordAnalyze::wordsdivide()
{
    col = 1;
    row = 1;

    //��ȡ��һ���ַ�
    wait_ch = sourceFile.get();

    //δ�����ļ�ĩβ
    while (wait_ch != EOF) //
    {
        while (isWhitespace(wait_ch)) //�����հ��ַ�
        {
            if (wait_ch == ' ') {
                col++;
            }
            else if (wait_ch == '\n') {
                row++;
                col = 1;
            }
            wait_ch = sourceFile.get();
        }
        if (isDigit(wait_ch))  //���������
        {
            tmp_Digit = 0;
            tmp_Digit = wait_ch - '0';
            for (wait_ch = sourceFile.get(); isDigit(wait_ch); )
            {
                tmp_Digit = tmp_Digit * 10 + wait_ch - '0';
                col++;
                wait_ch = sourceFile.get();
            }
            //�����������ӵ��ַ�����
            word_content = make_pair(to_string(tmp_Digit), "NUM");
            word_pos = make_pair(row, col);
            word_indi = make_pair(word_content, word_pos);
            Word_table.push_back(word_indi);
        }
        else if (isABC(wait_ch))  //�����ABCabc�ַ�
        {
            tmp_ABC = "";
            tmp_ABC += wait_ch;

            for (wait_ch = sourceFile.get(); isABC(wait_ch) || isDigit(wait_ch); )
            {
                tmp_ABC += wait_ch;
                col++;
                wait_ch = sourceFile.get();
            }

            //�����ڹؼ��ֱ��н��в���
            map<string, string>::iterator it = key_words.find(tmp_ABC);

            //���û���ҵ�
            if (it == key_words.end()) {
                //���ڹؼ��ֱ��У��Ǳ�ʶ��
                //�����������ӵ��ַ�����
                word_content = make_pair(tmp_ABC, "ID");
                word_pos = make_pair(row, col);
                word_indi = make_pair(word_content, word_pos);
                Word_table.push_back(word_indi);

                ID_table.insert(tmp_ABC);//����õ���ID����ID����
            }
            else {
                //�ڹؼ��ֱ���
                //�����������ӵ��ַ�����
                word_content = make_pair(it->first, it->second);
                word_pos = make_pair(row, col);
                word_indi = make_pair(word_content, word_pos);
                Word_table.push_back(word_indi);
            }
        }
        else {
            //�����ʼ���������ַ�
            switch (wait_ch)
            {
                // ���£������ switch ���У�ÿһ�����ִ����֮�󣬶�ʹ�� ch �Ǵ�������ַ�
                // ����� switch �����棬ͳһ�ٶ�һ�����ַ�����
                case '&':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '&') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "&&");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("&&");
                        col++;
                        break;
                    }
                    else {
                        //��&���治��&��˵���Ƿ���&������&&
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "&");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("&");
                        break;
                    }

                case '|':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '|') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "||");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("||");
                        col++;
                        break;
                    }
                    else {
                        //��|���治��|��˵���Ƿ���|������||
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "|");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("|");
                        break;
                    }

                case '(':
                    //�����������ӵ��ַ�����
                    word_content = make_pair("(", "(");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("(");
                    break;

                case ')':
                    //�����������ӵ��ַ�����
                    word_content = make_pair(")", ")");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert(")");
                    break;

                case ';':
                    //�����������ӵ��ַ�����
                    word_content = make_pair(";", ";");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert(";");
                    break;

                case '+':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "+=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("+=");
                        col++;
                        break;
                    }
                    else {
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "+");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("+");
                        break;
                    }

                case '-':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "-=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("-=");
                        col++;
                        break;
                    }
                    else if (isDigit(wait_ch)) //�п����Ǹ���
                    {
                        wait_ch = sourceFile.get();

                        int tmp_Digit = 0;
                        tmp_Digit = wait_ch - '0';

                        for (wait_ch = sourceFile.get(); isDigit(wait_ch); )
                        {
                            tmp_Digit = tmp_Digit * 10 + wait_ch - '0';
                            col++;
                            wait_ch = sourceFile.get();
                        }
                        //�����������ӵ��ַ�����
                        word_content = make_pair("-" + to_string(tmp_Digit), "NUM");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        need_read = false;
                        break;
                    }
                    else {
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "-");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("-");
                        break;
                    }

                case '*':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "*=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("*=");
                        col++;
                        break;
                    }
                    else {
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "*");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("*");
                        break;
                    }

                case '/':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "/=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("/=");
                        col++;
                        break;
                    }
                    else {
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "/");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("/");
                        break;
                    }

                case '=':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "==");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("==");
                        col++;
                        break;
                    }
                    else {
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("=");
                        break;
                    }

                case '<':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    switch (wait_ch) {
                        case '=':
                            tmp_string += wait_ch;
                            wait_ch = sourceFile.get();

                            //�����������ӵ��ַ�����
                            word_content = make_pair(tmp_string, "<=");
                            word_pos = make_pair(row, col);
                            word_indi = make_pair(word_content, word_pos);
                            Word_table.push_back(word_indi);

                            Sign_table.insert("<=");
                            col++;
                            break;

                        case '<':
                            tmp_string += wait_ch;
                            wait_ch = sourceFile.get();

                            //�����������ӵ��ַ�����
                            word_content = make_pair(tmp_string, "<<");
                            word_pos = make_pair(row, col);
                            word_indi = make_pair(word_content, word_pos);
                            Word_table.push_back(word_indi);

                            Sign_table.insert("<<");
                            col++;
                            break;

                        default:
                            //�����������ӵ��ַ�����
                            word_content = make_pair(tmp_string, "<");
                            word_pos = make_pair(row, col);
                            word_indi = make_pair(word_content, word_pos);
                            Word_table.push_back(word_indi);

                            Sign_table.insert("<");
                            break;
                    }
                    break;

                case '>':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, ">=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert(">=");
                        col++;
                        break;
                    }
                    else if (wait_ch == '>')
                    {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, ">>");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert(">>");
                        col++;
                        break;
                    }
                    else
                    {
                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, ">");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert(">");
                        break;
                    }

                case '#':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�����������ӵ��ַ�����
                    word_content = make_pair(tmp_string, "BEGIN");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("#");
                    break;

                case '{': {
                    //�����������ӵ��ַ�����
                    word_content = make_pair("{", "{");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("{");
                    break;
                }

                case '}': {
                    //�����������ӵ��ַ�����
                    word_content = make_pair("}", "}");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("}");
                    break;
                }

                case '!':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //�鿴��һ���ַ�
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //�����������ӵ��ַ�����
                        word_content = make_pair(tmp_string, "!=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("!=");
                        col++;
                        break;
                    }
                    else {
                        tmp_string += wait_ch;
                        word_content = make_pair(tmp_string, "���Ϸ��ַ�");
                        word_pos = make_pair(row, col);
                        ERROR_LIST.insert({ word_content, word_pos });
                        break;
                    }

                default:
                    //�������ַ�һ���ǷǷ��ַ�
                    tmp_string = "";
                    tmp_string += wait_ch;

                    word_content = make_pair(tmp_string, "���Ϸ��ַ�");
                    word_pos = make_pair(row, col);
                    ERROR_LIST.insert({ word_content, word_pos });
                    break;
            }

            col++;
            if (need_read == true) {
                wait_ch = sourceFile.get();  // �˴���Ҫ�� get һ���µ��ַ�����
            }
            else {
                need_read = true;
            }
        }
    }

    //�����������ӵ��ַ�����
    word_content = make_pair("#", "FINISH");
    word_pos = make_pair(row, col);
    word_indi = make_pair(word_content, word_pos);
    Word_table.push_back(word_indi);

}



void WordAnalyze::output(ofstream& file)
{
    //������գ������﷨����
    if (!ERROR_LIST.empty())
    {
        cout << "�ʷ��������̳���" << endl;
        cout << "������Ϣ������ʾ" << endl;
        file << "�ʷ��������̳���" << endl;
        file << "������Ϣ������ʾ" << endl;
        for (auto it = ERROR_LIST.begin(); it != ERROR_LIST.end(); it++)
        {
            cout << it->first.second << "    " << it->first.first << "    ";
            cout << "λ�ã�" << it->second.first << "��" << it->second.second << "��" << endl;
        }
        return;
    }

    cout << "�ʷ��������" << endl << endl;
    file << "�ʷ��������" << endl << endl;
    for (auto it = Word_table.begin(); it != Word_table.end(); it++)
    {
        file << "�ַ�ֵ:" << setiosflags(ios::left) << setw(6) << it->first.first
            << "�������:" << setw(6) << it->first.second
            << "  λ��:" << it->second.first << "," << it->second.second << endl;
        cout << "�ַ�ֵ:" << setiosflags(ios::left) << setw(6) << it->first.first
            << "�������:" << setw(6) << it->first.second
            << "  λ��:" << it->second.first << "," << it->second.second << endl;
    }
    return;
}


void WordAnalyze::outeach(ofstream& file)
{
    cout << endl << "��ʶ����;" << endl;
    file << endl << "��ʶ����;" << endl;
    for (auto it = ID_table.begin(); it != ID_table.end(); it++)
    {
        file << "�ַ�ֵ:" << setiosflags(ios::left)
            << setw(6) << *it << "�������:��ʶ��" << endl;
        cout << "�ַ�ֵ:" << setiosflags(ios::left)
            << setw(6) << *it << "�������:��ʶ��" << endl;
    }

    cout << endl << "���ű�;" << endl;
    file << endl << "���ű�;" << endl;
    for (auto it = Sign_table.begin(); it != Sign_table.end(); it++)
    {
        file << "����ֵ:" << setiosflags(ios::left)
            << setw(6) << *it << "�������:Sign" << endl;
        cout << "����ֵ:" << setiosflags(ios::left)
            << setw(6) << *it << "�������:Sign" << endl;
    }

    cout << endl << "�ؼ��ֱ�;" << endl;
    file << endl << "�ؼ��ֱ�;" << endl;
    for (auto it = key_words.begin(); it != key_words.end(); it++)
    {
        file << "����ֵ:" << setiosflags(ios::left)
            << setw(6) << it->first << "�������:�ؼ���" << endl;
        cout << "����ֵ:" << setiosflags(ios::left)
            << setw(6) << it->first << "�������:�ؼ���" << endl;
    }
}