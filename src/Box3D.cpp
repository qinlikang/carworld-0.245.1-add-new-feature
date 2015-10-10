#include "Box3D.h"
#include <gl/GL.h>
void Box3D::DrawFrame() const
{
	glDisable(GL_LIGHTING);			

#define RENDER(A,B,C,D) 		glBegin(GL_LINE_LOOP); glVertex3fv(A.p);glVertex3fv(B.p);glVertex3fv(C.p);glVertex3fv(D.p);glEnd();

	RENDER(A1,B1,C1,D1);
	RENDER(A2,B2,C2,D2);

	RENDER(A1,A2,B2,B1);
	RENDER(C1,C2,D2,D1);

	RENDER(A1,A2,D2,D1);
	RENDER(C1,C2,B2,B1);
#undef RENDER
	glEnable(GL_LIGHTING);
}

bool PlaneInsectSegment(const REAL plane[4],const Point3D& pt1,const Point3D& pt2,Point3D& insect )
{
	REAL val1 = SubstituteIntoPlane(plane,pt1);
	REAL val2 = SubstituteIntoPlane(plane,pt2);
	
	if(val1*val2>0)
		return false;

	Point3D n(plane[0],plane[1],plane[2]);
	Point3D v = pt1-pt2;
	float _lambda = val1/(v*n);
	assert(_lambda>=0&&_lambda<=1);
	insect = (1-_lambda)*pt1+(_lambda)*pt2;

	return true;
}
