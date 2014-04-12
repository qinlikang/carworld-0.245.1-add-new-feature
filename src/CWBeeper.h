#pragma once
#include "SDL_mixer.h"
#include "SDL_keysym.h"
#include <string>
#include <map>
class SoundPlayer
{
public:
	enum EBeeperState
	{
		EBS_Playing,
		EBS_Paused
	};

	SoundPlayer(void);
	~SoundPlayer(void);
	
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

class AudioPlayer
{
	std::map<std::string,SoundPlayer> m_Sounds;

	AudioPlayer(){}
public:
	static AudioPlayer* shared_audio();
	static void release_audio();

	void load_from_db();
	SoundPlayer* get_sound(const std::string& name);
};

class CWBeeper
{
	SoundPlayer m_player;
	SDLKey m_key;
public:
	std::string BeeperName;
	std::string Filename;
	std::string Keybinding;
	
	void init();
	SDLKey getSDLKeyBind(){return m_key;}
	void beep();
};