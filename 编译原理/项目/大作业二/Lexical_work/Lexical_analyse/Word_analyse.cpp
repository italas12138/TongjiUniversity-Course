#include <iostream>
using namespace std;


#include <map>
#include <string>
#include <set>
#include <iomanip>
#include <stack>
#include "Word_analyse.h"


//待处理的字符
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

//是否是空白符
bool isWhitespace(char ch) {
    return (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');
}

//是否是数字
bool isDigit(char ch) {
    return (ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' ||
        ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9');
}

//是否是英文字符
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
    //插入关键字
}


void WordAnalyze::wordsdivide()
{
    col = 1;
    row = 1;

    //获取下一个字符
    wait_ch = sourceFile.get();

    //未读到文件末尾
    while (wait_ch != EOF) //
    {
        while (isWhitespace(wait_ch)) //遇到空白字符
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
        if (isDigit(wait_ch))  //如果是数字
        {
            tmp_Digit = 0;
            tmp_Digit = wait_ch - '0';
            for (wait_ch = sourceFile.get(); isDigit(wait_ch); )
            {
                tmp_Digit = tmp_Digit * 10 + wait_ch - '0';
                col++;
                wait_ch = sourceFile.get();
            }
            //将这个内容添加到字符表中
            word_content = make_pair(to_string(tmp_Digit), "NUM");
            word_pos = make_pair(row, col);
            word_indi = make_pair(word_content, word_pos);
            Word_table.push_back(word_indi);
        }
        else if (isABC(wait_ch))  //如果是ABCabc字符
        {
            tmp_ABC = "";
            tmp_ABC += wait_ch;

            for (wait_ch = sourceFile.get(); isABC(wait_ch) || isDigit(wait_ch); )
            {
                tmp_ABC += wait_ch;
                col++;
                wait_ch = sourceFile.get();
            }

            //首先在关键字表中进行查找
            map<string, string>::iterator it = key_words.find(tmp_ABC);

            //如果没有找到
            if (it == key_words.end()) {
                //不在关键字表中，是标识符
                //将这个内容添加到字符表中
                word_content = make_pair(tmp_ABC, "ID");
                word_pos = make_pair(row, col);
                word_indi = make_pair(word_content, word_pos);
                Word_table.push_back(word_indi);

                ID_table.insert(tmp_ABC);//将获得到的ID放在ID表里
            }
            else {
                //在关键字表中
                //将这个内容添加到字符表中
                word_content = make_pair(it->first, it->second);
                word_pos = make_pair(row, col);
                word_indi = make_pair(word_content, word_pos);
                Word_table.push_back(word_indi);
            }
        }
        else {
            //如果开始是其他的字符
            switch (wait_ch)
            {
                // 以下，在这个 switch 块中，每一种情况执行完之后，都使得 ch 是处理完的字符
                // 在这个 switch 块外面，统一再读一个新字符备用
                case '&':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '&') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "&&");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("&&");
                        col++;
                        break;
                    }
                    else {
                        //在&后面不是&，说明是符号&，不是&&
                        //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '|') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "||");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("||");
                        col++;
                        break;
                    }
                    else {
                        //在|后面不是|，说明是符号|，不是||
                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "|");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("|");
                        break;
                    }

                case '(':
                    //将这个内容添加到字符表中
                    word_content = make_pair("(", "(");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("(");
                    break;

                case ')':
                    //将这个内容添加到字符表中
                    word_content = make_pair(")", ")");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert(")");
                    break;

                case ';':
                    //将这个内容添加到字符表中
                    word_content = make_pair(";", ";");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert(";");
                    break;

                case '+':
                    tmp_string = "";
                    tmp_string += wait_ch;

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "+=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("+=");
                        col++;
                        break;
                    }
                    else {
                        //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "-=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("-=");
                        col++;
                        break;
                    }
                    else if (isDigit(wait_ch)) //有可能是负数
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
                        //将这个内容添加到字符表中
                        word_content = make_pair("-" + to_string(tmp_Digit), "NUM");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        need_read = false;
                        break;
                    }
                    else {
                        //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "*=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("*=");
                        col++;
                        break;
                    }
                    else {
                        //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "/=");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("/=");
                        col++;
                        break;
                    }
                    else {
                        //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
                        word_content = make_pair(tmp_string, "==");
                        word_pos = make_pair(row, col);
                        word_indi = make_pair(word_content, word_pos);
                        Word_table.push_back(word_indi);

                        Sign_table.insert("==");
                        col++;
                        break;
                    }
                    else {
                        //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    switch (wait_ch) {
                        case '=':
                            tmp_string += wait_ch;
                            wait_ch = sourceFile.get();

                            //将这个内容添加到字符表中
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

                            //将这个内容添加到字符表中
                            word_content = make_pair(tmp_string, "<<");
                            word_pos = make_pair(row, col);
                            word_indi = make_pair(word_content, word_pos);
                            Word_table.push_back(word_indi);

                            Sign_table.insert("<<");
                            col++;
                            break;

                        default:
                            //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
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

                        //将这个内容添加到字符表中
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
                        //将这个内容添加到字符表中
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

                    //将这个内容添加到字符表中
                    word_content = make_pair(tmp_string, "BEGIN");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("#");
                    break;

                case '{': {
                    //将这个内容添加到字符表中
                    word_content = make_pair("{", "{");
                    word_pos = make_pair(row, col);
                    word_indi = make_pair(word_content, word_pos);
                    Word_table.push_back(word_indi);

                    Sign_table.insert("{");
                    break;
                }

                case '}': {
                    //将这个内容添加到字符表中
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

                    //查看下一个字符
                    wait_ch = sourceFile.peek();

                    if (wait_ch == '=') {
                        tmp_string += wait_ch;
                        wait_ch = sourceFile.get();

                        //将这个内容添加到字符表中
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
                        word_content = make_pair(tmp_string, "不合法字符");
                        word_pos = make_pair(row, col);
                        ERROR_LIST.insert({ word_content, word_pos });
                        break;
                    }

                default:
                    //其他的字符一定是非法字符
                    tmp_string = "";
                    tmp_string += wait_ch;

                    word_content = make_pair(tmp_string, "不合法字符");
                    word_pos = make_pair(row, col);
                    ERROR_LIST.insert({ word_content, word_pos });
                    break;
            }

            col++;
            if (need_read == true) {
                wait_ch = sourceFile.get();  // 此处需要再 get 一个新的字符备用
            }
            else {
                need_read = true;
            }
        }
    }

    //将这个内容添加到字符表中
    word_content = make_pair("#", "FINISH");
    word_pos = make_pair(row, col);
    word_indi = make_pair(word_content, word_pos);
    Word_table.push_back(word_indi);

}



