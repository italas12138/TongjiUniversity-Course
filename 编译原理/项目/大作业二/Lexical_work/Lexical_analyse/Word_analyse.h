#ifndef WORD_ANALYSE_H
#define WORD_ANALYSE_H

#include <iostream>
using namespace std;

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>



class WordAnalyze {
public:
    WordAnalyze(ifstream& fin);
    void wordsdivide();//词法分析
    void Word_output(string choice); //输出词法分析结
    map<pair<string, string>, pair<int, int>> ERROR_LIST; //错误位置和情况
private:
    map<string, string> key_words; //关键字表
    ifstream& sourceFile;           // 源程序文件输入流

    //词法分析结果
    vector<pair<pair<string, string>, pair<int, int>>> Word_table;  //总的字符表
    set <string> ID_table;    //标识符表
    set <string> Sign_table; //符号表

    friend class GrammerAnalyze;
};


#endif // WORD_ANALYSE_H
