#pragma once
#define MAX_PACKET_LENGTH 32
#define EEG_POWER_BANDS 8

// TK some kind of reset timer if we don't receive a packet for >3 seconds?
#include <fstream>
#include <vector>
struct CEEGdata
{
	double time;
	unsigned char signalQuality;
	unsigned char attention;
	unsigned char meditation;
	unsigned long eegPower[EEG_POWER_BANDS]; // delta,theta,low alpha, high alpha, low beta, high beta, low gamma, mid gamma
};
class Brain {
public:
	bool yescome;
	std::ifstream com;
	std::vector <CEEGdata> eegdata; // main record
//std::vector <unsigned char > eegbuffer; // temp buffer a queue
	bool biofeedback; // whether use attention to update the gas meter
	~Brain();	
	void init(const char * comport);
	// Run this in the main loop. receiving time
	bool update(double t);

	// String with most recent error.
	//char* readErrors();

private:
	double time;
	unsigned char signalQuality;
	unsigned char attention;
	unsigned char meditation;
	unsigned long eegPower[EEG_POWER_BANDS]; // delta,theta,low alpha, high alpha, low beta, high beta, low gamma, mid gamma
	unsigned char packetData[MAX_PACKET_LENGTH];
	bool inPacket;
	unsigned char latestByte;
	unsigned char lastByte;
	unsigned char packetIndex;
	unsigned char packetLength;
	unsigned char checksum;
	unsigned char checksumAccumulator;
	unsigned char eegPowerLength;
	bool hasPower;
	void clearPacket();
	void clearEegPower();
	bool parsePacket();
	char latestError[23];		
	bool freshPacket;
};