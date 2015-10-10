#pragma  once
#include "H_Geometry.h"
#include "H_Graphics.h"

struct Box3D
{
	Point3D A1,B1,C1,D1,A2,B2,C2,D2;
	REAL planeUp[4];
	REAL planeBottom[4];
	REAL planeLeft[4];
	REAL planeRight[4];
	REAL planeFore[4];
	REAL planeBack[4];

	Box3D()
	{
		for(unsigned i=0; i<4;++i)
		{
			planeUp[i]=planeBottom[i]=planeLeft[i]=planeRight[i]=planeFore[i]=planeBack[i]=0.0f;
		}
	}

	Box3D(const Point3D& PA1,const Point3D& PB1,const Point3D& PC1,const Point3D& PD1,
		const Point3D& PA2,const Point3D& PB2,const Point3D& PC2,const Point3D& PD2)
		: A1(PA1),A2(PA2), B1(PB1),B2(PB2), C1(PC1),C2(PC2), D1(PD1),D2(PD2)
	{
		Init();
	}

	Box3D& operator=(const Box3D& box)
	{
		for(unsigned i=0; i<4; ++i)
		{
#define COPY_SNIP(NAME,I)	NAME[I]=box.NAME[I]

			COPY_SNIP(planeUp,i);
			COPY_SNIP(planeBottom,i);
			COPY_SNIP(planeLeft,i);
			COPY_SNIP(planeRight,i);
			COPY_SNIP(planeFore,i);
			COPY_SNIP(planeBack,i);

#undef COPY_SNIP
		}
#define COPY_SNIP(NAME) NAME=box.NAME
		COPY_SNIP(A1);
		COPY_SNIP(A2);
		COPY_SNIP(B1);
		COPY_SNIP(B2);
		COPY_SNIP(C1);
		COPY_SNIP(C2);
		COPY_SNIP(D1);
		COPY_SNIP(D2);
#undef COPY_SNIP
		return *this;
	}

	void Init()
	{
		for(unsigned i=0; i<4;++i)
		{
			planeUp[i]=planeBottom[i]=planeLeft[i]=planeRight[i]=planeFore[i]=planeBack[i]=0.0f;
		}

		findPlane(planeBottom,A1,B1,C1);
		findPlane(planeUp,A2,B2,C2);
		findPlane(planeFore,A1,B1,B2);
		findPlane(planeBack,D1,C1,C2);
		findPlane(planeLeft,A1,D1,D2);
		findPlane(planeRight,B1,C1,C2);

	}

	bool IsPtInside(const Point3D& pt)const
	{
		if(SubstituteIntoPlane(planeUp,pt)*SubstituteIntoPlane(planeBottom,pt)>=0) return false;
		if(SubstituteIntoPlane(planeLeft,pt)*SubstituteIntoPlane(planeRight,pt)>=0) return false;
		if(SubstituteIntoPlane(planeFore,pt)*SubstituteIntoPlane(planeBack,pt)>=0) return false;
		return true;
	}


	template<class Functor>
	void VisitAllPoint(const Functor& visitor)
	{
		visitor(A1);
		visitor(A2);
		visitor(B1);
		visitor(B2);
		visitor(C1);
		visitor(C2);
		visitor(D1);
		visitor(D2);
		Init();
	}

	Point3D GetCenter()const{return (A1+C2)/2;}

	void DrawFrame()const;
};

bool PlaneInsectSegment(const REAL plane[4],const Point3D& pt1,const Point3D& pt2,Point3D& insect);
