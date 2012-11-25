#include "CWMushrom.h"
#include "OFFObjectPool.h"

CWMushrom::CWMushrom(void)
	: CWColladeFeature(ECT_MUSHROOM)
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
	if(MyMesh)
	{
		MyMesh->draw(MyRef);
	}
}

void CWMushrom::draw_init()
{
	MyMesh = OFFObjectPool::sharedOFFPool()->getMesh("mushroom");
}

Point3D CWMushrom::GetPos() const
{
	return MyRef.Position;
}
