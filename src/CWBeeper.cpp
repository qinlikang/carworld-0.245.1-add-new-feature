#include "CWBeeper.h"
#include "CWMixerManager.h"
#include "H_Standard.h"
#include "H_Keys.h"
#include "MyDatabase.h"
using namespace std;

SoundPlayer::SoundPlayer( void)
	:m_Chunk(NULL)
{
	m_nChannel = GetMixer().AddOneChannel()-1;
}


SoundPlayer::~SoundPlayer(void)
{
}

void SoundPlayer::free_chunk()
{
	if(m_Chunk)
	{
		Mix_FreeChunk(m_Chunk);
	}
}

bool SoundPlayer::load_wav_file( const string& file )
{
	free_chunk();
	m_strFileName = file;
	m_Chunk = Mix_LoadWAV(file.c_str());
	if(!is_valid())
		throw HException(GetMixer().GetError());
	return is_valid();
}

void SoundPlayer::play_once()
{
	play_n_times(0);
}

void SoundPlayer::pause()
{
	Mix_Pause(m_nChannel);
}

void SoundPlayer::stop()
{
	Mix_HaltChannel(m_nChannel);
}

void SoundPlayer::resume()
{
	Mix_Resume(m_nChannel);
}

SoundPlayer::EBeeperState SoundPlayer::get_state() const
{
	if(Mix_Playing(m_nChannel)) return EBS_Playing;
	else return EBS_Paused;
}

void SoundPlayer::play_n_times( unsigned int ntime )
{
	if(m_Chunk)
		Mix_PlayChannel(m_nChannel,m_Chunk,ntime);
}

void CWBeeper::init()
{
	m_player.load_wav_file(Filename);
	m_key = ToSDLKey(Keybinding.c_str());
}

void CWBeeper::beep()
{
	m_player.play_once();
}

static AudioPlayer* sp_audioplayer=NULL;

AudioPlayer* AudioPlayer::shared_audio()
{
	if(sp_audioplayer) return sp_audioplayer;
	sp_audioplayer = new AudioPlayer;
	return sp_audioplayer;
}

SoundPlayer* AudioPlayer::get_sound( const std::string& name )
{
	if(m_Sounds.find(name)==m_Sounds.end())return NULL;
	else return &m_Sounds[name];
}

void AudioPlayer::load_from_db()
{
	CppSQLite3Query q = MyDatabase::shared_input_database()->execQuery("select * from MusicResource;");

	while(!q.eof())
	{
		SoundPlayer sp;
		sp.load_wav_file(q.fieldValue("File"));
		m_Sounds[q.fieldValue("EffectName")]=sp;
		q.nextRow();
	}
}

void AudioPlayer::release_audio()
{
	if(sp_audioplayer)
		delete sp_audioplayer;

	sp_audioplayer = NULL;
}
