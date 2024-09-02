#ifndef LEXER_H
#define LEXER_H

#include "unit_type.h"
#include <string>
#include <fstream>

#define Wait 0
#define EndOf -1
#define Nothing -2
#define OK 1
#define Error 2

using namespace std;


enum class State	//返回值类型
{
    Ok = 0,
    FileOpenError,
    ERROR,
};



//额外的函数
bool isLetter(char ch);
bool isDigit(char ch);
bool isSpace(char ch);



//词法单元，其中有词的种类和属性值
struct word_unit{
    Unit_type unit_type;
    string unit_value;
    int unit_state;
    //0表示还没开始，准备状态
    //-1表示读到文件尾了
    //-2表示
    //1表示读到正常
    //2表示读到错误
};


//一个词法项目。
//当读取一个源程序的时候需要创建一个对应对象，负责整个词法分析过程。
class Lexer_program
{
private:

    //用两个标志变量，来标志读入字符的正确性
    bool cur_flag;
    bool next_flag;

    //记录读到的单词
    char curchar;
    char nextchar;

    ifstream file_in;


public:

    //记录当前读到的位置
    int line;
    int col;


    Lexer_program();
    ~Lexer_program();

    void Lexer_error();

    bool analyse_file(string filename);

    void clear_word();


    bool getNextchar();
    word_unit getNext_word_unit();

};

#endif // !LEXER_H
