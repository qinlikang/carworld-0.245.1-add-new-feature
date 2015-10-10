#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include "H_Object.h"
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT =800;
const int SCREEN_BPP = 32;
SDL_Surface *screen;
bool windowed;
bool windowOK;
OFFObject* g_pOffObj=NULL;
Ref g_Camera;

class HAppData
{
public:
	HAppData();
public:
};

HAppData::HAppData() {}
HWindow::~HWindow() {}
HJoystick::~HJoystick(){}

class HSDLWindow : public HWindow
{
public:
	HSDLWindow();
	virtual ~HSDLWindow();
	virtual void SetAttrib(int width, int height, bool fullscreen);
	bool IsPressed(SDLKey k);
	const char* GetKeyboardDescription();
	HJoystick* GetJoystick();
	virtual void MakeCurrent();
	virtual void SwapBuffers();
public:
};
HSDLWindow::HSDLWindow()
{}

HSDLWindow::~HSDLWindow()
{
}

const char *HSDLWindow::GetKeyboardDescription()
{
	return "SDL keyboard";
}

bool HSDLWindow::IsPressed(SDLKey k)
{
	Uint8* keys = SDL_GetKeyState(NULL);
	if (keys==NULL)
		return false;
	return keys[k]!=0;
}

HJoystick* HSDLWindow::GetJoystick()
{
	return NULL;
}

void HSDLWindow::MakeCurrent()
{
}

void HSDLWindow::SwapBuffers()
{
	SDL_GL_SwapBuffers();
}

void HSDLWindow::SetAttrib(int width, int height, bool fullscreen)
{
	m_width = width;
	m_height = height;
	m_fullscreen = fullscreen;
}


//default implementations for the application functions:
//CLASS HApplication
HApplication::HApplication() : m_data(new HAppData) {}
HApplication::~HApplication() {delete m_data;}
const char* HApplication::name() {return "generic HApplication";}
void HApplication::on_idle(unsigned int elapsed_time) {}

int HApplication::TimeRefreshRate()
{
	return 1000;
}

//CLASS HglApplication
HglApplication::HglApplication() : m_window(new HSDLWindow()) {}
HglApplication::~HglApplication() {delete m_window;}
void HglApplication::draw_init() {}
void HglApplication::key_down(SDLKey AHKey, char c) {}
void HglApplication::resize(unsigned int width, unsigned int height) {}
void HglApplication::draw() {}


//START Screen managment functions
unsigned int HglApplication::GetScreenMode()
{
	//BUG implement this function
	cout << "WARNING: this function is not yet implemented." << endl;
	return 0;
}

bool HglApplication::SetScreenMode(unsigned int ScreenMode, ostream &out)
{
	//BUG implement this function
	cout << "WARNING: this function is not yet implemented." << endl;
	return false;
}

void HglApplication::PrintScreenMode(unsigned int ScreenMode, ostream &out)
{
	//BUG implement this function
	cout << "WARNING: this function is not yet implemented." << endl;
}

void HglApplication::PrintAllScreenModes(ostream &out)
{
	//BUG implement this function
	cout << "WARNING: this function is not yet implemented." << endl;
}

class OffViewApp : public HglApplication 
{
public:
	OffViewApp(){}
	virtual ~OffViewApp(){};

	virtual void draw_init();
	virtual void draw();
	virtual void key_down(SDLKey AHKey, char c);
	virtual void resize(unsigned int width, unsigned int height);
	virtual void mouse_motion(const SDL_MouseMotionEvent& event);
	virtual void mouse_wheel(const SDL_MouseButtonEvent& event);

	virtual const char *name();
	virtual void on_idle(unsigned int elapsed_time);

	Hgl *m_Hgl;
};

void OffViewApp::draw_init()
{
	m_Hgl = new Hgl(m_window);
	m_Hgl->MakeCurrent();
}