void WordAnalyze::Word_output(string choice)
{
    if(choice == "Error")
    {
        fstream fout(R"(file\Word_error.txt)");
        if (!ERROR_LIST.empty())
        {

            fout << "词法分析过程出错" << endl;
            fout << "错误信息如下所示" << endl;
            for (auto it = ERROR_LIST.begin(); it != ERROR_LIST.end(); it++)
            {
                fout << it->first.second << "    " << it->first.first << "    ";
                fout << "位置：" << it->second.first << "行" << it->second.second << "列" << endl;
            }
        }
        else{
            fout << "词法分析过程未出错" << endl;
            fout << "可以查看具体的符号表" << endl;
        }

        return;
    }

    else if(choice == "ID")
    {
        fstream fout(R"(file\Word_ID.txt)");
        if (!ERROR_LIST.empty())
        {
            fout << "词法分析过程出错" << endl;
            fout << "请进入错误报告查看具体内容" << endl;

        }
        else{
            fout <<  "标识符表;" << endl;
            for (auto it = ID_table.begin(); it != ID_table.end(); it++)
            {
                fout << "字符值:" << setiosflags(ios::left)
                    << setw(6) << *it << "符号类别:标识符" << endl;
            }
        }

        return;
    }

    else if(choice == "Sign")
    {
        fstream fout(R"(file\Word_Sign.txt)");
        if (!ERROR_LIST.empty())
        {
            fout << "词法分析过程出错" << endl;
            fout << "请进入错误报告查看具体内容" << endl;

        }
        else{
            fout << endl << "符号表;" << endl;
            for (auto it = Sign_table.begin(); it != Sign_table.end(); it++)
            {
                fout << "符号值:" << setiosflags(ios::left)
                    << setw(6) << *it << "符号类别:Sign" << endl;
            }
        }

        return;
    }

    else if(choice == "ALL")
    {
        fstream fout(R"(file\Word_all.txt)");
        if (!ERROR_LIST.empty())
        {
            fout << "词法分析过程出错" << endl;
            fout << "请进入错误报告查看具体内容" << endl;

        }
        else{
            fout << "词法分析结果" << endl << endl;
            for (auto it = Word_table.begin(); it != Word_table.end(); it++)
            {
                fout << "字符值:" << setiosflags(ios::left) << setw(6) << it->first.first
                    << "符号类别:" << setw(6) << it->first.second
                    << "  位置:" << it->second.first << "," << it->second.second << endl;
            }
        }

        return;
    }
}


