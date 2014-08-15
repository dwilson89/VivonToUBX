#define _USE_MATH_DEFINES

#include <iostream>
#include <exception>
#include <cmath>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <sstream> 
#include <cstdint>
#include <math.h>

//#include <GeographicLib/Geocentric.hpp>
//#include <GeographicLib/GeoCoords.hpp>
//#include <GeographicLib/LocalCartesian.hpp>
//#include <GeographicLib/Math.hpp>
#include <Windows.h>
//kye's Code Snippet
#define WIN32_LEAN_AND_MEAN

#define MAX_MAVLINK_LENGTH 263
#define CMD_ID_VICON_DATA 104
#define PAY_LEN_VICON_DATA 255
#define X25_INIT_CRC 0xFFFF // this may work unsure
//end snippet

#include <ctime>
#include <time.h>

#include "UAVData.h"
#include "ViconUnman.h"
#include "SerialWriter.h"

using namespace std;

using namespace UnmanViconWrap;

//using namespace GeographicLib;

//include opengl

/*Code for Serial Communication*/
#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace System::Threading;
//this code section ends here

using namespace System::Collections::Generic;
struct msgBuff_t 														//Temporary Storage While Reading Messages
{
		unsigned int byteCount;												//Number of bytes in the string the message
		unsigned char inputs[MAX_MAVLINK_LENGTH];							//Large enough to hold a full Mavlink message
};

#pragma once
ref class ViconToGeo
{

private:
	static System::String^ currentMessage;
	static System::String^ rawData;
	System::String^ message;
	string RawDataString(double x, double y, double z, double lat, double lon, double alt, double pitch, double roll, double yaw);
	string RawDataString(double x, double y, double z, double pitch, double roll, double yaw);
	List<UAVData^>^ uavs;
	int numUAV;

	//ViconUnman^ viconConnect;
	
	UnmanViconWrap::Class1 viconConnect;

public:
	ViconToGeo(int numUav);
	
	System::String^ ToPayload(int uavIndex);
	
	void SendPayload(int uavIndex, SerialWriter^ sw);

	System::String^ GetStringRep();
	string ComLatLonMessage(double lat, double lon);
	System::String^ GetSentMessage();
	
	void GetandAssignViconData();
	//void ConvertViconToGeo();

	void ConnectToVicon(std::string hostName);
	void DisconnectFromVicon();
	
	// Kye's Code
	
	//This next line just needs to be a thing. Accept it.
	//Message CRC seeds. Each ID has a corresponding seed in this array. Heartbeat(ID:0)=50                              20                                                                                           20 
	//const unsigned char MAVLINK_MESSAGE_CRCS[256];

	static initonly array<unsigned char>^ MAVLINK_MESSAGE_CRCS = {50, 124, 137, 0, 237, 217, 104, 119, 0, 0, 0, 89, 0, 0, 0, 0, 0, 0, 0, 0, 214, 159, 220, 168, 24, 23, 170, 144, 67, 115, 39, 246, 185, 104, 237, 244, 222, 212, 9, 254, 230, 28, 28, 132, 221, 232, 11, 153, 41, 39, 214, 223, 141, 33, 15, 3, 100, 24, 239, 238, 0, 0, 183, 0, 130, 0, 148, 21, 0, 52, 124, 0, 0, 0, 20, 0, 152, 143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 183, 63, 54, 0, 0, 0, 0, 0, 0, 0, 19, 102, 158, 208, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 134, 219, 208, 188, 84, 22, 19, 21, 134, 0, 78, 68, 189, 127, 42, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 49, 170, 44, 83, 46, 247};

	static uint16_t crc_calculate(unsigned char *pBuffer, int length);
	static uint16_t crc_accumulate(unsigned char b, uint16_t crc);

	void compileMessage(msgBuff_t &msg, unsigned long long time, float x, float y, float z, float r, float p, float q);
	void packVariableFloat(msgBuff_t &msg, float input, unsigned char pos);
	void packVariableUint64(msgBuff_t &msg, unsigned long long input, unsigned char pos);
	void packVariableUint16(msgBuff_t &msg, unsigned int input, unsigned char pos);

	void clearMsgBuff(msgBuff_t &msg);
	//void sendMessage(msgBuff_t outMsg);
	// end
};

