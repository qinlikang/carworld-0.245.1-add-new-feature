#pragma once
#include "CppSQLite3.h"



class MyDatabase
{
	explicit MyDatabase(const char* dir,const char* file, bool force=false); // force = true : if dir do not exist,create it. 
	~MyDatabase(void);

	CppSQLite3DB* get_db();
	CppSQLite3DB m_DB;
	bool m_bOpened;
public:

	static CppSQLite3DB* shared_input_database();

	static CppSQLite3DB* shared_output_database();
};

