#pragma once
#include "carworldclasses.h"
class CWMushrom :
	public CWFeature
{
public:
	CWMushrom(void);
	virtual ~CWMushrom(void);

	virtual void draw_init();
	virtual void draw();

// protected:
	Ref MyRef;
	OFFObject* MyMesh;
};

