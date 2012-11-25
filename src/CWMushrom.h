#pragma once
#include "ColladeObject.h"
class CWMushrom :
	public CWColladeFeature
{
public:
	CWMushrom(void);
	virtual ~CWMushrom(void);

	virtual void draw_init();
	virtual void draw();

	Point3D GetPos()const;
// protected:
	Ref MyRef;
	OFFObject* MyMesh;
};

