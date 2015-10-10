#include "OFFObjectPool.h"
#include "MyDatabase.h"
OFFObjectPool* OFFObjectPool::m_Pool=NULL;

OFFObjectPool::OFFObjectPool(void)
{
}


OFFObjectPool::~OFFObjectPool(void)
{
}

void OFFObjectPool::loadOneFromFile( const std::string& filename, const std::string& tag/*=""*/ )
{
// 	OFFObjectPool::sharedOFFPool()->loadOneFromFile("data/Landscape/mushroom.off","mushroom");
// 	OFFObjectPool::sharedOFFPool()->loadOneFromFile("data/Landscape/cone.off","cone");

	std::string map_tag;
	if(tag.empty()) map_tag = filename;
	else map_tag = tag;

	if(m_Meshes.find(map_tag)!=m_Meshes.end())// add an exist one
		throw HException("OFFObjectPool::loadOneFromFile -- Loading OFF error: the tag is already exist!");
	else 
	{
		m_Meshes[map_tag].readfile(filename.c_str());
	}
}

OFFObject* OFFObjectPool::getMesh( const std::string& tag )
{
	if(m_Meshes.find(tag)!=m_Meshes.end())
		return &(m_Meshes[tag]);
	else
		return NULL;
}

void OFFObjectPool::loadOffs()
{
	CppSQLite3Query q = MyDatabase::shared_input_database()->execQuery("select * from off_resource;");
	
	while(!q.eof())
	{
		string _tag = q.fieldValue("tag");
		string _script = q.fieldValue("script");
		const char* _off=q.fieldValue("path");
		if(_off)
			loadOneFromFile(_off,_tag);
		q.nextRow();
		m_Scripts[_tag]=_script;
	}
}

vector<string> OFFObjectPool::getTags() const
{
	std::map<std::string,OFFObject>::const_iterator it = m_Meshes.begin();
	vector<string> ret;
	for(;it!=m_Meshes.end();++it)
		ret.push_back(it->first);
	return ret;
}

const string& OFFObjectPool::getScriptName( const std::string& tag )
{
	return m_Scripts[tag];
}