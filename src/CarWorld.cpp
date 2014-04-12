#include "H_Standard.h"
#include "CarWorld.h"
#include "H_Graphics.h"
#include <stdio.h>
#include <sstream>
#include "CWMushrom.h"
#include "H_Main.h"

#define FOR_ALL_RECORDERS for(std::map<const char*, CWRecorder*>::iterator it = m_Recorders.begin(); it!=m_Recorders.end();++it)
//CLASS CarWorld:
CarWorld::CarWorld(int TimeRefreshRate, const char *LandscapeFile, bool not_save) :
						m_Landscape(new CWLandscape(LandscapeFile)),
						m_Camera(new FixCam()),
						m_Background(new CWBackground()),
						RealTime(0),
						Frames(0),
						NbTimeClicksPerFrame(0),
						fps(0.f),
						MyTimeRefreshRate(TimeRefreshRate),
						draw_console(false),
						draw_background(true),
						LightDirection(-.6f,-.4f,1.f),
						m_Vehicle(NULL)
{
	m_fogon=false;
	add(m_Landscape);
	add(m_Camera);
	add(m_Background);

	Ref::TimeIncrement = INIT_TIME_INCREMENT;
	m_not_save=not_save;
}
void CarWorld::draw_init()
{
	Hgl::ClearColor(Color(0.3f, 0.3f, 1.0f));
	for (list<CWFeature*>::iterator I=m_Features.begin() ; I!=m_Features.end() ; I++)
		(*I)->draw_init();
}
CarWorld::~CarWorld()
{
	off_recorder();
	for (list<CWFeature*>::iterator I=m_Features.begin() ; I!=m_Features.end() ; I++)
		delete (*I);
}
void CarWorld::add(CWFeature* AFeature)
{
	AFeature->m_CarWorld = this;
	m_Features.push_back(AFeature);
	AFeature->reset();
}

void CarWorld::add(CWVehicle* AVehicle)
{
	add((CWFeature*)AVehicle);
	add(m_Camera = new InCarCam(AVehicle));
	add(m_Recorders["vehicle"] = new VehicleStateRecorder(AVehicle));

	add(new FreeCam(AVehicle));
	add(new FixCam(AVehicle));
	add(new FollowCam(AVehicle));
	add(new SateliteCam(AVehicle));

	m_Vehicle = AVehicle;
}

//precondition: m_Camera is a pointer to a CWFeature in m_Features
void CarWorld::next_camera()
{
	list<CWFeature*>::iterator I = m_Features.begin();
	//find the current camera
	while (*I != m_Camera) I++;
	//find the next one
	I++;
	while (I!=m_Features.end() && dynamic_cast<CWCamera*>(*I)==NULL) I++;
	if (I==m_Features.end())
	{
		I = m_Features.begin();
		while (dynamic_cast<CWCamera*>(*I)==NULL) I++;
	}
	m_Camera = dynamic_cast<CWCamera*>(*I);

}

void CarWorld::reset()
{
	for (list<CWFeature*>::iterator I=m_Features.begin() ; I!=m_Features.end() ; I++)
		(*I)->reset();
}

void CarWorld::update(int ElapsedTimeMs)
{
	RealTime += ElapsedTimeMs;
	Frames += 1;
	//re-evaluare frame rate every 5 seconds
	if (RealTime > MyTimeRefreshRate)
	{
		fps = (Frames*1000.0f)/RealTime;
		//reevaluate Ref::NbTimeClicksPerSec
		//and NbTimeClicksPerFrame
		//so that time always pases at the same speed
		{
			NbTimeClicksPerFrame = 1;
			while (MIN_TIME_CLICKS_SEC>NbTimeClicksPerFrame*fps)
			{
				++NbTimeClicksPerFrame;
			}
			Ref::TimeIncrement = 1/(NbTimeClicksPerFrame*fps);
		}
		Frames = 0;
		RealTime = 0;
	}
	
	for (int i=0 ; i<NbTimeClicksPerFrame ; i++)
	{
		if(m_Recorders["vehicle"]!=NULL&&m_Recorders["vehicle"]->is_replay_and_finished()) return;
		for (list<CWFeature*>::iterator I=m_Features.begin() ; I!=m_Features.end() ; I++)
			(*I)->update();	
	}
}

// XX add fog test
void CarWorld::fog_up(){ m_fogon= !m_fogon;};
void CarWorld::fog_down(){ int i=0;};

