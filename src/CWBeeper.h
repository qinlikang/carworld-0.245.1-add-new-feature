#pragma once
#include "SDL_mixer.h"
#include <string>
class CWBeeper
{
public:
	enum EBeeperState
	{
		EBS_Playing,
		EBS_Paused
	};

	CWBeeper(void);
	~CWBeeper(void);
	
	bool load_wav_file(const std::string& file);
	bool is_valid()const{return m_Chunk!=NULL;}

	EBeeperState get_state()const;

	void play_once();
	void play_n_times(unsigned int ntime);
	void pause();
	void stop();
	void resume();

private:
	void free_chunk();

	int m_nChannel;
	std::string m_strFileName;
	Mix_Chunk* m_Chunk;
};
