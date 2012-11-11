#pragma once
#include "H_Singleton.h"
class CWMixerManager
{
public:
	CWMixerManager(void);
	~CWMixerManager(void);

	int Init();
	void Quit();
	const char* GetError()const;

	int AddOneChannel();
	int CloseOneChannel();
	int GetChannelNum()const;
private:
	int m_nChannels;
};

typedef Singleton<CWMixerManager> MixerManagerSingleton;
inline CWMixerManager& GetMixer(){return MixerManagerSingleton::get_instance();}