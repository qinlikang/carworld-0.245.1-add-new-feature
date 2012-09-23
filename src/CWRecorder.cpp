#include "CWRecorder.h"
#include <boost/format.hpp>


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
	Hgl::WriteText(recorder_state.c_str(), Point2D(-.50f,-.75f)); //write global position

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

void VehicleStateRecorder::dump()
{

}

void VehicleStateRecorder::restore()
{

}

void VehicleStateRecorder::draw_on_screen()
{
	RecorderBase::draw_on_screen();
	glPushMatrix();

	boost::format fmt("Records Num: %d");
	fmt%m_Records.size();
	Hgl::WriteText(fmt.str().c_str(), Point2D(-.50f,-.85f)); //write global position

	glPopMatrix();
}
