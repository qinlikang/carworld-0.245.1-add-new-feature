
#include "CWCamera.h"
#include <math.h>

#define DEGREE_TO_RAD(x) ((x)/180*3.141593)

//the camera classes
//CLASS CWCamera
CWCamera::~CWCamera() {}
void CWCamera::DrawOnScreen() 
{
	char GlobalPosition[30]={0};
	const Point3D& Pos=GetRef().GetAbsCoord(Point3D(0,0,0));
	sprintf(GlobalPosition,"%5.2f %5.2f %5.2f",Pos.x(),Pos.y(),Pos.z());
	Hgl::WriteText(GlobalPosition, Point2D(.30f,.75f)); //write global position
}
const Point3D CWCamera::X(1,0,0);
const Point3D CWCamera::Y(0,1,0);
const Point3D CWCamera::Z(0,0,1);
const Point3D CWCamera::Up(0,0,1);

//CLASS FixCam
FixCam::FixCam(CWVehicle *Vehicle, const Point3D &Position) :
	m_Position(Position),
	m_Vehicle(Vehicle)
{}

FixCam::~FixCam() {}

Ref FixCam::GetRef()
{
	if (m_Vehicle != NULL)
		return Ref(m_Position,m_Vehicle->MyRef.Position - m_Position,Up);
	else
		return Ref(m_Position,-Z,X);
}

//CLASS FollowCam
FollowCam::FollowCam(CWVehicle *Vehicle) :
	m_Vehicle(Vehicle)
{}

FollowCam::~FollowCam() {}

Ref FollowCam::GetRef()
{
	Point3D Direction(m_Vehicle->MyRef.Speed);
	Direction.z() = 0;
	if (Direction.normalize() == 0) Direction = m_Vehicle->MyRef.GetDirection();
	return Ref(Direction*(-8.f) + m_Vehicle->MyRef.Position + Z*3.f,Direction,Up);
}

//CLASS InCarCam
InCarCam::InCarCam(CWVehicle *Vehicle) :
	m_Vehicle(Vehicle)
{}

InCarCam::~InCarCam() {}

Ref InCarCam::GetRef()
{
	Point3D Location(0.f,-1.0f,.8f);
	return m_Vehicle->MyRef.GetRef(Location);
}

void InCarCam::DrawOnScreen()
{
	glPushMatrix();
	CameraBase::DrawOnScreen();

	Hgl::Translate(Point3D(.75,-.75,0));
	Hgl::Scale(0.2f);
	m_Vehicle->drawInfo();
	glPopMatrix();
}

//CLASS SateliteCam
SateliteCam::SateliteCam(CWVehicle *Vehicle, REAL height) :
	m_Vehicle(Vehicle),
	m_height(height)
{}

SateliteCam::~SateliteCam() {}

Ref SateliteCam::GetRef()
{
	Point3D Y(0,1,0);
	Point3D Z(0,0,1);
	return Ref(m_Vehicle->MyRef.Position+Z*m_height,Z*(-1.f)+Y*.001f,m_Vehicle->MyRef.GetDirection());
}


FreeCam::FreeCam( CWVehicle *Vehicle, REAL sensitivity /*= 1.0f*/,REAL radius /*= 10.f*/,REAL theta/*=0.f*/,REAL phi/*=90.0f*/ )
	: m_Vehicle(Vehicle),m_radius(radius),m_theta(theta),m_phi(phi), m_sensitivity(sensitivity)
{

}

FreeCam::~FreeCam()
{

}

Ref FreeCam::GetRef()
{
	Point3D pos = m_Vehicle->MyRef.Position;
	pos.x() += REAL(    cos(DEGREE_TO_RAD(m_theta))*m_radius*sin(DEGREE_TO_RAD(m_phi))    );
	pos.y() += REAL(    sin(DEGREE_TO_RAD(m_theta))*m_radius*sin(DEGREE_TO_RAD(m_phi))    );
	pos.z() += REAL(    m_radius * cos(DEGREE_TO_RAD(m_phi))       );

	Point3D forward = m_Vehicle->MyRef.Position - pos;
	Point3D upward = Point3D(0,0,1);
	Point3D right = forward ^ upward;
	upward = right ^ forward;

	forward.normalize();
	upward.normalize();
	right.normalize();

	return Ref(pos,forward,upward);
}

void FreeCam::OnMouseDrag( int move_x,int move_y )
{
	m_theta -= move_x * m_sensitivity;
	m_phi -= move_y * m_sensitivity;
	if(m_phi > 170.f) m_phi = 170.f;
	if(m_phi < 10.f) m_phi = 10.f;
}

void FreeCam::DrawOnScreen()
{
	CWCamera::DrawOnScreen();

	Hgl::WriteText("FreeCam [drag]", Point2D(-.80f,.75f)); //write global position
}
