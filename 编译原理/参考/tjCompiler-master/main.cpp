#include "wordParse.h"
#include "grammarParse.h"
#include "ObjectCodeGen.h"
int main()
{
	// 先调词法工作
	wordParse wp;
	wp.work();
	wp.outputParseResult("LexicalResult.txt");

	// 再调语法分析
	_GrammarParse _gp("grammar.txt");
	_gp._Work();

	//再调语义分析
	_gp._SemanticAnalyse(wp.getWordParseResult());
	
	//目标代码生成
	ObjectCodeGen objectCodeGen;
	objectCodeGen.divideBlock(_gp.getFunTable(),_gp.getMediateCode());
	objectCodeGen.handleBlock();
	objectCodeGen.gen();

	system("pause");
	return 0;
}