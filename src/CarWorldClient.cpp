
#include "H_Standard.h"
#include "CarWorldClient.h"
#include "CarWorldNet.h"
#include "CWVersion.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "H_Variable.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "CollideObject.h"
#include "OFFObjectPool.h"
#include "CWBeeper.h"
#include "MyDatabase.h"
#include "boost/foreach.hpp"
#include "boost/format.hpp"
#include <ctime>

#define CLIENT_TIMEOUT 200
extern ofstream herr;

static const char * const help_text  =
	"available commands:\n"
	"help       : display this message\n"
	"version    : print version information\n"
	"set        : set a variable to a value\n"
	"exec       : execute configuration file\n"
	"bind       : bind a key to a command\n";

class ExecCFG : public HExecutable
{
public:
	ExecCFG(CarWorldClient *CWC) : CWC(CWC) {}
	void exec(const Command &c)
	{
		if (c.size()==2)
			CWC->execute_cfg(c[1].c_str());
		else
			cout << "usage: exec <file>\n";
	}
	virtual ~ExecCFG() {}
private:
	CarWorldClient *CWC;
};

class JoinServer : public HExecutable
{
public:
	JoinServer(CarWorldClient *CWC) : CWC(CWC) {}
	void exec(const Command &c)
	{
		if ((c.size()!=2) && (c.size()!=3))
			cout << "usage: join <server name> [<port>]\n";
		else
		{
			short port = (c.size()==3) ? (short)atoi(c[2].c_str()) : DEFAULT_PORT;
			CWC->join(c[1].c_str(),port);
		}
	}
	virtual ~JoinServer() {}
private:
	CarWorldClient *CWC;
};

class BindKey : public HExecutable
{
public:
	BindKey(CarWorldClient *CWC) : CWC(CWC) {}
	void exec(const Command &c)
	{
		if (c.size()==3)
			CWC->bind(ToSDLKey(c[1].c_str()), c[2].c_str());
		else
			cout << "usage: bind <key> \"<command line>\"\n";

	}
	void serialize(ostream &out)
	{
		map<SDLKey,string>::iterator I;
		for (I = CWC->KeyBindings.begin() ; I!=CWC->KeyBindings.end(); I++)
		{
			out << "bind " << (*I).first << " \"" << (*I).second << "\"\n";
		}
	}
	virtual ~BindKey() {}
private:
	CarWorldClient *CWC;
};

class Mode : public HExecutable
{
public:
	Mode(CarWorldClient *CWC) : CWC(CWC) {}
	void exec(const Command &c)
	{
		if (c.size()==2)
			CWC->ChangeMode(c[1]);
		else
		{
			cout << "usage: mode \"<mode-name>\"\n";
			cout << "current available modes:\n";
			map<string,CarWorldClient::KeyBindMap>::const_iterator it;
			for(it=CWC->m_ModesMap.begin();it!=CWC->m_ModesMap.end();++it)
			{
				cout<<it->first<<endl;
			}
			CWC->CoutMode();
		}

	}
	virtual ~Mode() {}
private:
	CarWorldClient *CWC;
};
class AddObject : public HExecutable
{
public:
	AddObject(CarWorldClient *CWC) : CWC(CWC) {}
	void exec(const Command &c)
	{
		if (c.size()==2)
			CWC->AddAObject(c[1],0);
		else if(c.size()==3)
			CWC->AddAObject(c[1],atoi(c[2].c_str()));
		else
		{
			cout << "usage: add_obj \"<off-file-tag>\" \"[width]\"\n";
			cout << "current available tags:\n";
			vector<string>::const_iterator it;
			vector<string> tags = OFFObjectPool::sharedOFFPool()->getTags();
			for(it=tags.begin();it!=tags.end();++it)
			{
				cout<<*it<<endl;
			}
		}

	}
	virtual ~AddObject() {}
private:
	CarWorldClient *CWC;
};

class EX_PlaySound : public HExecutable
{
public:
	EX_PlaySound(CarWorldClient *CWC) : CWC(CWC) {}
	void exec(const Command &c)
	{
		if (c.size()==2)
			CWC->PlayASoundOnce(c[1]);
		else
		{
			cout << "usage: playsound \"<sound-name>\"\n";
			cout << "current available sounds:\n";
			vector<string>::const_iterator it;
			vector<string> tags = AudioPlayer::shared_audio()->get_all_sound_name();
			for(it=tags.begin();it!=tags.end();++it)
			{
				cout<<*it<<endl;
			}
		}

	}
	virtual ~EX_PlaySound() {}
private:
	CarWorldClient *CWC;
};

