#include "unit_type.h"
#include "Lexer.h"
#include "Parser.h"


#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>

#include <QDebug>

using namespace std;

//读入的文本或文件是ASCLL码
//首先要判断是数字、英文字符还是其他字符
bool isLetter(char ch)
{
    return (ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z');
}

bool isDigit(char ch)
{
    return (ch>='0'&&ch<='9');
}

Lexer_program::Lexer_program()
{
    this->line = 1;
    this->col = 0;

    this->cur_flag = 0;
    this->next_flag = 0;
}


Lexer_program::~Lexer_program()
{

}


bool Lexer_program::analyse_file(string filename)
{
    //观察是否打开
    bool is_open;

    //文件已经打开了，将其关闭
    if (this->file_in.is_open())
        file_in.close();

    //再次打开文件
    this->file_in.open(filename);
    is_open = this->file_in.is_open();

    if (is_open && !file_in.get(this->nextchar))
        this->next_flag = 0;

    this->cur_flag = 0;
    this->next_flag = 1;

    return is_open;
}

//获得下一个字符，其中cur_char是当前读到的字符，next_char是预先看到字符
//函数作用是将next_char给cur_char，然后next_char变为新看到的字符
bool Lexer_program::getNextchar()
{
    if(this->next_flag == 0)//新字符是非法的  即读到了文件尾
        return false;
    else
    {
        //新字符不是非法的，就可以读入
        this->curchar = this->nextchar;
        this->cur_flag = this->next_flag;

        if(this->curchar == '\n')
        {
            this->line++;
            this->col = 0;
        }
        else{
            this->col++;
        }

        //已经完成了将next_char给cur_char
        //现在要将next_char变为新字符
        if (!file_in.get(this->nextchar))
            this->next_flag = 0;
        return true;
    }
}



bool isSpace(char ch)
{
    static const string WHITE_SPACE = " \n\t\r";
    return WHITE_SPACE.find(ch) != string::npos;
}

word_unit Lexer_program::getNext_word_unit()
{
    qDebug()<<"1";

    word_unit new_word_unit;
    new_word_unit.unit_state = Wait;

    bool flag;
    flag = getNextchar();
    if(flag == 0)//读入未成功，说明已经读到文件尾，后面没有了
    {
        qDebug()<<"3";
        new_word_unit.unit_type = Unit_type::the_end;
        new_word_unit.unit_value = "the_end";
        new_word_unit.unit_state = EndOf;

        return new_word_unit;
    }
    else {
        if(isSpace(this->curchar))
        {
            if(this->curchar == ' ')
                qDebug()<<"Space";
            else if(this->curchar == '\n')
                qDebug()<<"换行";

            return getNext_word_unit();
        }

        //首先看是否是英文字符
        if(isLetter(this->curchar))
        {
            qDebug()<<"2";

            //创建容器储存字符串
            string buffer = "";
            buffer += this->curchar;

            //看之后的字符内容
            //如果遇到其他字符或者文件尾就截断，否则继续读
            while(1)
            {
                //遇到文件尾就退出
                if(this->next_flag)
                {
                    //遇到其他字符就退出
                    if(isLetter(this->nextchar)||isDigit(this->nextchar))
                    {
                        //下一个字符没有问题，可以读入，并放入buffer中
                        getNextchar();
                        buffer += this->curchar;
                    }
                    else
                        break;
                }
                else
                    break;
            }

            qDebug()<<QString::fromStdString(buffer);

            if(keyword_to_Unit_type.count(buffer))
            {
                new_word_unit.unit_type = keyword_to_Unit_type.at(buffer);
                new_word_unit.unit_value = buffer;
                new_word_unit.unit_state = OK;
            }
            else {
                new_word_unit.unit_type = Unit_type::variable;
                new_word_unit.unit_value = buffer;
                new_word_unit.unit_state = OK;
            }

            return new_word_unit;

        }

        //再看是否是数字或者小数点
        if(isDigit(this->curchar) || this->curchar == '.')
        {
            //使用正则表达式来匹配，更加方便
            regex reg(R";(^(\d+|\d+\.\d+|\d+\.|\.\d+)(e\d+)?$);");

            //创建容器储存字符串
            string buffer = "";
            buffer += this->curchar;

            //看之后的字符内容
            //如果遇到其他字符或者文件尾就截断，否则继续读
            while(1)
            {
                //遇到文件尾就退出
                if(this->next_flag)
                {
                    //遇到其他字符（不是。或者e）就退出
                    if(isDigit(this->nextchar) || this->nextchar == '.' || this->nextchar == 'e')
                    {
                        //下一个字符没有问题，可以读入，并放入buffer中
                        getNextchar();
                        buffer += this->curchar;
                    }
                    else
                        break;
                }
                else
                    break;
            }

            if (!regex_match(buffer, reg))
            {
                new_word_unit.unit_state = Error;
                return new_word_unit;
            }

            new_word_unit.unit_type = Unit_type::number;
            new_word_unit.unit_value = buffer;
            new_word_unit.unit_state = OK;

            return new_word_unit;
        }


        //下面看符号终结符（这里是不用前瞻的）
        if(this->curchar == '+' || this->curchar == '-' ||
                this->curchar == '*' ||this->curchar == ',' ||
                this->curchar == ';' ||this->curchar == '(' ||
                this->curchar == ')' ||this->curchar == '{' ||
                this->curchar == '}')
        {
             qDebug()<<"这里";
            new_word_unit.unit_type = Terminal_to_Unit_type.at(string("") + this->curchar);
            new_word_unit.unit_value = string("") + this->curchar;
            new_word_unit.unit_state = OK;
        }

        //处理需要前瞻的终结符

        //'='与'=='的区分
        if(this->curchar == '=')
        {
            if (this->nextchar == '=') {
                getNextchar();
                new_word_unit.unit_type = Unit_type::symbol_equal;
                new_word_unit.unit_value = string("==");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }
            else
            {
                new_word_unit.unit_type = Unit_type::symbol_assign;
                new_word_unit.unit_value = string("=");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }

        }

        //'!'需要前瞻，'!='有两部分需要一起在
        if(this->curchar == '!')
        {
            if (this->nextchar == '=') {
                getNextchar();
                new_word_unit.unit_type = Unit_type::symbol_unequal;
                new_word_unit.unit_value = string("!=");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }
            else
                return new_word_unit;
        }

        //'<'与'<='的区分
        if(this->curchar == '<')
        {
            if (this->nextchar == '=') {
                getNextchar();
                new_word_unit.unit_type = Unit_type::symbol_le;
                new_word_unit.unit_value = string("<=");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }
            else
            {
                new_word_unit.unit_type = Unit_type::symbol_l;
                new_word_unit.unit_value = string("<");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }

        }

        //'>'与'>='的区分
        if(this->curchar == '>')
        {
            if (this->nextchar == '=') {
                getNextchar();
                new_word_unit.unit_type = Unit_type::symbol_ge;
                new_word_unit.unit_value = string(">=");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }
            else
            {
                new_word_unit.unit_type = Unit_type::symbol_g;
                new_word_unit.unit_value = string(">");
                new_word_unit.unit_state = OK;

                return new_word_unit;
            }

        }

        if(this->curchar == '/')
        {
            //此时为单行注释
            if(this->nextchar == '/')
            {
                //一直读取，直到遇到换行符或文件尾
                while(this->curchar != '\n')
                {
                    int a = getNextchar();
                    //读到文件尾
                    if(a == 0)
                    {
                        new_word_unit.unit_type = Unit_type::the_end;
                        new_word_unit.unit_value = "the_end";
                        new_word_unit.unit_state = EndOf;

                        return new_word_unit;
                    }
                }

                return new_word_unit;
            }

            //此时为多行注释
            if(this->nextchar == '*')
            {
                while(1)
                {
                    //一直读取，直到遇到换行符或'*'
                    while(this->curchar != '*')
                    {
                        int a = getNextchar();
                        //读到文件尾
                        if(a == 0)
                        {
                            new_word_unit.unit_type = Unit_type::the_end;
                            new_word_unit.unit_value = "the_end";
                            new_word_unit.unit_state = EndOf;

                            return new_word_unit;
                        }
                    }

                    //遇到了'*'，但是还缺少'/'
                    if(this->nextchar == '/')
                    {
                        getNextchar();

                        new_word_unit.unit_state = OK;
                        return new_word_unit;
                    }
                }
            }

            new_word_unit.unit_type = Unit_type::symbol_div;
            new_word_unit.unit_value = "/";
            new_word_unit.unit_state = OK;

            return new_word_unit;
        }
    }
}


void Lexer_program::clear_word()
{
    //将读到的位置归零
    this->line = 1;
    this->col = 0;

    //将标志位置0，说明未读到信息
    this->cur_flag = 0;
    this->next_flag = 0;

}












