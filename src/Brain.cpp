#include "Brain.h"
extern std::ofstream herr;
Brain::~Brain() {
	if (yescome) com.close();
}

void Brain::init(const char *comport) {
	//brainSerial->begin(9600);// default, no need to set
	if (comport[0]=='n')
		yescome=false;
	else
		yescome=true;
	com.open(comport);
	// old lines
	freshPacket = false;
	inPacket = false;
	packetIndex = 0;
	packetLength = 0;
	checksum = 0;
	checksumAccumulator = 0;
	eegPowerLength = 0;
	hasPower = false;
	signalQuality = 200;
	attention = 0;
	meditation = 0;
	clearEegPower();
}

bool Brain::update(double t) {
	if (com.gcount()==0)
		return false;
	com.read((char *)(&latestByte),1);

	// Build a packet if we know we're and not just listening for sync bytes.
	if (inPacket) {

		// First byte after the sync bytes is the length of the upcoming packet.
		if (packetIndex == 0) {
			packetLength = latestByte;

			// Catch error if packet is too long
			if (packetLength > MAX_PACKET_LENGTH) {
				// Packet exceeded max length
				// Send an error
				herr<< "ERROR: Packet too long"<<std::endl;
				inPacket = false;
			}
		}
		else if (packetIndex <= packetLength) {
			// Run of the mill data bytes.

			// Print them here

			// Store the byte in an array for parsing later.
			packetData[packetIndex - 1] = latestByte;

			// Keep building the checksum.
			checksumAccumulator += latestByte;
		}
		else if (packetIndex > packetLength) {
			// We're at the end of the data payload.

			// Check the checksum.
			checksum = latestByte;
			checksumAccumulator = 255 - checksumAccumulator;

			// Do they match?
			if (checksum == checksumAccumulator) {

				// Parse the data. parsePacker() returns true if parsing succeeds.
				time=t;
				if (parsePacket()) {
					freshPacket = true;
				}
				else {
					// Parsing failed, send an error.
					herr<< "ERROR: Could not parse"<<std::endl;
					//sprintf(latestError, "ERROR: Could not parse");
					// good place to print the packet if debugging
				}
			}
			else {
				// Checksum mismatch, send an error.
				herr<<  "ERROR: Checksum"<<std::endl;
				// good place to print the packet if debugging
			}
			// End of packet

			// Reset, prep for next packet
			inPacket = false;
		}

		packetIndex++;
	}

	// Look for the start of the packet
	if ((latestByte == 170) && (lastByte == 170) && !inPacket) {
		// Start of packet
		inPacket = true;
		packetIndex = 0;
		packetLength = 0; // Technically not necessarry.
		checksum = 0; // Technically not necessary.
		checksumAccumulator = 0;
		//clearPacket(); // Zeros the packet array, technically not necessarry.
		//clearEegPower(); // Zeros the EEG power. Necessary if hasPower turns false... better off on the gettter end?	 
	}

	// Keep track of the last byte so we can find the sync byte pairs.
	lastByte = latestByte;

	if(freshPacket) {
		freshPacket = false;
		return true;
	}
	else {
		return false;
	}
}

void Brain::clearPacket() {
	for (int i = 0; i < MAX_PACKET_LENGTH; i++) {
		packetData[i] = 0;
	}	 
}

void Brain::clearEegPower() {
	// Zero the power bands.
	for(int i = 0; i < EEG_POWER_BANDS; i++) {
		eegPower[i] = 0;
	}
}

bool Brain::parsePacket() {
	// Loop through the packet, extracting data.
	// Based on mindset_communications_protocol.pdf from the Neurosky Mindset SDK.
	hasPower = false;
	clearEegPower();	// clear the eeg power to make sure we're honest about missing values... null would be better than 0.

	for (int i = 0; i < packetLength; i++) {
		switch (packetData[i]) {
		case 2:
			signalQuality = packetData[++i];
			break;
		case 4:
			attention = packetData[++i];
			break;
		case 5:
			meditation = packetData[++i];
			break;
		case 131:
			// ASIC_EEG_POWER: eight big-endian 3-byte unsigned integer values representing delta, theta, low-alpha high-alpha, low-beta, high-beta, low-gamma, and mid-gamma EEG band power values			 
			// The next byte sets the length, usually 24 (Eight 24-bit numbers... big endian?)
			eegPowerLength = packetData[++i];

			// Extract the values. Possible memory savings here by creating three temp longs?
			for(int j = 0; j < EEG_POWER_BANDS; j++) {
				eegPower[j] = ((unsigned long)packetData[++i] << 16) | ((unsigned long)packetData[++i] << 8) | (unsigned long)packetData[++i];
			}

			hasPower = true;
			// This seems to happen once during start-up on the force trainer. Strange. Wise to wait a couple of packets before
			// you start reading.

			break;
		default:
			return false;
		}
	}
	CEEGdata temp;
	temp.signalQuality=signalQuality;
	temp.attention=attention;
	temp.time=time;
	for(int j = 0; j < EEG_POWER_BANDS; j++)
		temp.eegPower[j]=eegPower[j];
	eegdata.push_back(temp);
	return true;
}
