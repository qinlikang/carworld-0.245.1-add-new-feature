
#include <iostream>
#include "parameter.h"

using namespace std;

parameter::parameter(void)
{
	sqlite3 *db=NULL;
    char *zErrMsg = 0;
    int rc;
   db = openDB("C:\\Users\\xzhang63\\Documents\\Visual Studio 2010\\Projects\\testdatabase\\Debug\\database");
    if( db==NULL ){
      cout<< "Can't open database: "<< sqlite3_errmsg(db)<<endl;
      sqlite3_close(db);
      exit(1);
    }
	checkTable(db,"parameter");
	string name=string("parameter");
	rc=getOneParameter(db,"speedControl",name,speedControl);
	rc=getOneParameterString(db,"Subject",name,Subject);
	rc=getOneParameterString(db,"landscape",name,landscape);
	cout<< "landscape " <<landscape<<endl;
	cout<< "Subject " <<Subject<<endl;
	cout<< "speedControl "<<speedControl<<endl;
}


parameter::~parameter(void)
{
}
