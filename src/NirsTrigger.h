#pragma once
#include <fstream> 
class NirsTrigger
{
public:
	NirsTrigger(void){	TriggerTime=-1; }
	~NirsTrigger(void){ com.close();}
	std::ofstream com;
	double TriggerTime; //in ms since start
	void init(const char *comport){ com.open(comport);};
	void update(double t); // automatic turn off after 100ms
	void on(double t); // the user does not need to turn off
};

