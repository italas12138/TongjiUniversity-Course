#include <iostream>
using namespace std;

#include <iomanip>
#include <fstream>
#include <string>

#include "Word_analyse.h"
#include "Grammar_analyse.h"
#include "Explain.h"


string test_file;

int main()
{
	cout << "请输入测试文件名称" << endl;
	cin >> test_file;

	ifstream fin;
	fin.open("../test/" + test_file, ios::in);// 打开源程序文件

	if (!fin.is_open()) {   // 检查文件是否成功打开，如否，报错并退出程序
		cerr << "无法打开文件 " << test_file << endl;
		cerr << "请检查文件名称和位置是否正确" << endl;
		return 0;
	}

	//词法分析
	WordAnalyze word(fin);
	word.wordsdivide();
	ofstream fout;
	fout.open("D:\\桌面资料\\1.txt", ios::out);
	word.output(fout);
	if (!word.ERROR_LIST.empty())
	{
		cout << "词法分析有错误，请检查代码对应位置";
		return 0;
	}
	ofstream fout_d;
	fout_d.open("D:\\桌面资料\\2.txt", ios::out);
	word.outeach(fout_d);


	//语法分析
	GrammerAnalyze grammer(word);
	grammer.grammeranalyze();
	ofstream fout_grammer;
	fout_grammer.open("D:\\桌面资料\\2.txt", ios::out);
	grammer.outputfour(fout_grammer);

	fin.close();
	fout.close();
	fout_grammer.close();
	fout.close();
	return 0;
}