class BoostBindCall : public HExecutable
{
private:
	boost::function<void()> m_Callback;
public:
	template <class T>
	BoostBindCall(T& call_back):m_Callback(call_back){}

	virtual void exec(const Command& c)
	{
		m_Callback();
	}
};

//CLASS CarWorldClient
CarWorldClient::CarWorldClient(bool full_screen,bool not_save) :
	m_Hgl(NULL),
	IsPromptMode(false),
	RealJoystick(NULL),
	FakeJoystick(NULL),
	CurrentJoystick(NULL),
	m_socket(NULL),
	ID(0),
	m_Vehicle(NULL),
	start_time_mark(false),
	m_CarWorld(NULL)
{
	m_window->SetAttrib(640,480,full_screen);
//switch cout to display in the on screen prompt + log
	cout.rdbuf(&hbuf);
	cout << "starting " << name() << " ...\n\n";
	m_CarWorld = new CarWorld(TimeRefreshRate(),landscapename(),not_save);
	m_Vehicle = new CWVehicle(DEFAULT_VEHICLE);
	m_CarWorld->add(m_Vehicle);


	AddColladeObjs(m_CarWorld);
}

void CarWorldClient::draw_init()
{
	cout.rdbuf(&hbuf);

	cout << "initiating graphics...\n";
	m_Hgl = new Hgl(m_window);
	m_Hgl->MakeCurrent();
	cout << endl;

	// set up the off pool and load off files
	OFFObjectPool::sharedOFFPool()->loadOffs();

	m_CarWorld->addKeyboardRecorder("key_recorder",m_window);
	m_CarWorld->draw_init();

	Hgl::ThrowError(); //check to see if everything is OK

	cout << "initiating input...\n";
	RealJoystick = m_window->GetJoystick();
	if (RealJoystick!=NULL)
		cout << RealJoystick->GetDescription() << endl;
	else
		cout << "no joystick support.\n";
	FakeJoystick = new KeyJoystick(m_window);
	CurrentJoystick = FakeJoystick;

	cout << "\ninitiating command line parameters...\n";
	HExecutableSet *m_HExecutableSet = new HExecutableSet();
	m_HExecutableSet->add(new HVar<bool>("gl_finish", &Hgl::SetFinish,&Hgl::GetFinish));
	m_HExecutableSet->add(new HVar<bool>("gl_vertex_arrays", &Hgl::SetVertexArrays,&Hgl::GetVertexArrays));
	m_HExecutableSet->add(new HVar<bool>("gl_ext_compiled_vertex_array",&Hgl::SetExtCompiledVertexArrays,&Hgl::GetExtCompiledVertexArrays));
	m_HExecutableSet->add(new HVar<int>("gl_shadows", &Hgl::SetShadows, &Hgl::GetShadows));
	m_HExecutableSet->add(new HVar<Hgl::Enum>("gl_texturemode",&Hgl::SetTextureMode,&Hgl::GetTextureMode));
	m_HExecutableSet->add(new HVar<bool>("gl_use_opt",&OFFObject::UseOptimizedDraw));
	m_HExecutableSet->add(new HVar<bool>("draw_background",&(m_CarWorld->draw_background)));
	m_HExecutableSet->add(new HVarObj<CarWorldClient,int>("r_mode", this, &CarWorldClient::set_r_mode,&CarWorldClient::get_r_mode));
	m_HExecutableSet->add(new HVarObj<CarWorldClient,bool>("use_joystick",this, &CarWorldClient::set_joystick,&CarWorldClient::get_joystick));
	m_HExecutableSet->add(new HVar<SDLKey>("accel",&(FakeJoystick->up_key)));
	m_HExecutableSet->add(new HVar<SDLKey>("break",&(FakeJoystick->down_key)));
	m_HExecutableSet->add(new HVar<SDLKey>("left",&(FakeJoystick->left_key)));
	m_HExecutableSet->add(new HVar<SDLKey>("right",&(FakeJoystick->right_key)));
	m_HExecutableSet->add(new HVar<SDLKey>("handbreak",&(FakeJoystick->button_key)));


	m_Executables["set"] = m_HExecutableSet;
	m_Executables["join"] = new JoinServer(this);
	m_Executables["bind"] = new BindKey(this);
	m_Executables["reset"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::reset);
	m_Executables["next_camera"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::next_camera);
	m_Executables["record"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::record);
	m_Executables["replay"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::replay);
	m_Executables["off_recorder"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::off_recorder);
	m_Executables["toggleconsole"] = new MethodCall<CarWorldClient>(this,&CarWorldClient::toggleconsole);
	m_Executables["help"] = new MethodCall<CarWorldClient>(this,&CarWorldClient::print_help);
	m_Executables["version"] = new MethodCall<CarWorldClient>(this,&CarWorldClient::print_version);
	m_Executables["exec"] = new ExecCFG(this);
	m_Executables["dump"] = new MethodCall<OFFObject>(&m_Vehicle->Model,&OFFObject::debug_dump);
	// XX addfog
	m_Executables["fogUp"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::fog_up);
	m_Executables["fogDown"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::fog_down);

	m_Executables["zoom_in"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::zoom_in);
	m_Executables["zoom_out"] = new MethodCall<CarWorld>(m_CarWorld,&CarWorld::zoom_out);

	m_Executables["show_box"] =  new BoostBindCall(	boost::bind(&CWVehicle::SetShowBox,m_Vehicle,true));
	m_Executables["no_show_box"] =  new BoostBindCall(	boost::bind(&CWVehicle::SetShowBox,m_Vehicle,false));

	// change mode
	m_Executables["mode"] =  new Mode(this);

	// add object
	m_Executables["add"] =  new AddObject(this);
	m_Executables["del"] =   new MethodCall<CarWorldClient>(this,&CarWorldClient::DeleteNearestObject);
	m_Executables["save"] =   new MethodCall<CarWorldClient>(this,&CarWorldClient::SavePointObjectInfo);
	
	m_Executables["playsound"] = new EX_PlaySound(this);

	// keybindings---------
	// mode:play 
	for(vector<CWBeeper>::iterator it = m_Vehicle->Beepers.begin(); it!= m_Vehicle->Beepers.end();++it)
	{
		m_Executables[it->BeeperName] = new BoostBindCall(boost::bind(&CWBeeper::beep,&(*it)));
		bind(it->getSDLKeyBind(),it->BeeperName.c_str());
	}
	bind(SDLK_TAB,"toggleconsole");
	bind(SDLK_F2, "next_camera");
	bind(SDLK_F3, "reset");
	bind(SDLK_F4, "set use_joystick 1");
	bind(SDLK_F5, "set use_joystick 0");
	bind(SDLK_F6, "record");
	bind(SDLK_F7, "replay");
	bind(SDLK_F8, "off_recorder");
	bind(SDLK_F9, "fogUp");
	bind(SDLK_F10, "fogDown");
	bind(SDLK_MINUS,"zoom_out");
	bind(SDLK_EQUALS,"zoom_in");
	bind(SDLK_n,"no_show_box");
	bind(SDLK_b,"show_box");
	execute_cfg(ConfigurationFileName());
	AddMode("play",KeyBindings);

	// mode: add
	bind(SDLK_m,"add mushroom");
	bind(SDLK_c,"add cone");
	bind(SDLK_b,"add sobj 10");
	bind(SDLK_d,"del");
	bind(SDLK_s,"save");

	AddMode("add",KeyBindings);


	// change mode into play mode
	ChangeMode("play");
	// resource adjust
	OFFObjectPool::sharedOFFPool()->getMesh("mushroom")->Scale(Point3D(0.01f,0.01f,0.01f));
	OFFObjectPool::sharedOFFPool()->getMesh("mushroom")->Translate(Point3D(-1.0f,-1.f,0.f));
	OFFObjectPool::sharedOFFPool()->getMesh("mushroom")->Rotate(Point3D(-PI/2,0,0));

	OFFObjectPool::sharedOFFPool()->getMesh("cone")->Scale(Point3D(0.01f,0.01f,0.01f));
	OFFObjectPool::sharedOFFPool()->getMesh("cone")->Translate(Point3D(-1.0f,-0.3f,0.f));
	OFFObjectPool::sharedOFFPool()->getMesh("cone")->Rotate(Point3D(-PI/2,0,0));

}

