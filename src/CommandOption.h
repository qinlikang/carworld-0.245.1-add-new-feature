#pragma once
#include <string>
using namespace std;


class CommandOption
{
public:
	CommandOption(void);
	~CommandOption(void);

	string InDir;
	string OutDir;
	string InDatabaseFile;
	string OutDatabaseFile;

	bool bIsInit;
	static CommandOption& Option();

	void CheckOptions();
};