void OffViewApp::draw()
{
	m_Hgl->MakeCurrent();
	// Clear the color and depth buffers.  
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  
	// We don't want to modify the projection matrix. */  
	glMatrixMode( GL_MODELVIEW );  
	glLoadIdentity( );  

	Hgl::Relocate(g_Camera);

	if(g_pOffObj)
		g_pOffObj->draw();

	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	//Draw a square  
	glBegin(GL_LINE_LOOP);  
	glColor3f(1.0f,0.0f,0.0f);  
	glVertex3f(-1.0f  , 0.0f  ,  -1.0f  );  
	glColor3f(0.0f,1.0f,0.0f);  
	glVertex3f(-1.0f  , 0.0f  ,  1.0f  );  
	glColor3f(0.0f,0.0f,1.0f);  
	glVertex3f( 1.0f  , 0.0f  ,  1.0f  );  
	glColor3f(1.0f,1.0f,0.0f);  
	glVertex3f( 1.0f  , 0.0f  ,  -1.0f  );  
	glEnd();  


	Hgl::Finish();
	Hgl::SwapBuffers();
}

void OffViewApp::key_down( SDLKey AHKey, char c )
{

}

void OffViewApp::resize( unsigned int width, unsigned int height )
{
	m_Hgl->MakeCurrent();
	HRect ClientRect = HRect(0,0,(REAL)width,(REAL)height);
	Hgl::ResizeWindow(ClientRect);
}

void OffViewApp::mouse_motion( const SDL_MouseMotionEvent& event )
{

}

void OffViewApp::mouse_wheel( const SDL_MouseButtonEvent& event )
{

}

const char * OffViewApp::name()
{
	return "OffViewer";
}

void OffViewApp::on_idle( unsigned int elapsed_time )
{

}


int main( int argc, char* argv[] )
{
	if(argc!=2) return 0;
	HglApplication* app = new OffViewApp;
	Uint32 flags = SDL_OPENGL | SDL_RESIZABLE;


	int bpp = 32; //the current display color depth
	int width = 600;
	int height = 600;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_EnableUNICODE(1);
	//Set some Attribute of OpenGL in SDL  
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );  
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );  
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );  
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );  
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );  

	SDL_Surface* screen = SDL_SetVideoMode(width, height, bpp, flags);

	if (screen==NULL)
	{
		cout << "ERROR: Couldn't set " << width << "x" << height << " GL video mode: " << SDL_GetError() << endl;
		SDL_Quit();
		return 2;
	}
	SDL_WM_SetCaption(app->name(), app->name());

	//do OpenGL init
	app->draw_init();
	app->resize(screen->w, screen->h);

	try
	{
		g_pOffObj = new OFFObject();
		g_pOffObj->readfile(argv[1]);
	}
	catch(HException& e)
	{
		delete g_pOffObj;
		g_pOffObj=NULL;
		throw e;
	}

	// camera's axises are where it see the original axises
	g_Camera.Y = Point3D(0,0,-1);
	g_Camera.Z = Point3D(0,1,0);
	g_Camera.Position = Point3D(0,0,-5);


	bool done = false;
	Uint32 CurrentTime = SDL_GetTicks();
	while (!done)
	{
		//SDL_PumpEvents();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_KEYDOWN:
				{
					if (event.key.keysym.sym==SDLK_ESCAPE)
						done = true;
					else
					{
						char a = '\0';
						if (event.key.keysym.unicode<0x80 && event.key.keysym.unicode>0)
							a = (char)event.key.keysym.unicode;
						app->key_down(event.key.keysym.sym, a);
					}
				}
				break;
			case SDL_VIDEORESIZE:
				{
#ifndef WIN32
					SDL_SetVideoMode(event.resize.w, event.resize.h, bpp, flags);
#endif //WIN32
					app->resize(event.resize.w, event.resize.h);
				}
				break;
			case SDL_MOUSEMOTION:
				{
					app->mouse_motion(event.motion);
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				{
					if(event.button.button==SDL_BUTTON_WHEELDOWN || event.button.button==SDL_BUTTON_WHEELUP)
					{
						app->mouse_wheel(event.button);
					}
					break;
				}
			case SDL_QUIT:
				done = true;
				break;
			}
		}
		Uint32 NewTime = SDL_GetTicks();
		app->on_idle(NewTime-CurrentTime);
		CurrentTime = NewTime;
		app->draw();
	}
	delete app;
	app = NULL;
	SDL_Quit();
	return 0;
}
