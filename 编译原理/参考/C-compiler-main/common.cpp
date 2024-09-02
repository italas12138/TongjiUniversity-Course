#include "common.h"

void _errorLog(string _info)
{
    // 报错日志
    cerr << "error:" << _info << "\n";
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