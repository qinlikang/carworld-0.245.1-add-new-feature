#include "MyDatabase.h"
#include "H_Standard.h"
#include <boost/filesystem.hpp>
#include "CommandOption.h"

MyDatabase::MyDatabase( const char* dir,const char* file, bool force/*=false*/ )
{
	// use boost.filesystem to keep platform-independce
	// need compile the boost.filesystem to get the static lib.
	using namespace boost::filesystem;
	path p(dir);
	path fullname(p);
	fullname.append(file,file+strlen(file));
	string fname=fullname.string();

	m_bOpened=false;

	if(force)
	{
		try
		{
			if(portable_directory_name(p.string()) && !exists(p))
			{
				// create the folder
				create_directories(p);
			}
			else if(exists(p) && !is_directory(p))
			{
				throw HException("the specified directory already exist as a file : "+p.string());
			}
			else if(!portable_directory_name(p.string()))
			{
				throw HException(p.string()+" is not a portable directory name");
			}
		}
		catch(HException& e)
		{
			throw e;
		}
		catch(...)
		{
			throw HException("Cannot make directory for output database");
		}

		if(!portable_name(file))
		{
			throw HException("not portable file name:");
		}

		if(!exists(fullname))// if not exists, just create it
		{
			ofstream of(fname);
			of.close();
		}
	}

	if(!exists(fullname))
	{
		throw HException("Database file doesn't exist : "+fname);
	}
	else
	{
		try
		{
			m_DB.open(fname.c_str());
			m_bOpened=true;
		}
		catch (CppSQLite3Exception& e)
		{
			throw HException(string("Open database error : ")+e.errorMessage());
		}
	}
}

CppSQLite3DB* MyDatabase::shared_input_database()
{
	if(!CommandOption::Option().bIsInit)return NULL;
	static MyDatabase global_in_db(CommandOption::Option().InDir.c_str(),CommandOption::Option().InDatabaseFile.c_str(),false);
	return global_in_db.get_db();
}

CppSQLite3DB* MyDatabase::shared_output_database()// used to output records
{
	if(!CommandOption::Option().bIsInit)return NULL;
	static MyDatabase global_out_db(CommandOption::Option().OutDir.c_str(),CommandOption::Option().OutDatabaseFile.c_str(),true);
	CppSQLite3DB* db = global_out_db.get_db();
	if(!db->tableExists("RecordSummary"))
	{
		// DATE(YYYY-MM-DD); TIME(HH:MM:SS)
		db->execDML("create table RecordSummary (date DATE, time TIME, table_name TEXT);");
	}
	return db;
}

MyDatabase::~MyDatabase( void )
{

}

CppSQLite3DB* MyDatabase::get_db()
{
	if(m_bOpened)
		return &m_DB;
	else 
		return NULL;
}
