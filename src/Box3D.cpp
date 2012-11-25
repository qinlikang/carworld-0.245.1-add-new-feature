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
