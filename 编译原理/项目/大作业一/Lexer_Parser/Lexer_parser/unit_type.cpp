#include "unit_type.h"
#include "Lexer.h"
#include "Parser.h"


#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>

using namespace std;


//判断test是否为终结符
bool isVT(const Unit_type test)
{
    return test < Unit_type::board;
}

//判断test是否为非终结符
bool isVN(const Unit_type test)
{
    return test > Unit_type::board;
}


Unit_type transform_StrToUntitype(const string& str)
{
    return Terminal_to_Unit_type.at(str);
}



string GetStringFromUnitType(const map<string, Unit_type>& Terminal_to_Unit_type, Unit_type unit_type) {
    for (const auto& atom : Terminal_to_Unit_type) {
        if (atom.second == unit_type) {
            return atom.first;
        }
    }
    return ""; // 如果找不到匹配的Unit_type，可以返回一个默认值或者抛出异常
}













