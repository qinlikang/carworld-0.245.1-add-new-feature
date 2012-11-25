
#ifndef __CW_CAMERA_H_
#define __CW_CAMERA_H_

#include "CarWorldClasses.h"
#include "CWVehicle.h"

//camera classes

class CWCamera : public CWFeature
{
public:
	typedef CWCamera CameraBase;
	virtual ~CWCamera();
	virtual Ref GetRef() = 0;
	virtual void DrawOnScreen();
	virtual void OnMouseDrag(int move_x,int move_y){}
protected:
	static const Point3D X,Y,Z,Up;
};

class FixCam : public CWCamera
{
public:
	FixCam(CWVehicle *Vehicle = NULL, const Point3D &Position = Point3D(0,0,50));
	virtual ~FixCam();
	Ref GetRef();
private:
	Point3D m_Position;
	CWVehicle *m_Vehicle;
};

class FollowCam : public CWCamera
{
public:
	FollowCam(CWVehicle *Vehicle);
	virtual ~FollowCam();
	Ref GetRef();
private:
	CWVehicle *m_Vehicle;
};

class InCarCam : public CWCamera
{
public:
	InCarCam(CWVehicle *Vehicle);
	virtual ~InCarCam();
	Ref GetRef();
	virtual void DrawOnScreen();
public:
	CWVehicle *m_Vehicle;
};

class SateliteCam : public CWCamera
{
public:
	SateliteCam(CWVehicle *Vehicle, REAL height = 10.f);
	virtual ~SateliteCam();
	Ref GetRef();
private:
	CWVehicle *m_Vehicle;
	REAL m_height;
};

class FreeCam : public CWCamera
{
public:
	FreeCam(CWVehicle *Vehicle, REAL sensitivity = 1.0f,REAL radius = 10.f,REAL theta=0.f,REAL phi=90.0f);
	virtual ~FreeCam();
	Ref GetRef();
	virtual void OnMouseDrag(int move_x,int move_y);

	virtual void DrawOnScreen();

	void ZoomIn(){m_radius = REAL(m_radius*0.9);}
	void ZoomOut(){m_radius = REAL(m_radius*1.1);}

private:
	CWVehicle *m_Vehicle;
	REAL m_radius; 
	REAL m_theta;  // 
	REAL m_phi;    // range [10, 170]

	REAL m_sensitivity;
};

#endif //__CW_CAMERA_H_
