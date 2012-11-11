#include "CWBeeper.h"
#include "CWMixerManager.h"
#include "H_Standard.h"
using namespace std;

CWBeeper::CWBeeper( void)
	:m_Chunk(NULL)
{
	m_nChannel = GetMixer().AddOneChannel()-1;
}


CWBeeper::~CWBeeper(void)
{
}

void CWBeeper::free_chunk()
{
	if(m_Chunk)
	{
		Mix_FreeChunk(m_Chunk);
	}
}

bool CWBeeper::load_wav_file( const string& file )
{
	free_chunk();
	m_strFileName = file;
	m_Chunk = Mix_LoadWAV(file.c_str());
	if(!is_valid())
		throw HException(GetMixer().GetError());
	return is_valid();
}

void CWBeeper::play_once()
{
	play_n_times(0);
}

void CWBeeper::pause()
{
	Mix_Pause(m_nChannel);
}

void CWBeeper::stop()
{
	Mix_HaltChannel(m_nChannel);
}

void CWBeeper::resume()
{
	Mix_Resume(m_nChannel);
}

CWBeeper::EBeeperState CWBeeper::get_state() const
{
	if(Mix_Playing(m_nChannel)) return EBS_Playing;
	else return EBS_Paused;
}

void CWBeeper::play_n_times( unsigned int ntime )
{
	if(m_Chunk)
		Mix_PlayChannel(m_nChannel,m_Chunk,ntime);
}
