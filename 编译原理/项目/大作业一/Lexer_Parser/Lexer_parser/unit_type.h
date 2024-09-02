#ifndef UNIT_TYPE_H
#define UNIT_TYPE_H

#include <string>
#include <map>

using namespace std;


//语法单元的类型
enum class Unit_type
{
    //key_word 关键词

    //定义类型关键词
    key_int,             //int类型
    key_double,          //double类型
    key_char,            //char类型
    key_void,            //void类型

    //定义语句关键词
    key_if,              //if关键词
    key_else,            //else关键词
    key_for,             //for关键词
    key_while,           //while关键词
    key_include,         //include关键词
    key_main,            //main关键词
    key_return,          //return关键词
    key_cin,             //cin关键词
    key_cout,            //cout关键词


    //Terminal 终结符

    //终结符类型
    variable,            //变量
    number,              //数字，整数或小数
    the_end,             //#，终止符
    epsilon,             //空符号


    //符号终结符
    symbol_add,          //符号‘+’
    symbol_sub,          //符号‘-’
    symbol_mul,          //符号‘*’
    symbol_div,          //符号‘/’
    symbol_assign,       //符号‘=’
    symbol_equal,        //符号‘==’
    symbol_unequal,      //符号‘!=’
    symbol_g,            //符号‘>’     greater than
    symbol_ge,           //符号‘>=’    greater and equal
    symbol_l,            //符号‘<’     less than
    symbol_le,           //符号‘<=’    less and equal
    symbol_comma,        //符号‘,’     逗号英文 comma
    symbol_semicolon,    //符号‘;’     分号英文 semicolon
    symbol_llbag,        //符号‘(’     左小括号 left little bag
    symbol_rlbag,        //符号‘)’     右小括号 right little bag
    symbol_lbbag,        //符号‘{’     左大括号 left big bag
    symbol_rbbag,        //符号‘}’     右大括号 right big bag


    //终结符和非终结符的分界线
    //此枚举值之后的非终结符我么在读文件的时候加入
    board,



    //非终结符
    S0,				//拓广文法开始符号
    program,
    dec_list,		//declaration_list,声明列表
    dec,
    dec_type,
    func_dec,
    var_dec,
    params,
    stat_block,
    param_list,
    param,
    pre_param,
    in_dec,
    stat_str,
    in_var_dec,
    pre_in_var_dec,
    stat,
    if_stat,
    while_stat,
    return_stat,
    assign_stat,
    expr,
    add_expr,
    comp_add_expr,
    comp_operator,
    term,
    pre_term,
    factor,
    pre_factor,
    func_type,
    call,
    argus,
    argu_list,
    pre_expr,

};



const map<string, Unit_type> keyword_to_Unit_type = {
    {"int",    Unit_type::key_int},
    {"double", Unit_type::key_double},
    {"char",   Unit_type::key_char},
    {"void",   Unit_type::key_void},

    {"if",     Unit_type::key_if},
    {"else",   Unit_type::key_else},
    {"for",    Unit_type::key_for},
    {"while",  Unit_type::key_while},
    {"include",Unit_type::key_include},
    {"main",   Unit_type::key_main},
    {"return", Unit_type::key_return},
    {"cin",    Unit_type::key_cin},
    {"cout",   Unit_type::key_cout}
};



const map<string, Unit_type> Terminal_to_Unit_type = {
    {"int",     Unit_type::key_int},
    {"double",  Unit_type::key_double},
    {"char",    Unit_type::key_char},
    {"void",    Unit_type::key_void},
    {"if",      Unit_type::key_if},
    {"else",    Unit_type::key_else},
    {"for",     Unit_type::key_for},
    {"while",   Unit_type::key_while},
    {"include", Unit_type::key_include},
    {"main",    Unit_type::key_main},
    {"return",  Unit_type::key_return},
    {"cin",     Unit_type::key_cin},
    {"cout",    Unit_type::key_cout},
    {"variable", Unit_type::variable},
    {"number",   Unit_type::number},
    {"the_end",  Unit_type::the_end},
    {"epsilon",  Unit_type::epsilon},
    {"+",        Unit_type::symbol_add},
    {"-",        Unit_type::symbol_sub},
    {"*",        Unit_type::symbol_mul},
    {"/",        Unit_type::symbol_div},
    {"=",        Unit_type::symbol_assign},
    {"==",       Unit_type::symbol_equal},
    {"!=",       Unit_type::symbol_unequal},
    {">",        Unit_type::symbol_g},
    {">=",       Unit_type::symbol_ge},
    {"<",        Unit_type::symbol_l},
    {"<=",       Unit_type::symbol_le},
    {",",        Unit_type::symbol_comma},
    {";",        Unit_type::symbol_semicolon},
    {"(",        Unit_type::symbol_llbag},
    {")",        Unit_type::symbol_rlbag},
    {"{",        Unit_type::symbol_lbbag},
    {"}",        Unit_type::symbol_rbbag},

    {"S0",      Unit_type::S0},

    {"program"			,Unit_type::program},
    {"dec_list"			,Unit_type::dec_list},
    {"dec"				,Unit_type::dec},
    {"dec_type"			,Unit_type::dec_type},
    {"func_dec"			,Unit_type::func_dec},
    {"var_dec"			,Unit_type::var_dec},
    {"params"			,Unit_type::params},
    {"stat_block"		,Unit_type::stat_block},
    {"param_list"		,Unit_type::param_list},
    {"param"			,Unit_type::param},
    {"pre_param"		,Unit_type::pre_param},
    {"in_dec"			,Unit_type::in_dec},
    {"stat_str"			,Unit_type::stat_str},
    {"in_var_dec"		,Unit_type::in_var_dec},
    {"pre_in_var_dec"	,Unit_type::pre_in_var_dec},
    {"stat"				,Unit_type::stat},
    {"if_stat"			,Unit_type::if_stat},
    {"while_stat"		,Unit_type::while_stat},
    {"return_stat"		,Unit_type::return_stat},
    {"assign_stat"		,Unit_type::assign_stat},
    {"expr"				,Unit_type::expr},
    {"add_expr"			,Unit_type::add_expr},
    {"comp_add_expr"	,Unit_type::comp_add_expr},
    {"comp_operator"	,Unit_type::comp_operator},
    {"term"				,Unit_type::term},
    {"pre_term"			,Unit_type::pre_term},
    {"factor"			,Unit_type::factor},
    {"pre_factor"		,Unit_type::pre_factor},
    {"func_type"		,Unit_type::func_type},
    {"call"				,Unit_type::call},
    {"argus"			,Unit_type::argus},
    {"argu_list"		,Unit_type::argu_list},
    {"pre_expr"			,Unit_type::pre_expr},
};





//判断test是否为终结符
bool isVT(const Unit_type test);

//判断test是否为非终结符
bool isVN(const Unit_type test);

Unit_type transform_StrToUntitype(const string& str);

string GetStringFromUnitType(const map<string, Unit_type>& Terminal_to_Unit_type, Unit_type unit_type);



#endif // UNIT_TYPE_H
