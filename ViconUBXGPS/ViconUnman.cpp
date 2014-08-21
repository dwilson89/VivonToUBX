#include "ViconUnman.h"


ViconUnman::ViconUnman(std::string hostName)
{
	//can potentially expand this constructor to have a parametre which can set this value
	TransmitMulticast = false;

	this->hostName = hostName;
}


ViconUnman::~ViconUnman(void)
{
	
}
//

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
        Sleep( 50 );//was originally 200
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
	
	std::cout << "  Subject #" << SubjectIndex << std::endl;

      // Get the subject name
      std::string SubjectName = MyClient.GetSubjectName( SubjectIndex ).SubjectName;
      std::cout << "            Name: " << SubjectName << std::endl;

      // Get the root segment
      std::string RootSegment = MyClient.GetSubjectRootSegmentName( SubjectName ).SegmentName;
      std::cout << "    Root Segment: " << RootSegment << std::endl;

      // Count the number of segments
      unsigned int SegmentCount = MyClient.GetSegmentCount( SubjectName ).SegmentCount;
      std::cout << "    Segments (" << SegmentCount << "):" << std::endl;
      for( unsigned int SegmentIndex = 0 ; SegmentIndex < SegmentCount ; ++SegmentIndex )
      {
        std::cout << "      Segment #" << SegmentIndex << std::endl;

        // Get the segment name
        std::string SegmentName = MyClient.GetSegmentName( SubjectName, SegmentIndex ).SegmentName;
        std::cout << "          Name: " << SegmentName << std::endl;

        // Get the segment parent
        std::string SegmentParentName = MyClient.GetSegmentParentName( SubjectName, SegmentName ).SegmentName;
        std::cout << "        Parent: " << SegmentParentName << std::endl;

        // Get the segment's children
        unsigned int ChildCount = MyClient.GetSegmentChildCount( SubjectName, SegmentName ).SegmentCount;
        std::cout << "     Children (" << ChildCount << "):" << std::endl;
        for( unsigned int ChildIndex = 0 ; ChildIndex < ChildCount ; ++ChildIndex )
        {
          std::string ChildName = MyClient.GetSegmentChildName( SubjectName, SegmentName, ChildIndex ).SegmentName;
          std::cout << "       " << ChildName << std::endl;
        }

        // Get the static segment translation
        Output_GetSegmentStaticTranslation _Output_GetSegmentStaticTranslation = 
          MyClient.GetSegmentStaticTranslation( SubjectName, SegmentName );
        std::cout << "        Static Translation: (" << _Output_GetSegmentStaticTranslation.Translation[ 0 ]  << ", " 
                                                     << _Output_GetSegmentStaticTranslation.Translation[ 1 ]  << ", " 
                                                     << _Output_GetSegmentStaticTranslation.Translation[ 2 ]  << ") " << std::endl;

        // Get the static segment rotation in helical co-ordinates
        Output_GetSegmentStaticRotationHelical _Output_GetSegmentStaticRotationHelical = 
          MyClient.GetSegmentStaticRotationHelical( SubjectName, SegmentName );
        std::cout << "        Static Rotation Helical: (" << _Output_GetSegmentStaticRotationHelical.Rotation[ 0 ]     << ", " 
                                                          << _Output_GetSegmentStaticRotationHelical.Rotation[ 1 ]     << ", " 
                                                          << _Output_GetSegmentStaticRotationHelical.Rotation[ 2 ]     << ") " << std::endl;

        // Get the static segment rotation as a matrix
        Output_GetSegmentStaticRotationMatrix _Output_GetSegmentStaticRotationMatrix = 
          MyClient.GetSegmentStaticRotationMatrix( SubjectName, SegmentName );
        std::cout << "        Static Rotation Matrix: (" << _Output_GetSegmentStaticRotationMatrix.Rotation[ 0 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 1 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 2 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 3 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 4 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 5 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 6 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 7 ]     << ", " 
                                                         << _Output_GetSegmentStaticRotationMatrix.Rotation[ 8 ]     << ") " << std::endl;

        // Get the static segment rotation in quaternion co-ordinates
        Output_GetSegmentStaticRotationQuaternion _Output_GetSegmentStaticRotationQuaternion = 
          MyClient.GetSegmentStaticRotationQuaternion( SubjectName, SegmentName );
        std::cout << "        Static Rotation Quaternion: (" << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 0 ]     << ", " 
                                                             << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 1 ]     << ", " 
                                                             << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 2 ]     << ", " 
                                                             << _Output_GetSegmentStaticRotationQuaternion.Rotation[ 3 ]     << ") " << std::endl;

        // Get the static segment rotation in EulerXYZ co-ordinates
        Output_GetSegmentStaticRotationEulerXYZ _Output_GetSegmentStaticRotationEulerXYZ = 
          MyClient.GetSegmentStaticRotationEulerXYZ( SubjectName, SegmentName );
        std::cout << "        Static Rotation EulerXYZ: (" << _Output_GetSegmentStaticRotationEulerXYZ.Rotation[ 0 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationEulerXYZ.Rotation[ 1 ]     << ", " 
                                                           << _Output_GetSegmentStaticRotationEulerXYZ.Rotation[ 2 ]     << ") " << std::endl;

        // Get the global segment translation - XYZ - i think we want this one
        Output_GetSegmentGlobalTranslation _Output_GetSegmentGlobalTranslation = 
          MyClient.GetSegmentGlobalTranslation( SubjectName, SegmentName );
        std::cout << "        Global Translation: (" << _Output_GetSegmentGlobalTranslation.Translation[ 0 ]  << ", " 
                                                     << _Output_GetSegmentGlobalTranslation.Translation[ 1 ]  << ", " 
                                                     << _Output_GetSegmentGlobalTranslation.Translation[ 2 ]  << ") " 
                                                     << Adapt( _Output_GetSegmentGlobalTranslation.Occluded ) << std::endl;

		x = _Output_GetSegmentGlobalTranslation.Translation[ 0 ];
		y = _Output_GetSegmentGlobalTranslation.Translation[ 1 ];
		z = _Output_GetSegmentGlobalTranslation.Translation[ 2 ];

        // Get the global segment rotation in helical co-ordinates
        Output_GetSegmentGlobalRotationHelical _Output_GetSegmentGlobalRotationHelical = 
          MyClient.GetSegmentGlobalRotationHelical( SubjectName, SegmentName );
        std::cout << "        Global Rotation Helical: (" << _Output_GetSegmentGlobalRotationHelical.Rotation[ 0 ]     << ", " 
                                                          << _Output_GetSegmentGlobalRotationHelical.Rotation[ 1 ]     << ", " 
                                                          << _Output_GetSegmentGlobalRotationHelical.Rotation[ 2 ]     << ") " 
                                                          << Adapt( _Output_GetSegmentGlobalRotationHelical.Occluded ) << std::endl;

        // Get the global segment rotation as a matrix
        Output_GetSegmentGlobalRotationMatrix _Output_GetSegmentGlobalRotationMatrix = 
          MyClient.GetSegmentGlobalRotationMatrix( SubjectName, SegmentName );
        std::cout << "        Global Rotation Matrix: (" << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 0 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 1 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 2 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 3 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 4 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 5 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 6 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 7 ]     << ", " 
                                                         << _Output_GetSegmentGlobalRotationMatrix.Rotation[ 8 ]     << ") " 
                                                         << Adapt( _Output_GetSegmentGlobalRotationMatrix.Occluded ) << std::endl;

        // Get the global segment rotation in quaternion co-ordinates
        Output_GetSegmentGlobalRotationQuaternion _Output_GetSegmentGlobalRotationQuaternion = 
          MyClient.GetSegmentGlobalRotationQuaternion( SubjectName, SegmentName );
        std::cout << "        Global Rotation Quaternion: (" << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 0 ]     << ", " 
                                                             << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 1 ]     << ", " 
                                                             << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 2 ]     << ", " 
                                                             << _Output_GetSegmentGlobalRotationQuaternion.Rotation[ 3 ]     << ") " 
                                                             << Adapt( _Output_GetSegmentGlobalRotationQuaternion.Occluded ) << std::endl;

        // Get the global segment rotation in EulerXYZ co-ordinates - And we want this one;
        Output_GetSegmentGlobalRotationEulerXYZ _Output_GetSegmentGlobalRotationEulerXYZ = 
          MyClient.GetSegmentGlobalRotationEulerXYZ( SubjectName, SegmentName );
        std::cout << "        Global Rotation EulerXYZ: (" << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 0 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 1 ]     << ", " 
                                                           << _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 2 ]     << ") " 
                                                           << Adapt( _Output_GetSegmentGlobalRotationEulerXYZ.Occluded ) << std::endl;
		
		roll = _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 0 ] ;
		pitch = _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 1 ];
		yaw = _Output_GetSegmentGlobalRotationEulerXYZ.Rotation[ 2 ];

        // Get the local segment translation
        Output_GetSegmentLocalTranslation _Output_GetSegmentLocalTranslation = 
          MyClient.GetSegmentLocalTranslation( SubjectName, SegmentName );
        std::cout << "        Local Translation: (" << _Output_GetSegmentLocalTranslation.Translation[ 0 ]  << ", " 
                                                    << _Output_GetSegmentLocalTranslation.Translation[ 1 ]  << ", " 
                                                    << _Output_GetSegmentLocalTranslation.Translation[ 2 ]  << ") " 
                                                    << Adapt( _Output_GetSegmentLocalTranslation.Occluded ) << std::endl;

        // Get the local segment rotation in helical co-ordinates
        Output_GetSegmentLocalRotationHelical _Output_GetSegmentLocalRotationHelical = 
          MyClient.GetSegmentLocalRotationHelical( SubjectName, SegmentName );
        std::cout << "        Local Rotation Helical: (" << _Output_GetSegmentLocalRotationHelical.Rotation[ 0 ]     << ", " 
                                                         << _Output_GetSegmentLocalRotationHelical.Rotation[ 1 ]     << ", " 
                                                         << _Output_GetSegmentLocalRotationHelical.Rotation[ 2 ]     << ") " 
                                                         << Adapt( _Output_GetSegmentLocalRotationHelical.Occluded ) << std::endl;

        // Get the local segment rotation as a matrix
        Output_GetSegmentLocalRotationMatrix _Output_GetSegmentLocalRotationMatrix = 
          MyClient.GetSegmentLocalRotationMatrix( SubjectName, SegmentName );
        std::cout << "        Local Rotation Matrix: (" << _Output_GetSegmentLocalRotationMatrix.Rotation[ 0 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 1 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 2 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 3 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 4 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 5 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 6 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 7 ]     << ", " 
                                                        << _Output_GetSegmentLocalRotationMatrix.Rotation[ 8 ]     << ") " 
                                                        << Adapt( _Output_GetSegmentLocalRotationMatrix.Occluded ) << std::endl;

        // Get the local segment rotation in quaternion co-ordinates
        Output_GetSegmentLocalRotationQuaternion _Output_GetSegmentLocalRotationQuaternion = 
          MyClient.GetSegmentLocalRotationQuaternion( SubjectName, SegmentName );
        std::cout << "        Local Rotation Quaternion: (" << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 0 ]     << ", " 
                                                            << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 1 ]     << ", " 
                                                            << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 2 ]     << ", " 
                                                            << _Output_GetSegmentLocalRotationQuaternion.Rotation[ 3 ]     << ") " 
                                                            << Adapt( _Output_GetSegmentLocalRotationQuaternion.Occluded ) << std::endl;

        // Get the local segment rotation in EulerXYZ co-ordinates
        Output_GetSegmentLocalRotationEulerXYZ _Output_GetSegmentLocalRotationEulerXYZ = 
          MyClient.GetSegmentLocalRotationEulerXYZ( SubjectName, SegmentName );
        std::cout << "        Local Rotation EulerXYZ: (" << _Output_GetSegmentLocalRotationEulerXYZ.Rotation[ 0 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationEulerXYZ.Rotation[ 1 ]     << ", " 
                                                          << _Output_GetSegmentLocalRotationEulerXYZ.Rotation[ 2 ]     << ") " 
                                                          << Adapt( _Output_GetSegmentLocalRotationEulerXYZ.Occluded ) << std::endl;
      }

      // Count the number of markers
      unsigned int MarkerCount = MyClient.GetMarkerCount( SubjectName ).MarkerCount;
      std::cout << "    Markers (" << MarkerCount << "):" << std::endl;
      for( unsigned int MarkerIndex = 0 ; MarkerIndex < MarkerCount ; ++MarkerIndex )
      {
        // Get the marker name
        std::string MarkerName = MyClient.GetMarkerName( SubjectName, MarkerIndex ).MarkerName;

        // Get the marker parent
        std::string MarkerParentName = MyClient.GetMarkerParentName( SubjectName, MarkerName ).SegmentName;

        // Get the global marker translation
        Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
          MyClient.GetMarkerGlobalTranslation( SubjectName, MarkerName );

        std::cout << "      Marker #" << MarkerIndex            << ": "
                                      << MarkerName             << " ("
                                      << _Output_GetMarkerGlobalTranslation.Translation[ 0 ]  << ", "
                                      << _Output_GetMarkerGlobalTranslation.Translation[ 1 ]  << ", "
                                      << _Output_GetMarkerGlobalTranslation.Translation[ 2 ]  << ") "
                                      << Adapt( _Output_GetMarkerGlobalTranslation.Occluded ) << std::endl;


		//check for a bad reading
		if(CheckForBadReading(x, y, z, roll, pitch, yaw)){

			x = y = z = pitch = roll = yaw = FLOAT_MAX;

		}
	}
}

