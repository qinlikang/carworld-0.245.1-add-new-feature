#include "CWRecorder.h"
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <ctime>
#include <cassert>
#include <fstream>
#include "H_Main.h"
#include "MyDatabase.h"

CWRecorder::CWRecorder(RecorderState state)
	:m_RecorderState(state)
{
	reset();
	if(m_RecorderState==ERS_Replay)
		restore(); // restore the records from replay file
}


CWRecorder::~CWRecorder(void)
{
	if(m_RecorderState==ERS_Record)
		dump(); // dump the records
}

void CWRecorder::update()
{
	if(m_RecorderState==ERS_Replay)
	{
		replay();
	}
	else if(m_RecorderState==ERS_Record)
	{
		record();
	}
	else if(m_RecorderState==ERS_OFF)
	{
		//do nothing
	}
}

void CWRecorder::reset()
{
	m_Cursor=m_Records.begin();
	if(m_RecorderState==ERS_Record)
	{
		m_Records.clear();
	}
	m_Timer.start();
}

void CWRecorder::draw_on_screen()
{
	glPushMatrix();

	string recorder_state("RECODER: ");
	switch (m_RecorderState)
	{
	case ERS_Record:
		recorder_state+="recording..";
		break;
	case ERS_Replay:
		recorder_state+="replaying..";
		break;
	case ERS_OFF:
		recorder_state+="OFF";
		break;
	}
	Hgl::WriteText(recorder_state.c_str(), Point2D(-.90f,-.75f)); //write global position
	
	// write more information about recorder
	string msg_line_1;
	string msg_line_2;
	const unsigned int threshold=40;
	if(m_strOtherMsg.size()	>threshold)
	{
		msg_line_1=m_strOtherMsg.substr(0,threshold);
		msg_line_2="    "+m_strOtherMsg.substr(threshold,2*threshold-4);
	}
	else
	{
		msg_line_1=m_strOtherMsg;
	}
	Hgl::WriteText(msg_line_1.c_str(), Point2D(-.95f,-.85f)); 
	Hgl::WriteText(msg_line_2.c_str(), Point2D(-.95f,-.95f)); 

	glPopMatrix();
}

std::string CWRecorder::dump()
{
	using namespace boost;
	format day_fmt("%04d/%02d/%02d");
	format time_fmt("%02d:%02d:%02d");
	format table_name_fmt("%s_%04d%02d%02d_%02d%02d%02d");

	// get current time
	time_t raw_time;
	tm* ptm;
	time(&raw_time);
	ptm = localtime(&raw_time);

	day_fmt%(ptm->tm_year+1900)%(ptm->tm_mon+1)%(ptm->tm_mday);
	time_fmt%(ptm->tm_hour)%(ptm->tm_min)%(ptm->tm_sec);
	table_name_fmt%name()%(ptm->tm_year+1900)%(ptm->tm_mon+1)%(ptm->tm_mday)%(ptm->tm_hour)%(ptm->tm_min)%(ptm->tm_sec);

	// add a record to RecordSummary table
	CppSQLite3DB* db = MyDatabase::shared_output_database();
	if(!db->tableExists("RecordSummary"))
	{
		throw HException("RecordSummary does not exist in output database");
	}
	format value_fmt("insert into RecordSummary values('%s','%s','%s')");
	value_fmt%day_fmt.str()%time_fmt.str()%table_name_fmt.str();
	db->execDML(value_fmt.str().c_str());

	return table_name_fmt.str();
}

VehicleStateRecorder::VehicleStateRecorder( CWVehicle* pVehicle/*=NULL*/,RecorderState state/*=ERS_Recording*/ )
	:m_Vehicle(pVehicle),CWRecorder(state)
{
}

void VehicleStateRecorder::record()
{
	if(!m_Vehicle) return;
	if(m_Records.empty())
	{
		reset();
	}

	double elapse_time = double(m_Timer.get_elapse_clocks())/CLOCKS_PER_SEC;

	m_Records.push_back(BaseItemPtrT(new CWRecordItem_VehicleState(m_Vehicle,elapse_time)));
	m_Cursor=m_Records.end();
}

