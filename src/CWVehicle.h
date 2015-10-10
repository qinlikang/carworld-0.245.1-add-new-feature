
#ifndef __CW_VEHICLE_H_
#define __CW_VEHICLE_H_

#include "CarWorldClasses.h"
#include "CWBeeper.h"
#include "NirsTrigger.h"
#include "Brain.h"
#include "CollideObject.h"
#include <fstream>
#include <vector>
class CWVehicle;

//describes the state of the vehicle controls
struct CWCommand
{
	CWCommand();
	REAL GasBrake;
	REAL Steer;
	REAL SteerFeedBack;
	bool HandBrake;
};

class Wheel
{
public:
	Wheel(
		const Point3D &RelPos = Point3D(),
		REAL SpringStiffness = 30000,
		REAL MaxTravel = -0.5f,
		REAL MaxLoad = 5000,
		REAL WheelRadius = 0.4f,
		REAL DamperCompression = 2000,
		REAL DamperRebound = 2000,
		REAL Grip = 1.0f,
		REAL AccelFactor = 4000,
		REAL BrakeFactor = 4000,
		REAL SteerFactor = .5f,
		bool LockUp = true
	);
	void draw_init();
	void reset();
	void Set(
		const Ref &ARef,
		CWCommand &ACommand,
		CarWorld &CW //the world to interact with
	);
	//void Set(Ref &ARef);
	Point3D CalcForce(/*REAL &ForceFeedback*/);
	FixedVector &GetRay();
	void draw();

//constant for this wheel:
public:
	OFFObject Model;
	string ModelFile;
	Point3D ModelPos;
	Point3D ModelScale;

	Point3D RelPos;
	REAL SpringStiffness;
	REAL MaxTravel; ///< do not understand this one, a negative float use to generate Ray's Pos
	REAL MaxLoad; //load beyond which the tyre is no longer efficient
	REAL WheelRadius;
	REAL DamperCompression;
	REAL DamperRebound;
	REAL Grip;
	REAL AccelFactor;
	REAL BrakeFactor;
	REAL SteerFactor;
	bool LockUp;

//variables used for computation:
public:
	REAL Thrust;
	REAL Angle;
	bool Lock;
	Point3D Acceleration;
	Ref MyRef;
	Point3D LandSpeed, LandDirection;
	FixedVector Ray; //the ray used to determine the impact with the ground
	REAL Gs;
	REAL Travel, DeltaTravel;
	Contact MyContact;
};

//structure to be sent over the network
struct CWVehicleState
{
	Ref m_Ref;
	CWCommand m_Command;
};
struct CCrashRec
{
	double time;
	int type; // 0 crash, 1 cone, 2 mushroom, 
};
struct CDistractor
{
	double time;
	int type; // 0 music, 1 text
	string str;
	int duration; // 0 music no meaning, but text will disappear later
};
class CWVehicle : public CWFeature, public CollideObjectInterface
{
public:
	CWVehicle(const char *name);
	void load(const char *name);
	virtual ~CWVehicle();
	void reset();
	void reset_to_fall_block();
	bool is_vehicle_out_of_road();
	void update();
	REAL GetSpeed() const; //returns speed in kph

	//update every thing attached to the car
	//ie: roll cage position, wheels, ...
	void UpdateCWVehicleParams();

	void draw_init();
	void draw();
	void drawShape();
	void ProjectShadow(const Point3D &LightDirection);
	void drawInfo();

	CWVehicleState GetState();
	void SetState(CWVehicleState &state);

	void LocalPosToGlobalPos(Point3D& pt)const;

	// for collision test
	Point3D GetCenterPos()const;
	void GetBox3D(Box3D& box)const;
	bool IsPointInside(const Point3D& pt) const;
	void CollisionTest();
	bool bFakeCar;// if set true, we only draw wheels, and not do collision

	// for box
	void SetShowBox(bool show){m_bShowBox = show;}
public:
//read in the configuration file
	string ModelFile;
	Point3D ModelPos;
	Point3D ModelScale;

	Point3D InitPos;
//mass:
	REAL Mass; //kg
	REAL Weight; //N
	REAL MassDistrib; //kg.m
//to do with drive train:
	REAL RedLine; //REV.min-1
	REAL MaxTorque; //N.m
	REAL Friction; //N.s2.m-2

//the state of the commands of the vehicle
	CWCommand MyCommand;
	DriveTrain MyDriveTrain;
	InertRef MyRef;
	vector<Wheel> Wheels;
	OFFObject Model;
	vector<CWBeeper> Beepers;

	WorldBlock* LastHitBlock;

	// bonus and punish
	vector<CWPointObject*> m_ObjectsToCollade;
	void AddToColladeList(CWPointObject* object);
	void RemoveFromeCollideList(CWPointObject* object);
	std::map<std::string,unsigned int> m_HitCount;
	std::map<CWPointObject*,bool> m_ObjIsColliding;

	// box
	bool m_bShowBox;

	// xian added record events
	std::ofstream outf;
	std::vector <CCrashRec> Crashrecord;
	double elapsed_time; //in ms since start
	void updateTime(double t);
	std::vector <CDistractor *> distractor;
	int distractorN;
	void AddToDistractor( const double t, const int tp, const char *c , const int dur);
	// add a time so that one colliion does not continuously happen, current solution is to remove the object, 
	// but the second lapse, the drive wont see them anymore
	double collisionTime;

	NirsTrigger nirs; // for triggering
	Brain eeg;
};

#endif //__CW_VEHICLE_H_
