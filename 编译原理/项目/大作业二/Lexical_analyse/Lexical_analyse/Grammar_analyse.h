#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "Word_analyse.h"
using namespace std;


class GrammerAnalyze
{
public:
	GrammerAnalyze(WordAnalyze&);
	bool grammeranalyze(); //语法分析函数
	void outputfour(ofstream& file);

private:
	WordAnalyze& wordanalyze; //语法分析要使用词法分析
	vector < pair< pair< string, string >, pair<int, int> > >::iterator it = wordanalyze.Word_table.begin();
	void Program();//检测开头文件
	void Define();
	void Start();
	void Numgiven();
	void IFCHECK();
	void WhileCheck();
	void FORCHECK();
	void Printout();
	void Scanfin();

	string AND();
	string OR();
	string Bool();
	string Drift();
	string DoubleAnd();
	string Relopcheck();
	string ADDCheck();
	string MulCheck();
	string Equal();

	set<string> Relop;
	set<string> DEQU;
	map<string, string> IDList; //声明的变量 <名称 <,值>>

	bool jumpsymbol = false;//使用此标记表明是否需要跳转
	struct Foursentence
	{
		string Type;
		string Number_1;
		string Number_2;
		string Name;
		int Posi;
	};
	friend class Explain;
public:
	vector < Foursentence > FourLine; //类型，运算符一，运算符2 代称
	vector < Foursentence >::iterator start = FourLine.begin();

	void Four(string s1, string s2, string s3, string s4);
	void FoueTemp(string s1, string s2, string s3, string s4);


	Foursentence line;
	Foursentence line_tmp;
};