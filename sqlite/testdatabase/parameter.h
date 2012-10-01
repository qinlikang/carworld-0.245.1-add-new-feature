#pragma once
#include "Glob_Defs.h"
#include "sqlite3.h"
#include "RJMFTime.h"
#include "RJM_SQLite_Resultset.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;
class parameter
{
public:
	parameter(void);
	~parameter(void);
	int speedControl;
	string Subject;
	string landscape;
	vector <float> start;
	vector <int> conditions;
};

