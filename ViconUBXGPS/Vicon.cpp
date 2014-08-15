#include "Vicon.h"


ViconUnman::ViconUnman(std::string hostName)
{
	//can potentially expand this constructor to have a parametre which can set this value
	TransmitMulticast = false;

	this->hostName = hostName;
}


//ViconUnman::~ViconUnman(void)
//{
	
//}


void ViconUnman::ViconConnect(){
	
	//for the time being this might cause the program to freeze up until it connects to vicon
	//It should also be noted for the intial setup all data will be printed to the console, all other data will be printed to the GUI

	// Connect to a server
	std::cout << "Connecting to " << hostName << " ..." << std::flush;
	while( !MyClient.IsConnected().Connected )
	{
		// Direct connection
		MyClient.Connect( hostName );

		// Multicast connection
		// MyClient.ConnectToMulticast( HostName, "224.0.0.0" );

		std::cout << ".";
	#ifdef WIN32
		Sleep( 200 );
	#else
		sleep(1);
	#endif
	}
	
	std::cout << std::endl;
	EnableDataTypes();

	// Set the streaming mode
	// Should be noted that ClientPull works the best for multiple objects being tracked
	MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
	// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
	// MyClient.SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

	SetupAxisMapping();
	GetCurrentVersion();

	// will never be this for the time being
	if( TransmitMulticast )
	{
		MyClient.StartTransmittingMulticast( "localhost", "224.0.0.0" );
	}

}

void ViconUnman::ViconDisconnect(){
	
	if( TransmitMulticast ){
		MyClient.StopTransmittingMulticast();
	}

	// Disconnect and dispose
	MyClient.Disconnect();

}


void ViconUnman::EnableDataTypes(){
	
	// Enable some different data types
	MyClient.EnableSegmentData();
	MyClient.EnableMarkerData();
	MyClient.EnableUnlabeledMarkerData();
	MyClient.EnableDeviceData();

	//Can Potentially add more depending on what you need

	std::cout << "Segment Data Enabled: "          << Adapt( MyClient.IsSegmentDataEnabled().Enabled )         << std::endl;
	std::cout << "Marker Data Enabled: "           << Adapt( MyClient.IsMarkerDataEnabled().Enabled )          << std::endl;
	std::cout << "Unlabeled Marker Data Enabled: " << Adapt( MyClient.IsUnlabeledMarkerDataEnabled().Enabled ) << std::endl;
	std::cout << "Device Data Enabled: "           << Adapt( MyClient.IsDeviceDataEnabled().Enabled )          << std::endl;

}

//add in an option and have a if statements for choice of mapping z up or y up
void ViconUnman::SetupAxisMapping(){
  
	// Set the global up axis
	MyClient.SetAxisMapping( Direction::Forward, 
		                     Direction::Left, 
			                 Direction::Up ); // Z-up
	// MyClient.SetGlobalUpAxis( Direction::Forward, 
	//                           Direction::Up, 
	//                           Direction::Right ); // Y-up

	// This outputs the setting to make sure the user knows what the current Axis Mapping is
	Output_GetAxisMapping _Output_GetAxisMapping = MyClient.GetAxisMapping();
	std::cout << "Axis Mapping: X-" << Adapt( _Output_GetAxisMapping.XAxis ) 
                         << " Y-" << Adapt( _Output_GetAxisMapping.YAxis ) 
                         << " Z-" << Adapt( _Output_GetAxisMapping.ZAxis ) << std::endl;



}

//Just an option to let the use know the current version
void ViconUnman::GetCurrentVersion(){
	// Discover the version number
	Output_GetVersion _Output_GetVersion = MyClient.GetVersion();
	std::cout << "Version: " << _Output_GetVersion.Major << "." 
						     << _Output_GetVersion.Minor << "." 
                             << _Output_GetVersion.Point << std::endl;
}


void ViconUnman::GetNewFrame(){
	
	// Get a frame
    std::cout << "Waiting for new frame...";
    while( MyClient.GetFrame().Result != Result::Success )
    {
      // Sleep a little so that we don't lumber the CPU with a busy poll
      #ifdef WIN32
        Sleep( 200 );
      #else
        sleep(1);
      #endif

      std::cout << ".";
    }
    std::cout << std::endl;

    // Get the frame number
    Output_GetFrameNumber _Output_GetFrameNumber = MyClient.GetFrameNumber();
    std::cout << "Frame Number: " << _Output_GetFrameNumber.FrameNumber << std::endl;

    // Get the timecode
    Output_GetTimecode _Output_GetTimecode  = MyClient.GetTimecode();

    std::cout << "Timecode: "
              << _Output_GetTimecode.Hours               << "h "
              << _Output_GetTimecode.Minutes             << "m " 
              << _Output_GetTimecode.Seconds             << "s "
              << _Output_GetTimecode.Frames              << "f "
              << _Output_GetTimecode.SubFrame            << "sf "
              << Adapt( _Output_GetTimecode.FieldFlag ) << " " 
              << _Output_GetTimecode.Standard            << " " 
              << _Output_GetTimecode.SubFramesPerFrame   << " " 
              << _Output_GetTimecode.UserBits            << std::endl << std::endl;

    // Get the latency
    std::cout << "Latency: " << MyClient.GetLatencyTotal().Total << "s" << std::endl;
    
    for( unsigned int LatencySampleIndex = 0 ; LatencySampleIndex < MyClient.GetLatencySampleCount().Count ; ++LatencySampleIndex )
    {
      std::string SampleName  = MyClient.GetLatencySampleName( LatencySampleIndex ).Name;
      double      SampleValue = MyClient.GetLatencySampleValue( SampleName ).Value;

      std::cout << "  " << SampleName << " " << SampleValue << "s" << std::endl;
    }
    std::cout << std::endl;

	// Doesn't need to be here - just to let the user know how many subjects has been detected in the new frame - might need to be kept to keep track of global
	// Count the number of subjects
    unsigned int SubjectCount = MyClient.GetSubjectCount().SubjectCount;
    std::cout << "Subjects (" << SubjectCount << "):" << std::endl;

}

void ViconUnman::GetSubjectInformation(unsigned int SubjectIndex, double &x,double &y, double &z, double &pitch, double &roll, double &yaw){

}

//code from the ViconDataStreamSDK_CPPTest.cpp
std::string ViconUnman::Adapt( const bool i_Value )
{
	return i_Value ? "True" : "False";
}

std::string ViconUnman::Adapt( const Direction::Enum i_Direction )
{
	switch( i_Direction )
	{
		case Direction::Forward:
			return "Forward";
		case Direction::Backward:
			return "Backward";
		case Direction::Left:
			return "Left";
		case Direction::Right:
			return "Right";
		case Direction::Up:
			return "Up";
		case Direction::Down:
			return "Down";
		default:
			return "Unknown";
	}
}

std::string ViconUnman::Adapt( const DeviceType::Enum i_DeviceType )
{
	switch( i_DeviceType )
	{
		case DeviceType::ForcePlate:
			return "ForcePlate";
		case DeviceType::Unknown:
		default:
			return "Unknown";
	}
}

std::string ViconUnman::Adapt( const Unit::Enum i_Unit )
{
	switch( i_Unit )
	{
		case Unit::Meter:
			return "Meter";
		case Unit::Volt:
			return "Volt";
		case Unit::NewtonMeter:
			return "NewtonMeter";
		case Unit::Newton:
			return "Newton";
		case Unit::Unknown:
		default:
			return "Unknown";
	}
}


