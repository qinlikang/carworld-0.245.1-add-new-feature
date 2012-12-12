#pragma once
#include "CppSQLite3.h"



class MyDatabase
{
	explicit MyDatabase(const char* dir,const char* file, bool force=false); // force = true : if dir do not exist,create it. 
	~MyDatabase(void);

	CppSQLite3DB* get_db(){return &m_DB;}
	CppSQLite3DB m_DB;
public:

	static CppSQLite3DB* shared_input_database();

	static CppSQLite3DB* shared_output_database();
};

