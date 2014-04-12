#pragma once
#include "ColladeObject.h"
class CWCone
	: 	public CWPointObject
{
public:
	CWCone(void);
	~CWCone(void);

	virtual void draw_init();
	virtual void draw();

	Point3D GetPos()const;
	// protected:
	Ref MyRef;
	OFFObject* MyMesh;
};

