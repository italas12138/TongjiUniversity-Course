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
	cout << "����������ļ�����" << endl;
	cin >> test_file;

	ifstream fin;
	fin.open("../test/" + test_file, ios::in);// ��Դ�����ļ�

	if (!fin.is_open()) {   // ����ļ��Ƿ�ɹ��򿪣���񣬱����˳�����
		cerr << "�޷����ļ� " << test_file << endl;
		cerr << "�����ļ����ƺ�λ���Ƿ���ȷ" << endl;
		return 0;
	}

	//�ʷ�����
	WordAnalyze word(fin);
	word.wordsdivide();
	ofstream fout;
	fout.open("D:\\��������\\1.txt", ios::out);
	word.output(fout);
	if (!word.ERROR_LIST.empty())
	{
		cout << "�ʷ������д�����������Ӧλ��";
		return 0;
	}
	ofstream fout_d;
	fout_d.open("D:\\��������\\2.txt", ios::out);
	word.outeach(fout_d);


	//�﷨����
	GrammerAnalyze grammer(word);
	grammer.grammeranalyze();
	ofstream fout_grammer;
	fout_grammer.open("D:\\��������\\2.txt", ios::out);
	grammer.outputfour(fout_grammer);

	fin.close();
	fout.close();
	fout_grammer.close();
	fout.close();
	return 0;
}