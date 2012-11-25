#pragma once
#include "H_Geometry.h"
#include "H_Graphics.h"
#include <boost/function.hpp>
#include "CarWorldClasses.h"
#include "Box3D.h"

enum E_COLLADE_TAG
{
	ECT_MUSHROOM,
	ECT_CONE
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

class CWColladeFeature:public CWFeature
{
public:
	CWColladeFeature(E_COLLADE_TAG tag):m_tag(tag)
	{}

	int GetTag()const {return m_tag;}
	void SetTag(E_COLLADE_TAG tag){m_tag = tag;}

	virtual Point3D GetPos()const=0;
private:
	int m_tag;
};