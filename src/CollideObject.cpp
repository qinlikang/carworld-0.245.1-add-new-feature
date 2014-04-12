#include "CollideObject.h"
#include "OFFObjectPool.h"
#include "CarWorldClient.h"

extern CarWorldClient* pCWC;
void CWPointObject::draw_init()
{
	MyMesh = OFFObjectPool::sharedOFFPool()->getMesh(m_tag);
	ScriptFile = OFFObjectPool::sharedOFFPool()->getScriptName(m_tag);
}


void CWPointObject::draw()
{
	if(pCWC->m_CurrentMode=="add")
		drawColliding();
	if(MyMesh)
	{
		MyMesh->draw(MyRef);
	}
}


void CWPointObject::drawColliding()
{
	Point3D pt1=MyRef.Position+MyRef.GetX()*(float)Width/2;
	Point3D pt2=MyRef.Position-MyRef.GetX()*(float)Width/2;

	glPushMatrix();

	glPointSize(5.0);   //设置点大小，不能再glBegin和glEnd之间
	glLineWidth(3.0);
	glBegin(GL_POINTS);  //mode为GL_POINTS
	glColor3f(1.0,1.0,1.0); //设置点颜色
	glVertex3fv(MyRef.Position.p);
	glEnd();
	glBegin(GL_LINES);
	glVertex3fv(pt1.p);
	glVertex3fv(pt2.p);
	glEnd();

	glPopMatrix();	
}


Point3D CWPointObject::GetPos() const
{
	return MyRef.Position;
}

static bool IsFaceInsectSegment(const REAL face[4],const REAL bound1[4],const REAL bound2[4],const Point3D& pt1, const Point3D& pt2)
{
	Point3D insect;
	if(!PlaneInsectSegment(face,pt1,pt2,insect))
		return false;
	float bd1=SubstituteIntoPlane(bound1,insect);
	float bd2=SubstituteIntoPlane(bound2,insect);
	if(bd1*bd2>=0)return false;
	return true;
}

bool CWPointObject::IsCollideWithBox( const Box3D& box ) const
{
	if(Width==0)
	{
		return box.IsPtInside(GetPos());
	}
	else
	{
		// do line segment collision detect
		Point3D pt1=MyRef.Position+MyRef.GetX()*(float)Width/2;
		Point3D pt2=MyRef.Position-MyRef.GetX()*(float)Width/2;
		if(box.IsPtInside(pt1)) return true;
		if(box.IsPtInside(pt2)) return true;
		
		if(IsFaceInsectSegment(box.planeFore,box.planeLeft,box.planeRight,pt1,pt2)) return true;
		if(IsFaceInsectSegment(box.planeLeft,box.planeFore,box.planeBack,pt1,pt2)) return true;
		if(IsFaceInsectSegment(box.planeBack,box.planeLeft,box.planeRight,pt1,pt2)) return true;

		return false;
	}
}
