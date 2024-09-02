#include <iostream>
using namespace std;
#include <fstream>

#include "Common.h"
#include "mainwindow.h"

extern int abcde;

void _errorLog(string _info)
{
    abcde = 1;
    // 报错日志
    ofstream fout(R"(file\Result.txt)"); // 打开文件以进行追加写入

    if (fout.is_open()) {
        fout << "error: " << _info << "\n";
        fout.close(); // 关闭文件
    } else {
    }
    fout.close();
}

bool if_no_var(string op)
{
    if (op == "call" || op == "j")
        return true;
    else
        return false;
}

bool if_valid_var(string var)
{
    return isalpha(var[0]) && var != "RETURN";
}

bool if_control(string op)
{
    if (op == "return" || op == "call")
        return true;
    else
        return false;
}

bool if_valid_num(string num)
{
    return isdigit(num[0]);
}

bool need_store_first(string op)
{
    if (op[0] == 'j' || op == "call" || op == "return" || op == "get")
        return true;
    else
        return false;
}
