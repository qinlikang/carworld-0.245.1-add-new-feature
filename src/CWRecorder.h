
#ifndef __CW_RECORDER_H
#define __CW_RECORDER_H

#define MAX_RECORD_NUM 100000

#include "CarWorldClasses.h"
#include "CWVehicle.h"
#include <boost/smart_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>

//!  RecorderTimer 
/*!
	for recording time, when recording.
	can pause and resume.
	record the clock passed since start (except pausing time)
*/
class RecorderTimer
{
	long m_TimeElapse; // clocks passed by since start to record
	bool m_Toggled;    // mark if the timer is on
	std::clock_t m_lastTime; // last call time
public:
	RecorderTimer():m_TimeElapse(0),m_Toggled(false){}

	void reset(){m_TimeElapse=0;m_lastTime=std::clock();}
	void start(){m_Toggled=true;reset();}

	void pause(){m_Toggled=false;}
	void resume(){m_Toggled=true;m_lastTime=std::clock();}

	long get_elapse_clocks(){update(); return m_TimeElapse;}
private:
	void update()
	{
		if(m_Toggled)
		{
			m_TimeElapse += std::clock()-m_lastTime;
			m_lastTime = std::clock();
		}
	}
};


//! Base class of record item
/*!
	the smart pointers of this class will be record
	this class is the base class of all record item
*/
class CWRecordItem
{
public:
	CWRecordItem(double time_elapse):m_TimeElapse(time_elapse){}
	virtual ~CWRecordItem(){}
	double m_TimeElapse; // seconds passed by since start to record
	typedef CWRecordItem BaseT;
};


//! Base class of recorder
/*!
	this class is the base class of all recorder
*/
class CWRecorder :
	public CWFeature
{
public:
	typedef boost::shared_ptr<CWRecordItem> BaseItemPtrT;
	typedef CWRecordItem BaseItemT;
	typedef CWRecorder BaseT;

	enum RecorderState
	{
		ERS_Record,
		ERS_Replay,
		ERS_OFF
	};

	CWRecorder(RecorderState state);
	virtual ~CWRecorder(void);

	// no need to implement
	virtual void reset();
	virtual void update();

	// implement these if need
	virtual void draw_init(){}
	virtual void draw(){}
	virtual void draw_on_screen();

	// must implement these
	virtual void record(){};
	virtual void replay(){};
	virtual std::string dump();// return table created
	virtual void restore(){};
	virtual const char* name(){ return "CWRecorder";};

	void set_state(RecorderState state){m_RecorderState=state;reset();}
	RecorderState get_state()const{return m_RecorderState;}
	bool is_replay_and_finished()const{return m_RecorderState==ERS_Replay&&m_Cursor==m_Records.end();}
public:
	static const string RecordPath;
	RecorderTimer m_Timer;
	string m_strOtherMsg;// other msg about recorder that will print onto screen
protected:
	RecorderState m_RecorderState;
	std::vector<BaseItemPtrT> m_Records;
	std::vector<BaseItemPtrT>::iterator m_Cursor;
};

////////////////////////////////////////////////////////////////////////// Vehicle State Recorder
//! the Vehicle State record item
class CWRecordItem_VehicleState: public CWRecordItem
{
public:
	CWRecordItem_VehicleState(CWVehicle* pVehicle,double time_elapse)
		: CWRecordItem(time_elapse)
	{
		m_State=pVehicle->GetState();
	}
	CWVehicleState m_State;
};

//! the Vehicle State Recorder
class VehicleStateRecorder : public CWRecorder
{
public:
	typedef CWRecordItem_VehicleState ItemT;
	typedef boost::shared_ptr<CWRecordItem_VehicleState> ItemPtrT;

	VehicleStateRecorder(CWVehicle* pVehicle=NULL,RecorderState state=ERS_Record);

	virtual void record();
	virtual void replay();
	virtual std::string dump();
	virtual void restore();
	virtual const char* name(){return "VehicleStateRecorder";}

	virtual void draw_on_screen();
private:
	CWVehicle* m_Vehicle;
};
////////////////////////////////////////////////////////////////////////// KeyboardInput Recorder
class HWindow;
//! the KeyboardInput record item
class CWRecordItem_KeyboardInput: public CWRecordItem
{
public:
	CWRecordItem_KeyboardInput(HWindow* hwindow,double time_elapse);

	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;
	bool spacePressed;
};

//! the KeyboardInput Recorder
class KeyboardInputRecorder : public CWRecorder
{
public:
	typedef CWRecordItem_KeyboardInput ItemT;
	typedef boost::shared_ptr<CWRecordItem_KeyboardInput> ItemPtrT;

	KeyboardInputRecorder(HWindow* hwindow,RecorderState state=ERS_Record);

	virtual void record();
	virtual void replay();
	virtual std::string dump();
	virtual void restore();
	virtual const char* name(){return "UserInputRecorder";}

	virtual void draw_on_screen();
private:
	HWindow* m_hWindow;

};



#endif //__CW_RECORDER_H
