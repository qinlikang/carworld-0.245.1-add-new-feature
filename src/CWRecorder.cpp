#include "CWRecorder.h"
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <ctime>
#include <cassert>
#include <fstream>

const string CWRecorder::RecordPath="./Records/";

CWRecorder::CWRecorder(RecorderState state)
	:m_RecorderState(state)
{
	m_Cursor=m_Records.begin();
	if(m_RecorderState==ERS_Replaying)
		restore(); // restore the records from replay file
}


CWRecorder::~CWRecorder(void)
{
	if(m_RecorderState==ERS_Recording)
		dump(); // dump the records
}

void CWRecorder::update()
{
	if(m_RecorderState==ERS_Replaying)
	{
		replay();
	}
	else if(m_RecorderState==ERS_Recording)
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
	if(m_RecorderState==ERS_Recording)
	{
		m_Records.clear();
	}
}

void CWRecorder::draw_on_screen()
{
	glPushMatrix();

	string recorder_state("RECODER: ");
	switch (m_RecorderState)
	{
	case ERS_Recording:
		recorder_state+="recording..";
		break;
	case ERS_Replaying:
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

VehicleStateRecorder::VehicleStateRecorder( CWVehicle* pVehicle/*=NULL*/,RecorderState state/*=ERS_Recording*/ )
	:m_Vehicle(pVehicle),CWRecorder(state)
{
	m_Cursor=m_Records.begin();
}

void VehicleStateRecorder::record()
{
	if(!m_Vehicle) return;
	m_Records.push_back(CWRecordItemPtr(new CWRecordItem_VehicleState(m_Vehicle)));
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
	boost::format fmt("%d-%d-%d_%d-%d-%d.txt");
	
	// get current time, use it to format output filename
	time_t raw_time;
	tm* ptm;
	time(&raw_time);
	ptm = localtime(&raw_time);

	fmt%(ptm->tm_year+1900)%(ptm->tm_mon+1)%(ptm->tm_mday)%(ptm->tm_hour)%(ptm->tm_min)%(ptm->tm_sec);
	string filename=fmt.str();

	// use boost.filesystem to keep platform-independce
	// need compile the boost.filesystem to get the static lib.
	using namespace boost::filesystem;
	path record_path(RecorderBase::RecordPath + name());

	try
	{
		if(!exists(record_path))
		{
			// create the folder
			create_directories(record_path);
		}
		else if(!is_directory(record_path))
		{
			// delete the file, and create the folder
			remove_all(record_path);
			create_directories(record_path);
		}
	}
	catch(...)
	{
		cout<<"FAILED In "<<__FILE__<<"("<<__LINE__<<")"<<endl;
		cout<<"Cannot make directory for records file: "<<record_path<<endl;
		return "";
	}

	if(!portable_name(filename))
	{
		cout<<"FAILED In "<<__FILE__<<"("<<__LINE__<<")"<<endl;
		cout<<"not portable file name: "<<filename.c_str()<<endl;
		return "";
	}

	record_path.append(filename.begin(),filename.end());

	std::ofstream record_os(record_path.c_str());

	// record the stuff
	BOOST_FOREACH(CWRecordItemPtr& p_record,m_Records)
	{
		p_record->write_to_os(record_os);
	}
	record_os.close();

	return record_path.string();
}

void VehicleStateRecorder::restore()
{

}

void VehicleStateRecorder::draw_on_screen()
{
	RecorderBase::draw_on_screen();
	glPushMatrix();

	boost::format fmt("%d");
	fmt%m_Records.size();
	Hgl::WriteText(fmt.str().c_str(), Point2D(-0.25f,-0.75f)); // write the recorder state

	glPopMatrix();
}



void CWRecordItem_VehicleState::write_to_os( std::ostream& os )
{
	os<<m_State.m_Ref.Position<<endl;
}
