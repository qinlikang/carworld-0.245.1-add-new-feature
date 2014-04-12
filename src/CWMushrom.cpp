#include "CWMushrom.h"


CWMushrom::CWMushrom(void)
	: CWPointObject(ECT_MUSHROOM)
{
	MyRef.X = Point3D(1,0,0);
	MyRef.Y = Point3D(0,1,0);
	MyRef.Z = Point3D(0,0,1);
}


CWMushrom::~CWMushrom(void)
{
}

void CWMushrom::draw()
{
}

void CWMushrom::draw_init()
{

}

Point3D CWMushrom::GetPos() const
{
	
}
