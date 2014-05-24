#include "CommandOption.h"
#include <boost/filesystem.hpp>
#include "H_Standard.h"

const char* default_in_dir="data";
const char* default_out_dir="Records";

const char* default_in_database="carworld.db";
const char* default_out_database="record.db";


CommandOption::CommandOption(void)
{
	InDir=default_in_dir;
	OutDir=default_out_dir;

	InDatabaseFile=default_in_database;
	OutDatabaseFile=default_out_database;

	bIsInit=false;
}


CommandOption::~CommandOption(void)
{
}

CommandOption& CommandOption::Option()
{
	static CommandOption sOpt;
	return sOpt;
}

void CommandOption::CheckOptions()
{
	using namespace boost::filesystem;
	path p;

	p=InDir;
	if(!portable_directory_name(p.string()))
		throw HException(p.string()+" is not a portable directory name");

	p=OutDir;
	if(!portable_directory_name(p.string()))
		throw HException(p.string()+" is not a portable directory name");

	bIsInit=true;// finished initiation
}
