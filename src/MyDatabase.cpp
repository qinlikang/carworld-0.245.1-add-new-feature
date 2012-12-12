#include "MyDatabase.h"
#include "H_Standard.h"
#include <boost/filesystem.hpp>

const char* g_in_database_dir="./data/";
const char* g_out_database_dir="./Records/";

const char* g_in_database_name="carworld.db";
const char* g_out_database_name="record.db";

MyDatabase::MyDatabase( const char* dir,const char* file, bool force/*=false*/ )
{
	// use boost.filesystem to keep platform-independce
	// need compile the boost.filesystem to get the static lib.
	using namespace boost::filesystem;
	path p(dir);

	if(force)
	{
		try
		{
			if(is_directory(p)&&!exists(p))
			{
				// create the folder
				create_directories(p);
			}
			else if(!is_directory(p))
			{
				throw HException("the specified parameter isn't a directory!");
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
	}

	p.append(file,file+strlen(file));

	m_DB.open(p.string().c_str());
}

CppSQLite3DB* MyDatabase::shared_input_database()
{
	static MyDatabase global_in_db(g_in_database_dir,g_in_database_name,false);
	return global_in_db.get_db();
}

CppSQLite3DB* MyDatabase::shared_output_database()
{
	static MyDatabase global_out_db(g_out_database_dir,g_out_database_name,true);
	return global_out_db.get_db();
}

MyDatabase::~MyDatabase( void )
{

}
