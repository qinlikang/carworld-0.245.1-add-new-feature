
#ifndef _CAR_WORLD_CLIENT_H_
#define _CAR_WORLD_CLIENT_H_

namespace std {}
using namespace std;

#include "H_Main.h"

#include "H_Input.h"
#include "H_Prompt.h"
#include "H_Variable.h"
#include "H_Socket.h"
#include "CarWorld.h"

#include <queue>

class CarWorldClient : public HglApplication
{
public:
	CarWorldClient(bool full_screen,bool not_save);
	virtual ~CarWorldClient();
//overrided inherited methods
	const char* name();
	
	void draw_init();

	void key_down(SDLKey AHKey, char c);
	void resize(unsigned int width, unsigned int height);
	void mouse_motion(const SDL_MouseMotionEvent& event);
	void mouse_wheel(const SDL_MouseButtonEvent& event);

	void on_idle(unsigned int elapsed_time);

	void draw();

//methods particular to CarW, executable from the command line
	void toggleconsole();

	void set_r_mode(int mode); //screen mode manipulation (not 100% yet...)
	int get_r_mode();

	void set_joystick(bool use_joystick);
	bool get_joystick();

	void exec_file(const string& FileName);
	void exec_command(const string& value);

	void bind(SDLKey key,const char *command);

	void print_help();
	void print_version();

	void init_script_engine();

//connect to a network server
	void join(const char *host, short port);

	void write_cfg(ostream &out);

//add mode support, by LX, in different mode, we have different sets of keybind;
	typedef map<SDLKey,string> KeyBindMap;
	void ChangeMode(const string& mode);
	map<string,string> m_Modes;
	string m_CurrentMode;
	void AddMode(const string& mode, const string& script);
	void CoutMode();
//add object 
	struct ObjectInfo
	{
		string tag;
		Point3D position;
		Point3D forward;
		Point3D right;
		int width;
		CWPointObject* pObject;
	};
	vector<ObjectInfo> m_ObjectList;
	void AddAObject(const string& tag,int width);// add object at current position
	void DeleteNearestObject();
	void ClearObjects();
	void SavePointObjectInfo(); // save the object list into the database;
	
	// play a sound
	void PlayASoundOnce(const string& sound);

	string m_CurScriptDirectory;// the directory of current running script
public:
	map<SDLKey,string> KeyBindings;
private:
	// start_time_mark;
	bool start_time_mark;
	//xian landscape name in db
	const char *landscapename();
	// end xian
//graphics
	Hgl_streambuf hbuf;
	Hgl *m_Hgl;
//command line
	bool IsPromptMode;
	map<string,HExecutable*> m_Executables;


//input
	HJoystick *RealJoystick;
	KeyJoystick *FakeJoystick;
	HJoystick *CurrentJoystick;
//network
	HSocClient *m_socket;
	int ID;
	void SendState();
	bool RecieveState();
	void AddColladeObjs( CarWorld * m_CarWorld );
	map<int,CWVehicle*> m_Opponents;
//CarWorld
public:
	CWVehicle *m_Vehicle;
	CarWorld *m_CarWorld;
};

#endif //_CAR_WORLD_CLIENT_H_
