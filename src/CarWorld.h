#ifndef __CAR_WORLD_H_
#define __CAR_WORLD_H_

#include "H_Standard.h"
#include "CarWorldClasses.h"
#include "CWCamera.h"
#include "CWRecorder.h"

//#define DEFAULT_VEHICLE "data/Vehicles/82porsch.vhc"
#define DEFAULT_VEHICLE "data/Vehicles/truck.vhc"
//#define DEFAULT_LANDSCAPE "data/Landscape/landscape.txt"

#define EARTH_GRAVITY 9.8f

//World caracteristics:
#define INIT_TIME_INCREMENT 0.0001f
#define MIN_TIME_CLICKS_SEC 40.0
class HWindow;
class CarWorld
{
public:
	CarWorld(int TimeRefreshRate, const char *LandscapeFile,bool not_save);
	~CarWorld();
	void add(CWFeature* AFeature);
	void addKeyboardRecorder(const char* recorder_name,HWindow* hwindow);
	void remove(const CWFeature* AFeature);
	void add(CWVehicle* AVehicle);
	void next_camera();
	void record();
	void replay();
	void pause_recorder_timer(bool pause=true);
	void off_recorder();
	void DrawOnScreen();

	void reset();
	void update(int ElapsedTimeMs);
	void draw_init();
	void draw();

	// XX add fog test
	void fog_up();
	void fog_down();
	bool m_fogon;

	void zoom_in();
	void zoom_out();

	void remove_all_collide_objects();

public:
	CWLandscape *m_Landscape;
	CWCamera *m_Camera; //the current camera
	std::map<const char*,CWRecorder*> m_Recorders;

	CWVehicle* m_Vehicle;
private:
	CWBackground *m_Background;

	list<CWFeature*> m_Features;

	//to do with time scaling:
	int RealTime; //in ms since start
	int Frames;
	int NbTimeClicksPerFrame;
	REAL fps;
	int MyTimeRefreshRate;
public:
	bool draw_console;
	bool draw_background;
	Point3D LightDirection;
	bool m_not_save;
};

#endif //__CAR_WORLD_H_
