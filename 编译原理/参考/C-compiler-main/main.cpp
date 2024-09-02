#include <iostream>

#include "wordParse.h"
#include "grammarParse.h"
#include "blocksGenerator.h"
#include "memManager.h"
#include "objectCodeGenerator.h"
int main()
{
    string file_name;
    cout << "请输入文件名：如[test.txt]"
         << "\n";
    cin >> file_name;
    // 先调词法工作
    wordParse wp;
    wp.work(file_name);
    wp.outputParseResult("parseResult.txt");
    _GrammarParse gp("grammar.txt");
    gp._Work();
    gp._SemanticAnalyse(wp.getWordParseResult());
    ObjectCodeGenerator ocg(gp._code._quaCode, gp.fun_enter);
    ocg.generate_code();
    return 0;
}