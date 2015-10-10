#include "CWCone.h"
#include "OFFObjectPool.h"


CWCone::CWCone(void)
	: CWPointObject(ECT_CONE)
{
	MyRef.X = Point3D(1,0,0);
	MyRef.Y = Point3D(0,1,0);
	MyRef.Z = Point3D(0,0,1);
}


CWCone::~CWCone(void)
{
}

void CWCone::draw()
{
	if(MyMesh)
	{
		MyMesh->draw(MyRef);
	}
}

void CWCone::draw_init()
{
	MyMesh = OFFObjectPool::sharedOFFPool()->getMesh("cone");
}

Point3D CWCone::GetPos() const
{
	return MyRef.Position;

}
