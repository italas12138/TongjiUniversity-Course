#pragma once
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
    void wordsdivide();//�ʷ�����
    void output(ofstream& file); //����ʷ��������
    void outeach(ofstream& file); //��������ֱ� ID ��sign
    map<pair<string, string>, pair<int, int>> ERROR_LIST; //����λ�ú����
private:
    map<string, string> key_words; //�ؼ��ֱ�
    ifstream& sourceFile;           // Դ�����ļ�������
    
    //�ʷ��������
    vector<pair<pair<string, string>, pair<int, int>>> Word_table;  //�ܵ��ַ���
    set <string> ID_table;    //��ʶ����
    set <string> Sign_table; //���ű�

    friend class GrammerAnalyze;
};