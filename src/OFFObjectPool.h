#pragma once
#include <map>
#include <string>
#include "H_Object.h"

class OFFObjectPool
{
	OFFObjectPool(void);
	~OFFObjectPool(void);

public:
	static OFFObjectPool* sharedOFFPool()
	{
		if(m_Pool==NULL) init();
		return m_Pool;
	}

	static void init()
	{
		if(m_Pool==NULL)
			m_Pool = new OFFObjectPool;
	}

	static void release()//release the pool before quit main
	{
		if(m_Pool!=NULL)
			delete m_Pool;
	}

	void loadOffs();// must be load in draw_init(). so early loading will failed.
	
	OFFObject* getMesh(const std::string& tag);

private:
	static OFFObjectPool* m_Pool;
	std::map<std::string,OFFObject> m_Meshes;

	// use the filename as tag by default. 
	void loadOneFromFile(const std::string& filename, const std::string& tag="");
};

