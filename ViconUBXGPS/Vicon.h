#include <ViconDataStreamSDK_CPP/Client.h>
#include <iostream>


#ifdef WIN32
  #include <conio.h>   // For _kbhit()
  #include <windows.h> // For Sleep()
#endif // WIN32

using namespace ViconDataStreamSDK::CPP;

namespace {

#pragma once
class ViconUnman
{
public:
	
	ViconUnman(std::string hostName);
	//~ViconUnman(void);
	void ViconConnect();
	void ViconDisconnect();
	
	void GetNewFrame();
	void GetSubjectInformation(unsigned int SubjectIndex, double &x,double &y, double &z, double &pitch, double &roll, double &yaw);

	std::string Adapt( const bool i_Value );
	std::string Adapt( const Direction::Enum i_Direction );
	std::string Adapt( const DeviceType::Enum i_DeviceType );
	std::string Adapt( const Unit::Enum i_Unit );

private:
	
	Client MyClient;
	std::string hostName;
	bool TransmitMulticast;

	void EnableDataTypes();

	void SetupAxisMapping();

	void GetCurrentVersion();

};

}