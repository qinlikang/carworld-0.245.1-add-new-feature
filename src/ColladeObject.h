#pragma once
#include "H_Geometry.h"
#include "H_Graphics.h"
// MACRO to distinguish different objects
#define TAG_CAR			0
#define TAG_MUSHROOM	1
#define TAG_CONE		2

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
		: A1(PA1),A2(PA2), B1(PB1),B2(PB2), C1(PC1),C2(PC2), D1(PD1),D2(PD2),
	{
		CalcEachPlane();
	}
	void CalcEachPlane()
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
		if(calcPlane(planeUp,pt)*calcPlane(planeBottom,pt)>=0) return false;
		if(calcPlane(planeLeft,pt)*calcPlane(planeRight,pt)>=0) return false;
		if(calcPlane(planeFore,pt)*calcPlane(planeBack,pt)>=0) return false;
		return true;
	}
};

// every object which can collade should derive this class
class ColladeObjectInterface
{
public:

	// interface to implement
	virtual Point3D GetCenterPos()const = 0;
	virtual void GetBox3D(Box3D& box)const = 0;
	virtual bool IsPointInside(const Point3D& pt) const = 0;
};

