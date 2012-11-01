#include "CWMixerManager.h"
#include "sdl_mixer.h"

CWMixerManager::CWMixerManager(void)
	: m_nChannels(0)
{
}


CWMixerManager::~CWMixerManager(void)
{
}


int CWMixerManager::AddOneChannel() 
{
	++m_nChannels;
	//allocate channels
	m_nChannels = Mix_AllocateChannels(m_nChannels);
	return m_nChannels;
}

int CWMixerManager::CloseOneChannel() 
{
	--m_nChannels;
	if(m_nChannels<0) m_nChannels=0;
	//allocate channels
	m_nChannels = Mix_AllocateChannels(m_nChannels);
	return m_nChannels;
}

int CWMixerManager::GetChannelNum() const
{
	return m_nChannels;
}

int CWMixerManager::Init()
{
	//start up sdl_mixer
	return Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,1,4096);
}

void CWMixerManager::Quit()
{
	Mix_AllocateChannels(0);
	//close sdl_mixer
	Mix_CloseAudio();

}

const char* CWMixerManager::GetError() const
{
	return Mix_GetError();
}