void VehicleStateRecorder::replay()
{
	if(!m_Vehicle) return;
	boost::shared_ptr<CWRecordItem_VehicleState> p=dynamic_pointer_cast<CWRecordItem_VehicleState>(*m_Cursor);
	if(p)
		m_Vehicle->SetState(p->m_State);
	++m_Cursor;
}

std::string VehicleStateRecorder::dump()
{
	using namespace boost;
	string table = BaseT::dump();
	
	CppSQLite3DB* db = MyDatabase::shared_output_database();

	format fmt = format("create table %s (i int, elapsed_time double,x double, y double, z double)")%table;
	// creat a new record table
	db->execDML(
		fmt.str().c_str()
		);

	// record the stuff
	unsigned int i=0;
	BOOST_FOREACH(BaseItemPtrT& p_record,m_Records)
	{
		ItemPtrT p = static_pointer_cast<ItemT>(p_record);
		if(p)
		{
			Point3D& pos = p->m_State.m_Ref.Position;
			format fmt = format("insert into %s values(%d, %f, %f, %f, %f)")%table%i%(p->m_TimeElapse)%pos.x()%pos.y()%pos.z();
			db->execDML(fmt.str().c_str());
			++i;
		}
	}

	return table;
}

void VehicleStateRecorder::restore()
{

}

void VehicleStateRecorder::draw_on_screen()
{
	BaseT::draw_on_screen();
	glPushMatrix();

	boost::format fmt("%d");
	fmt%m_Records.size();
	Hgl::WriteText(fmt.str().c_str(), Point2D(-0.25f,-0.75f)); // write the recorder state

	glPopMatrix();
}

CWRecordItem_KeyboardInput::CWRecordItem_KeyboardInput( HWindow* hwindow,double time_elapse ) : CWRecordItem(time_elapse)
{
	upPressed = hwindow->IsPressed(SDLK_UP);
	downPressed = hwindow->IsPressed(SDLK_DOWN);
	leftPressed = hwindow->IsPressed(SDLK_LEFT);
	rightPressed = hwindow->IsPressed(SDLK_RIGHT);
	spacePressed = hwindow->IsPressed(SDLK_SPACE);
}

KeyboardInputRecorder::KeyboardInputRecorder( HWindow* hwindow,RecorderState state/*=ERS_Record*/ )
	: BaseT(state)
	, m_hWindow(hwindow)
{}

void KeyboardInputRecorder::draw_on_screen()
{

}

void KeyboardInputRecorder::restore()
{

}

std::string KeyboardInputRecorder::dump()
{
	using namespace boost;
	string table = BaseT::dump();

	CppSQLite3DB* db = MyDatabase::shared_output_database();

	format fmt = format("create table %s (i int, elapsed_time double,up_pressed int, down_pressed int, left_pressed int, right_pressed int, space_pressed int)")%table;
	// creat a new record table
	db->execDML(
		fmt.str().c_str()
		);

	// record the stuff
	unsigned int i=0;
	BOOST_FOREACH(BaseItemPtrT& p_record,m_Records)
	{
		ItemPtrT p = static_pointer_cast<ItemT>(p_record);
		if(p)
		{
			format fmt = format("insert into %s values(%d, %f, %d, %d, %d, %d, %d)")%table%i%(p->m_TimeElapse)
				%int(p->upPressed)%int(p->downPressed)%int(p->leftPressed)%int(p->rightPressed)%int(p->spacePressed);
			db->execDML(fmt.str().c_str());
			++i;
		}
	}

	return table;

}

void KeyboardInputRecorder::replay()
{

}

void KeyboardInputRecorder::record()
{
	if(!m_hWindow) return;
	if(m_Records.empty())
	{
		reset();
	}

	double elapse_time = double(m_Timer.get_elapse_clocks())/CLOCKS_PER_SEC;

	m_Records.push_back(BaseItemPtrT(new ItemT(m_hWindow,elapse_time)));
	m_Cursor=m_Records.end();
}
