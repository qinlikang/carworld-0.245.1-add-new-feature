#include "CollideObject.h"
#include "OFFObjectPool.h"

void CWPointObject::draw_init()
{
	MyMesh = OFFObjectPool::sharedOFFPool()->getMesh(m_tag);
}

void CWPointObject::draw()
{
	if(MyMesh)
	{
		MyMesh->draw(MyRef);
	}
}

Point3D CWPointObject::GetPos() const
{
	return MyRef.Position;
}