CarWorldClient::~CarWorldClient()
{
	//cout.rdbuf(&hbuf);
	//state must be saved while graphics variables are still valid...
	ofstream cfg_file(ConfigurationFileName(), ios::out);
	write_cfg(cfg_file);

	//cout.rdbuf(&hbuf);
	delete FakeJoystick;
	for (map<string,HExecutable *>::iterator I = m_Executables.begin(); I != m_Executables.end() ; I++)
		delete (*I).second;
	delete m_CarWorld;

	m_Hgl->MakeCurrent();
	delete m_Hgl;
	m_Hgl = NULL;

	if (m_socket!=NULL)
	{
	//disconnect from the server
		ClientDisconnect disc;
		disc.ClientNumber = ID;
		disc.DatagramType = CLIENT_DISCONNECT;
		m_socket->Send((void*)&disc,sizeof(disc));
		delete m_socket;
	}
	cout << name() << " terminated.\n";
}

const char *CarWorldClient::name()
{
	/*int majorv = CARWORLD_VERSION/100000;
	int minorv = (CARWORLD_VERSION%100000)/100;
	int minorminorv = CARWORLD_VERSION%100;
	static char CarWorldName[1024];
	sprintf_s(CarWorldName,ARRAY_SIZE(CarWorldName),"Car World v%d.%.3d.%.2d",majorv,minorv,minorminorv);
	return CarWorldName;*/
	return CW_PRODUCT_NAME " " CW_VERSION_STR;
}

