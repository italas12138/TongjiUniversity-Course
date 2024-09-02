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
	bool grammeranalyze(); //�﷨��������
	void outputfour(ofstream& file);

private:
	WordAnalyze& wordanalyze; //�﷨����Ҫʹ�ôʷ�����
	vector < pair< pair< string, string >, pair<int, int> > >::iterator it = wordanalyze.Word_table.begin();
	void Program();//��⿪ͷ�ļ�
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
	map<string, string> IDList; //�����ı��� <���� <,ֵ>>

	bool jumpsymbol = false;//ʹ�ô˱�Ǳ����Ƿ���Ҫ��ת
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
	vector < Foursentence > FourLine; //���ͣ������һ�������2 ����
	vector < Foursentence >::iterator start = FourLine.begin();

	void Four(string s1, string s2, string s3, string s4);
	void FoueTemp(string s1, string s2, string s3, string s4);


	Foursentence line;
	Foursentence line_tmp;
};