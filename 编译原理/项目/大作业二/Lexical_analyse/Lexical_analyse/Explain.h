#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "Word_analyse.h"
#include "Grammar_analyse.h"
using namespace std;

class Explain {

public:
	GrammerAnalyze& grammerresult;
	Explain(GrammerAnalyze&);
	void Valueout(int, int);
	void outing(int posi);
};