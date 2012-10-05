
#ifndef __CW_RECORDER_H
#define __CW_RECORDER_H

#define MAX_RECORD_NUM 100000

#include "CarWorldClasses.h"
#include "CWVehicle.h"
#include <boost/smart_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>

//! Base class of record item
/*!
	the smart pointers of this class will be record
	this class is the base class of all record item
*/
class CWRecordItem
{
public:
	CWRecordItem(){}
	virtual ~CWRecordItem(){}
	virtual void write_to_os(std::ostream& os){}
};
typedef boost::shared_ptr<CWRecordItem> CWRecordItemPtr;

//! the Vehicle State record item
class CWRecordItem_VehicleState: public CWRecordItem
{
public:
	CWRecordItem_VehicleState(CWVehicle* pVehicle)
	{
		m_State=pVehicle->GetState();
	}
	CWVehicleState m_State;
	virtual void write_to_os(std::ostream& os);
};

//! Base class of recorder
/*!
	this class is the base class of all recorder
*/
class CWRecorder :
	public CWFeature
{
public:
	enum RecorderState
	{
		ERS_Recording,
		ERS_Replaying,
		ERS_OFF
	};

	CWRecorder(RecorderState state);
	virtual ~CWRecorder(void);

	typedef CWRecorder RecorderBase;

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
	virtual std::string dump(){return "";}// return the filename
	virtual void restore(){};
	virtual const char* name(){ return "CWRecorder";};

	void set_state(RecorderState state){m_RecorderState=state;reset();}
	RecorderState get_state()const{return m_RecorderState;}
	bool is_replay_and_finished()const{return m_RecorderState==ERS_Replaying&&m_Cursor==m_Records.end();}
public:
	static const string RecordPath;

	string m_strOtherMsg;// other msg about recorder that will print onto screen
protected:
	RecorderState m_RecorderState;
	std::vector<CWRecordItemPtr> m_Records;
	std::vector<CWRecordItemPtr>::iterator m_Cursor;
};

//! the Vehicle State Recorder
class VehicleStateRecorder : public CWRecorder
{

public:
	VehicleStateRecorder(CWVehicle* pVehicle=NULL,RecorderState state=ERS_Recording);

	virtual void record();
	virtual void replay();
	virtual std::string dump();
	virtual void restore();
	virtual const char* name(){return "VehicleStateRecorder";}

	virtual void draw_on_screen();
private:
	CWVehicle* m_Vehicle;
};



#endif //__CW_RECORDER_H