void CarWorldClient::toggleconsole()
{
	IsPromptMode = !IsPromptMode;
	m_CarWorld->pause_recorder_timer(IsPromptMode);
}

void CarWorldClient::execute_cfg(const char *FileName)
{
	cout << "executing " << FileName << "...\n";
	char buffer[1024];
	ifstream in(FileName, ios::in);
	if (!in)
	{
		cout << "could not open \"" << FileName << "\"\n";
	}
	else while (!in.eof())
	{
		in.getline(buffer,1024);
		pars_command(buffer);
	}
}

void CarWorldClient::pars_command(const char *value)
{
	Command ACommand(value);
	try
	{
		//if nothing do nothing
		if (ACommand.size() <= 0) return;
		if (m_Executables.find(ACommand[0].c_str())==m_Executables.end())
			cout << "\"" << ACommand[0].c_str() << "\" : is not a valid executable\n";
		else
		{
			m_Executables[ACommand[0].c_str()]->exec(ACommand);
		}
	}
	catch (const exception &E)
	{
		cout << E.what() << endl;
	}
}

void CarWorldClient::bind(SDLKey key,const char *command)
{
	KeyBindings[key] = command;
}

void CarWorldClient::print_help()
{
	cout << help_text;
}

void CarWorldClient::print_version()
{
	cout << name()  << endl;
	Hgl::PrintVersion(cout);
	Hgl::PrintExtentions(cout);
	PrintAllScreenModes(cout);
}

void CarWorldClient::join(const char *host, short port)
{
//close previous socket
	delete m_socket;
//open the socket
	cout << "trying to connect to: " << host << ":" << port << "...\n";
	m_socket = new HSocClient(host,port);
//request to join the game
	ClientRequest request;
	request.ClientNumber = 0;
	request.DatagramType = CLIENT_CONNECT;
	request.VersionNumber = CW_VERSION;
	m_socket->Send((void*)&request,sizeof(request));
//get the confirmation
	ServerConfirm confirm;
	m_socket->Recieve((void*)&confirm,sizeof(confirm));
	ID = confirm.ClientNumber;
//initiate the opponents:
	m_Opponents.clear();
	m_Opponents[ID] = m_Vehicle;
	for (int i=0 ; i<confirm.N ; i++)
	{
		int new_id = confirm.ClientNumbers[i];
		if (new_id!=ID)
		{
			m_Opponents[new_id] = new CWVehicle(DEFAULT_VEHICLE);
			m_CarWorld->add(m_Opponents[new_id]);
			m_Opponents[new_id]->draw_init();
		}
	}
//set non blocking socket
	m_socket->SetBlocking(false);
}

void CarWorldClient::write_cfg(ostream &out)
{
	for (map<string,HExecutable*>::iterator I = m_Executables.begin() ; I!=m_Executables.end() ; I++)
		(*I).second->serialize(out);
}

void CarWorldClient::set_r_mode(int mode)
{
	SetScreenMode(mode,cout);
}

int CarWorldClient::get_r_mode()
{
	//echo not supported yet...
	return 0;
}