bool ViconUnman::CheckForBadReading(double x, double y, double z, double roll, double pitch, double yaw){

	if(x == 0 && y == 0 && z == 0 && roll == 0 && pitch == 0 && yaw == 0){
		return true;
	}
	
	return false;

}

//if u need it;
void ViconUnman::GetUnlabledData(){
	
	// Get the unlabeled markers
    unsigned int UnlabeledMarkerCount = MyClient.GetUnlabeledMarkerCount().MarkerCount;
    std::cout << "  Unlabeled Markers (" << UnlabeledMarkerCount << "):" << std::endl;
    for( unsigned int UnlabeledMarkerIndex = 0 ; UnlabeledMarkerIndex < UnlabeledMarkerCount ; ++UnlabeledMarkerIndex )
    { 
      // Get the global marker translation
      Output_GetUnlabeledMarkerGlobalTranslation _Output_GetUnlabeledMarkerGlobalTranslation =
        MyClient.GetUnlabeledMarkerGlobalTranslation( UnlabeledMarkerIndex );

      std::cout << "      Marker #" << UnlabeledMarkerIndex   << ": ("
                                    << _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 0 ] << ", "
                                    << _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 1 ] << ", "
                                    << _Output_GetUnlabeledMarkerGlobalTranslation.Translation[ 2 ] << ") " << std::endl;
    }

    // Count the number of devices
    unsigned int DeviceCount = MyClient.GetDeviceCount().DeviceCount;
    std::cout << "  Devices (" << DeviceCount << "):" << std::endl;
    for( unsigned int DeviceIndex = 0 ; DeviceIndex < DeviceCount ; ++DeviceIndex )
    {
      std::cout << "    Device #" << DeviceIndex << ":" << std::endl;

      // Get the device name and type
      Output_GetDeviceName _Output_GetDeviceName = MyClient.GetDeviceName( DeviceIndex );
      std::cout << "      Name: " << _Output_GetDeviceName.DeviceName << std::endl;
      std::cout << "      Type: " << Adapt( _Output_GetDeviceName.DeviceType ) << std::endl;

      // Count the number of device outputs
      unsigned int DeviceOutputCount = MyClient.GetDeviceOutputCount( _Output_GetDeviceName.DeviceName ).DeviceOutputCount;
      std::cout << "      Device Outputs (" << DeviceOutputCount << "):" << std::endl;
      for( unsigned int DeviceOutputIndex = 0 ; DeviceOutputIndex < DeviceOutputCount ; ++DeviceOutputIndex )
      {
        // Get the device output name and unit
        Output_GetDeviceOutputName _Output_GetDeviceOutputName = 
          MyClient.GetDeviceOutputName( _Output_GetDeviceName.DeviceName, DeviceOutputIndex );

        // Get the device output value
        Output_GetDeviceOutputValue _Output_GetDeviceOutputValue = 
          MyClient.GetDeviceOutputValue( _Output_GetDeviceName.DeviceName, _Output_GetDeviceOutputName.DeviceOutputName );

        std::cout << "        Device Output #" << DeviceOutputIndex                                     << ": "
                                               << _Output_GetDeviceOutputName.DeviceOutputName          << " "
                                               << _Output_GetDeviceOutputValue.Value                    << " " 
                                               << Adapt( _Output_GetDeviceOutputName.DeviceOutputUnit ) << " " 
                                               << Adapt( _Output_GetDeviceOutputValue.Occluded )        << std::endl;
      }
    }

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


