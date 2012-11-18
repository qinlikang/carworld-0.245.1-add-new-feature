#include "CWMushrom.h"
#include "OFFObjectPool.h"

CWMushrom::CWMushrom(void)
{
}


CWMushrom::~CWMushrom(void)
{
}

void CWMushrom::draw()
{
	if(MyMesh)
		MyMesh->draw(MyRef);
}

void CWMushrom::draw_init()
{
	MyMesh = OFFObjectPool::sharedOFFPool()->getMesh("mushroom");
}