void CarWorldClient::set_joystick(bool use_joystick)
{
	if (use_joystick && (RealJoystick!=NULL) && RealJoystick->IsValid())
		CurrentJoystick = RealJoystick;
	else
		CurrentJoystick = FakeJoystick;
	cout << "now using: \"" << CurrentJoystick->GetDescription() << "\" for input\n";
}

bool CarWorldClient::get_joystick()
{
	return (CurrentJoystick==RealJoystick);
}

void CarWorldClient::key_down(SDLKey AHKey, char c)
{
	//cout.rdbuf(&hbuf);
	if (IsPromptMode)
	{
		string ReturnedCommand(hbuf.HitKey(AHKey,c));
		if (!ReturnedCommand.empty())
			pars_command(ReturnedCommand.c_str());
	}
	map<SDLKey,string>::iterator I = KeyBindings.find(AHKey);
	if (I != KeyBindings.end()&&!IsPromptMode)// LX: to disable further process of keydown msg when using console.
		pars_command((*I).second.c_str());
	else if(I != KeyBindings.end()&&IsPromptMode&&I->second=="toggleconsole")// LX: enable toggleconsole msg
		pars_command((*I).second.c_str());
	//else
	//	cout << "\"" << KeyMap.find(AHKey) << "\" key unbound\n";
}

void CarWorldClient::resize(unsigned int width, unsigned int weight)
{
	//cout.rdbuf(&hbuf);
	m_Hgl->MakeCurrent();
	HRect ClientRect = HRect(0,0,(REAL)width,(REAL)weight);
	Hgl::ResizeWindow(ClientRect);
}

void CarWorldClient::SendState()
{
	ClientGamestate state;
	state.ClientNumber = ID;
	state.DatagramType = CLIENT_GAMESTATE;
	state.vehicle = m_Vehicle->GetState();
	m_socket->Send((void*)&state,sizeof(state));
}

bool CarWorldClient::RecieveState()
{
	ServerGamestate state;
	if (m_socket->Recieve((void*)&state,sizeof(state))==sizeof(state))
	{
	//update the states of the opponents
		for (int i=0 ; i<state.N ; i++)
		{
			int new_id = state.ClientNumbers[i];
			if (new_id!=ID)
			{
				map<int,CWVehicle*>::iterator I = m_Opponents.find(new_id);
				if (I==m_Opponents.end()) //the client is not in the list yet
				{
				//add the new vehicle
					m_Opponents[new_id] = new CWVehicle(DEFAULT_VEHICLE);
					m_CarWorld->add(m_Opponents[new_id]);
					m_Opponents[new_id]->draw_init();
				}
				m_Opponents[new_id]->SetState(state.vehicle[i]);
			}
		}
		return true;
	}
	return false;
}

//update the controles of a vehicle with the current position of the joystick
static void UpdateCommand(CWCommand *Command, HJoystick *Joystick)
{
	Joystick->UpdateState();
	Command->Steer = Joystick->GetAxisPos(0);
	Command->GasBrake = Joystick->GetAxisPos(1);
	Command->HandBrake = Joystick->GetButtonPos(0);
	Joystick->SetForceFeedback(Command->SteerFeedBack,-Command->GasBrake);
}

void CarWorldClient::on_idle(unsigned int elapsed_time)
{
	if (m_window != NULL)
	{
		//cout.rdbuf(&hbuf);
		if (m_socket!=NULL)
		{
			static unsigned int time_since_send = 0;
			if (RecieveState())
			{
				SendState();
				time_since_send = 0;
			}
			else
				time_since_send += elapsed_time;
			if (time_since_send>CLIENT_TIMEOUT)
			{
				SendState();
				time_since_send = 0;
			}
		}
		if(!IsPromptMode)
		{
			UpdateCommand(&m_Vehicle->MyCommand, CurrentJoystick);
			m_CarWorld->update(elapsed_time);
			if (start_time_mark==false){
				start_time_mark=true;
				herr<<elapsed_time<<" ";
				time_t curTime;
				struct tm locTime;
				const int TimeStrLen = 26;
				char timeStr[ TimeStrLen ];

				if (    ( -1 != time( &curTime ) )                          // Seconds since 01-01-1970
							   &&  ( 0 == localtime_s( &locTime, &curTime ) )          // Convert to local time
							  &&  ( 0 == asctime_s( timeStr, TimeStrLen, &locTime ) ) // Convert to string
							  )
				{
			   herr << "Date-time is: " << timeStr;
				}
			}
		}// LX: to disable further process of keydown msg when using console.
		draw();
	}
}

