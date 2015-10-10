
#include <stdio.h>

#include "CarWorldClasses.h"
#include "CarWorld.h"
#include "H_Object.h"
#include "H_Graphics.h"

//CLASS CWFeature
CWFeature::~CWFeature() {}
void CWFeature::reset() {}
void CWFeature::update() {}
void CWFeature::draw_init() {}
void CWFeature::draw() {}


//CALSS CWBackground
CWBackground::CWBackground()
{
}
CWBackground::~CWBackground()
{}
void CWBackground::draw_init()
{
	m_OFFObject.readfile("data/Landscape/Background.off");
	//m_OFFObject.Center();
	m_OFFObject.Scale(Point3D(50.f,50.f,50.f));
}
void CWBackground::draw()
{
	Ref BackgroundRef = m_CarWorld->m_Camera->GetRef();
	BackgroundRef.Position = Point3D(0,0,0);

	glPushMatrix();
	glLoadIdentity();
	Hgl::LookFrom(BackgroundRef);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL); 
	m_OFFObject.draw();
	glEnable(GL_COLOR_MATERIAL); 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

//CLASS CWLandscape
CWLandscape::CWLandscape(const char *file_name)
{
	try
	{
		LastContactBlock = MyWorldBlocks.begin();
		char BlockName[1024];

		Ref CurrentRef;
		CurrentRef.reset();
		ifstream infile;
		OpenDataFile(infile,file_name);

		unsigned int NbWorldBlocks;
		infile >> NbWorldBlocks;
		infile.getline(BlockName, 1024);
		for (unsigned int i=0 ; i<NbWorldBlocks ; i++)
		{
			infile.getline(BlockName, 1024);
			string ThisPath = PathOf(file_name);
			string BlockFile = ThisPath+'/'+BlockName;
			MyWorldBlocks.push_back(WorldBlock());
			MyWorldBlocks.back().ReadFile(BlockFile.c_str());
			MyWorldBlocks.back().Place(CurrentRef);
			CurrentRef = MyWorldBlocks.back().EndPosition;
		}
	}
	catch (const HException &E)
	{
		MyWorldBlocks.clear();
		throw HException(string("problem with CWLandscape object \"")+TitleOf(file_name)+("\":\n")+E.what());
	}
}

CWLandscape::~CWLandscape()
{
	for(list<WorldBlock>::iterator I=MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
		I->GetContact(Point3D());
}

void CWLandscape::draw_init()
{
	for(list<WorldBlock>::iterator I=MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
		(*I).InitOffObject();
}

void CWLandscape::draw()
{
	glDisable(GL_CULL_FACE);
	for (list<WorldBlock>::iterator I = MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
		(*I).draw();
	glEnable(GL_CULL_FACE);
}
Contact CWLandscape::GetContact(const FixedVector &AVector)
{
	Contact tmp;
	for (list<WorldBlock>::iterator I = MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
	{
		tmp = (*I).GetContact(AVector);
		if (tmp.Found)
			return tmp;
	}
	return tmp;
}
Contact CWLandscape::GetContact(const Point3D &APoint)
{
	Contact tmp;
	for (list<WorldBlock>::iterator I = MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
	{
		tmp = (*I).GetContact(APoint);
		if (tmp.Found)
			return tmp;
	}
	return tmp;
}

Contact CWLandscape::GetFixedVectorContact( const FixedVector &AVector )
{
	Contact tmp;
	for (list<WorldBlock>::iterator I = MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
	{
		tmp = (*I).GetContact(AVector);
		if (tmp.Found)
		{
			if(tmp.Position.distance(AVector.Position)<AVector.Value.norm()+EPSILON)
			{
				LastContactBlock = I;
				LastContactTriangle = I->PreviousTriangle;
				Point2D pt(tmp.Position.x()	,tmp.Position.y());
				LastContactTriangle->GetInsidePointUVParameter(pt,LastU,LastV);
				return tmp;
			}
			else
				tmp.Found=false;
		}
	}
	return tmp;
}

Contact CWLandscape::GetPointContact( const Point3D &APoint,double distance )
{
	Contact tmp;
	for (list<WorldBlock>::iterator I = MyWorldBlocks.begin() ; I!=MyWorldBlocks.end() ; I++)
	{
		tmp = (*I).GetContact(APoint);
		if (tmp.Found)
		{
			if(tmp.Position.distance(APoint)<distance+EPSILON)
			{
				LastContactBlock = I;
				LastContactTriangle = I->PreviousTriangle;
				Point2D pt(tmp.Position.x()	,tmp.Position.y());
				LastContactTriangle->GetInsidePointUVParameter(pt,LastU,LastV);
				return tmp;
			}
			else
				tmp.Found=false;
		}
	}
	return tmp;
}
