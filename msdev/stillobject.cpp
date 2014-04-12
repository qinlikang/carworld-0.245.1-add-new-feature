#include "stillobject.h"
#include "../src/OFFObjectPool.h"

stillobject::stillobject(std::string tug)
	: CWPointObject(ECT_MUSHROOM)
{
	MyRef.X = Point3D(1,0,0);
	MyRef.Y = Point3D(0,1,0);
	MyRef.Z = Point3D(0,0,1);
	name=tug;
}


stillobject::~stillobject(void)
{
}

void stillobject::draw()
{
	if(MyMesh)
	{
		MyMesh->draw(MyRef);
	}
}

void stillobject::draw_init()
{
	MyMesh = OFFObjectPool::sharedOFFPool()->getMesh(name);
}

Point3D stillobject::GetPos() const
{
	return MyRef.Position;
}