void CarWorldClient::draw()
{
	if (m_window != NULL)
	{
		//cout.rdbuf(&hbuf);
		m_Hgl->MakeCurrent();
		m_CarWorld->draw();
		if (IsPromptMode)
			hbuf.draw();
		else
			m_CarWorld->DrawOnScreen();
		Hgl::Finish();
		Hgl::ThrowError();
		Hgl::SwapBuffers();
	}
}

void CarWorldClient::mouse_motion( const SDL_MouseMotionEvent& event )
{
	if(event.state==SDL_PRESSED)
	{
		m_CarWorld->m_Camera->OnMouseDrag(event.xrel,event.yrel);
	}
}

void CarWorldClient::mouse_wheel( const SDL_MouseButtonEvent& event )
{
	if(event.button == SDL_BUTTON_WHEELDOWN)
	{
		m_CarWorld->zoom_in();
	}
	else if(event.button == SDL_BUTTON_WHEELUP)
	{
		m_CarWorld->zoom_out();
	}
}


//xian change

const char * CarWorldClient::landscapename(){
	CppSQLite3Query q = MyDatabase::shared_input_database()->execQuery("select * from landscape;");
	static string sret;
	sret = string(q.fieldValue("name"));
	return sret.c_str();
}
// end of xian change
void CarWorldClient::AddColladeObjs( CarWorld * m_CarWorld )
{
 	ofstream fs("Refs.txt");
 	CWLandscape* landscape = m_CarWorld->m_Landscape;
	/*
 	for(list<WorldBlock>::iterator it = landscape->MyWorldBlocks.begin(); it != landscape->MyWorldBlocks.end(); ++it)
 	{
 		CWMushrom* pMushroom = new CWMushrom;
		pMushroom->MyRef.Position = it->Triangles[0].GetPointByUV(0,0) + Point3D(0,0,1);
		pMushroom->MyRef.Y = it->Triangles[0].GetForwardDirection();
 		pMushroom->MyRef.X = pMushroom->MyRef.Y ^ pMushroom->MyRef.Z;
 		fs<<pMushroom->MyRef.Position<<endl;
 		fs<<pMushroom->MyRef.Y<<endl;
		fs<<pMushroom->MyRef.X<<endl;
		fs<<endl;
		m_CarWorld->add(pMushroom);
 		m_Vehicle->AddToColladeList(pMushroom);
		++it; // so that every other block has different collision
        if(it==landscape->MyWorldBlocks.end())
			break;
		CWCone* pCone = new CWCone;
 		pCone->MyRef.Position = it->Triangles[0].GetPointByUV(0,0)+ Point3D(0,0,1);
 		pCone->MyRef.Y = it->Triangles[0].GetForwardDirection();
		pCone->MyRef.X = pCone->MyRef.Y ^ pCone->MyRef.Z;

 		fs<<pCone->MyRef.Position<<endl;
 		fs<<pCone->MyRef.Y<<endl;
		fs<<pCone->MyRef.X<<endl;
		fs<<endl;
		m_CarWorld->add(pCone);
 		m_Vehicle->AddToColladeList(pCone);
 	}
 	fs.close();
	*/  // commit out the automatic adding. Still read positions from database. I'll add support to write collide object database inside the game.
	
	CppSQLite3Query q = MyDatabase::shared_input_database()->execQuery("select * from CollideObjPosition;");
	while(!q.eof())
	{
		string objtag = q.fieldValue("tag");
		CWPointObject* pObj = new CWPointObject;
		pObj->SetTag(objtag);
		pObj->MyRef.Position = Point3D((REAL)q.getFloatField("x"),(REAL)q.getFloatField("y"),(REAL)q.getFloatField("z"));
		pObj->MyRef.Y = Point3D((REAL)q.getFloatField("forwardx"),(REAL)q.getFloatField("forwardy"),(REAL)q.getFloatField("forwardz"));
		pObj->MyRef.X = Point3D((REAL)q.getFloatField("rightx"),(REAL)q.getFloatField("righty"),(REAL)q.getFloatField("rightz"));
		pObj->Width = (int)q.getFloatField("width");
		m_CarWorld->add(pObj);
		m_Vehicle->AddToColladeList(pObj);

		q.nextRow();
	}

	//Xian added, for distractorse
	// can be either sound play once, or text, shown for duration should be separate data base for location
	// but for now fixed location
	q = MyDatabase::shared_input_database()->execQuery("select * from Distractor;");
	while(!q.eof())
	{
		m_Vehicle->AddToDistractor((double)q.getFloatField("time"),(int)q.getFloatField("type"),q.fieldValue("content"),(int)q.getFloatField("duration"));
		q.nextRow();
	}
	q = MyDatabase::shared_input_database()->execQuery("select * from COM;");
	while(!q.eof())
	{
		if(q.fieldValue("tag")==string("Trigger")){
			m_Vehicle->nirs.init(q.fieldValue("name"));
		//	break;
		}
		if(q.fieldValue("tag")==string("EEG")){
			m_Vehicle->eeg.init(q.fieldValue("name"));
			break;
		}
		q.nextRow();
	}
	q = MyDatabase::shared_input_database()->execQuery("select * from parameters; where tag='biofeedback'");
   	m_Vehicle->eeg.biofeedback=q.fieldValue("value")==string("T");
}

