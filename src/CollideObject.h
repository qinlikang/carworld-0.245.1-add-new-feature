#pragma once
#include "H_Geometry.h"
#include "H_Graphics.h"
#include <boost/function.hpp>
#include "CarWorldClasses.h"
#include "Box3D.h"


// every object which can collade should derive this class
class CollideObjectInterface
{
public:

	// interface to implement
	virtual Point3D GetCenterPos()const = 0;
	virtual void GetBox3D(Box3D& box)const = 0;
	virtual bool IsPointInside(const Point3D& pt) const = 0;
};

class CWPointObject:public CWFeature
{
public:
	CWPointObject(){}

	std::string GetTag()const {return m_tag;}
	void SetTag(const string& tag){m_tag = tag;}

	virtual Point3D GetPos()const;

	virtual void draw_init();
	virtual void draw();

	Ref MyRef;
	OFFObject* MyMesh;
private:
	std::string m_tag;
};