void CarWorld::draw()
{
	//cout << "clearing screen...\n";
	int ToBeCleared = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
	if (Hgl::GetShadows()>0)
		ToBeCleared |= GL_STENCIL_BUFFER_BIT;
	glClear(ToBeCleared);

	// XX add fog
	if (m_fogon){
	glEnable(GL_FOG);
	float FogCol[3]={0.8f,0.8f,0.8f}; // Define a nice light grey
    glFogfv(GL_FOG_COLOR,FogCol);     // Set the fog color
	glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
    glFogf(GL_FOG_START, 5.f);
    glFogf(GL_FOG_END, 20.f);
	}
	else glDisable(GL_FOG);

	//cout << "looking from the camera...\n";
	glLoadIdentity();
	Hgl::LookFrom(m_Camera->GetRef());

	//cout << "setting the light source...\n";
	Hgl::SetLightSource(LightDirection);

	//cout << "drawing the background...\n";
	if (draw_background)
		m_Background->draw();

	//cout << "drawing the landscape...\n";
	m_Landscape->draw();

	//cout << "draw all the other features...\n";
	for (list<CWFeature*>::iterator I = m_Features.begin() ; I!=m_Features.end() ; I++)
	{
		if (dynamic_cast<CWLandscape*>(*I)==NULL && dynamic_cast<CWBackground*>(*I)==NULL)
			(*I)->draw();
	}
}
void CarWorld::DrawOnScreen()
{
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1,1,-1,1, -1.0, 1.0 );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	ostringstream FPSCaption;
	FPSCaption << fps;
	/*
	Hgl::SetColor(White);
	Hgl::WriteText(FPSCaption.str().c_str(), Point2D(-.25,.75)); //write fps and speed
	

	//draw car info
	m_Camera->DrawOnScreen();
	//draw recorder info
	if(m_Recorders["vehicle"]!=NULL)
		m_Recorders["vehicle"]->draw_on_screen();

	// test for u,v
	WorldBlock::MyTriangle* pTri;
	if(m_Landscape->LastContactBlock!=m_Landscape->MyWorldBlocks.end() 
		&& (pTri=m_Landscape->LastContactBlock->PreviousTriangle)!=NULL)
	{

		double u,v;
		Point3D carPos;
		InCarCam* in_car_cam = dynamic_cast<InCarCam*>(m_Camera);
		if(in_car_cam!=NULL)
		{
			carPos = in_car_cam->m_Vehicle->MyRef.Position;
			Point2D carPlanePos(carPos.x(),carPos.y());
			if(pTri->GetInsidePointUVParameter(carPlanePos,u,v))
			{
				{
					char UVindicator[30]={0};
					sprintf(UVindicator,"u:%5.2f, v:%5.2f",u,v);
					Hgl::WriteText(UVindicator, Point2D(.0f,.0f)); //write global position
				}
				{
					Point3D pt = pTri->GetPointByUV(u,v);
					char RoadPoint[50]={0};
					sprintf(RoadPoint,"%5.2f,%5.2f,%5.2f !! %5.2f,%5.2f,%5.2f",pt.x()	,pt.y()	,pt.z(), carPos.x()	,carPos.y()	,carPos.z()	);
					Hgl::WriteText(RoadPoint, Point2D(-.2f,-.2f)); //write global position
				}
			}
		}
	}
	*/

	char BonusHint[30]={0};
	sprintf(BonusHint,"Score: %d | Fail:%d",m_Vehicle->m_MushroomCnt,m_Vehicle->m_ConeCnt);
	Hgl::WriteText(BonusHint, Point2D(.0f,-.90f)); //write global position

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void CarWorld::record()
{
	FOR_ALL_RECORDERS
	{
		CWRecorder* R = it->second;
		R->set_state(CWRecorder::ERS_Record);
	}
}

void CarWorld::replay()
{
	FOR_ALL_RECORDERS
	{
		CWRecorder* R = it->second;
		R->set_state(CWRecorder::ERS_Replay);
	}
}

void CarWorld::off_recorder()
{
	if ( m_not_save) return;
	m_Recorders["vehicle"]->m_strOtherMsg= "FileSaved!! ";
	FOR_ALL_RECORDERS
	{
		CWRecorder* R = it->second;
		R->dump();
		R->set_state(CWRecorder::ERS_OFF);
	}
}

void CarWorld::pause_recorder_timer( bool pause/*=true*/ )
{
	if(pause)
	{
		FOR_ALL_RECORDERS
		{
			CWRecorder* R = it->second;
			R->m_Timer.pause();
		}
	}
	else
	{
		FOR_ALL_RECORDERS
		{
			CWRecorder* R = it->second;
			R->m_Timer.resume();
		}
	}}

void CarWorld::zoom_in()
{
	FreeCam* cam = dynamic_cast<FreeCam*>(m_Camera);
	if(cam)
	{
		cam->ZoomIn();
	}
}

void CarWorld::zoom_out()
{
	FreeCam* cam = dynamic_cast<FreeCam*>(m_Camera);
	if(cam)
	{
		cam->ZoomOut();
	}
}

void CarWorld::remove( const CWFeature* AFeature )
{
	for(list<CWFeature*>::iterator it=m_Features.begin(); it!= m_Features.end();++it)
	{
		if(AFeature==(*it))
		{
			delete (*it);
			m_Features.erase(it);
			return;
		}
	}
}


void CarWorld::addKeyboardRecorder( const char* recorder_name,HWindow* hwindow )
{
	add(m_Recorders[recorder_name] = new KeyboardInputRecorder(hwindow));
}
