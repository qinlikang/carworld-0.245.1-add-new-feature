#pragma once
#include "../src/OFFObjectPool.h"
#include "../src/ColladeObject.h"
#include <string>

class stillobject :
	public CWColladeFeature
{
public:
	stillobject(string name);
	virtual ~stillobject(void);

	virtual void draw_init();
	virtual void draw();

	Point3D GetPos()const;
// protected:
	std::string name;
	Ref MyRef;
	OFFObject* MyMesh;
};