void CarWorldClient::ChangeMode( const string& mode )
{
	if(m_ModesMap.find(mode)==m_ModesMap.end())
	{
		// log error
		cout<<"mode ["+mode+"] not exist, type mode to see what are supported"<<endl;
		return;
	}
	m_CurrentMode=mode;
	KeyBindings=m_ModesMap[mode];
	if(mode=="play")
	{
		m_Vehicle->bFakeCar=false;
	}
	else if(mode=="add")
	{
		// clear all existing object
		m_Vehicle->m_ObjectsToCollade.clear();
		m_Vehicle->bFakeCar=true;
		m_CarWorld->remove_all_collide_objects();
	}
}

void CarWorldClient::AddMode( const string& mode, const KeyBindMap& modemap )
{
	if(mode.empty())return;
	m_ModesMap[mode]=modemap;
	CoutMode();
}

void CarWorldClient::CoutMode()
{
	cout<<"current mode ["+m_CurrentMode+"]";
}

void CarWorldClient::AddAObject( const string& tag ,int width)
{
	Ref& ref = m_Vehicle->MyRef;
	
	CWPointObject* pObj = new CWPointObject;
	pObj->SetTag(tag);
	pObj->MyRef = ref;
	m_CarWorld->add(pObj);
	m_Vehicle->AddToColladeList(pObj);
	pObj->Width=width;
	pObj->draw_init();

	ObjectInfo info;
	info.tag=tag;
	info.position=m_Vehicle->GetCenterPos();
	info.forward=ref.GetDirection();
	info.right=ref.GetX();
	info.pObject=pObj;
	info.width=width;
	m_ObjectList.push_back(info);
}

void CarWorldClient::DeleteNearestObject()
{
	vector<ObjectInfo>::iterator it = m_ObjectList.begin();
	int nNearest=-1;
	float fNearestDist=1000000.0f;
	for(int i=0;it!=m_ObjectList.end();++it,++i)
	{
		float dist = it->position.distance(m_Vehicle->GetCenterPos());
		if(fNearestDist>dist)
		{
			nNearest=i;
			fNearestDist=dist;
		}
	}
	if(nNearest==-1)
		return;
	it = m_ObjectList.begin()+nNearest;
	m_CarWorld->remove(it->pObject);
	m_Vehicle->RemoveFromeCollideList(it->pObject);
	m_ObjectList.erase(it);
}

void CarWorldClient::SavePointObjectInfo()
{
	// write into database
	using namespace boost;
	CppSQLite3DB* db = MyDatabase::shared_input_database();
	db->execDML("delete from CollideObjPosition");

	string sql;
	BOOST_FOREACH(ObjectInfo& info,m_ObjectList)
	{
		format fmt = format("insert into CollideObjPosition values(%d,'%s',%f,%f,%f,%f,%f,%f,%f,%f,%f);")
			%info.width %info.tag %info.position.x() %info.position.y() %info.position.z() %info.forward.x() %info.forward.y() %info.forward.z()
			%info.right.x() %info.right.y() %info.right.z() ;
		sql+=fmt.str();
	}
	db->execDML(sql.c_str());
}

void CarWorldClient::PlayASoundOnce( const string& sound )
{
	AudioPlayer::shared_audio()->get_sound(sound)->play_once();
}
