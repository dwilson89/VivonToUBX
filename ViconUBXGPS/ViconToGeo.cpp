#include "ViconToGeo.h"


//#include "Vicon.h"
using namespace std;
//using namespace GeographicLib;

ViconToGeo::ViconToGeo(int numUAV){
	uavs = gcnew List<UAVData^>();
	this->numUAV = numUAV; 

	//MAVLINK_MESSAGE_CRCS = gcnew array<unsigned char>(256){50, 124, 137, 0, 237, 217, 104, 119, 0, 0, 0, 89, 0, 0, 0, 0, 0, 0, 0, 0, 214, 159, 220, 168, 24, 23, 170, 144, 67, 115, 39, 246, 185, 104, 237, 244, 222, 212, 9, 254, 230, 28, 28, 132, 221, 232, 11, 153, 41, 39, 214, 223, 141, 33, 15, 3, 100, 24, 239, 238, 0, 0, 183, 0, 130, 0, 148, 21, 0, 52, 124, 0, 0, 0, 20, 0, 152, 143, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 183, 63, 54, 0, 0, 0, 0, 0, 0, 0, 19, 102, 158, 208, 56, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 134, 219, 208, 188, 84, 22, 19, 21, 134, 0, 78, 68, 189, 127, 42, 21, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 204, 49, 170, 44, 83, 46, 247};

	//create new uav objects for each uav 
	for(int i = 0; i < numUAV; i++){

		uavs->Add(gcnew UAVData());

	}

}

System::String^ ViconToGeo::ToPayload(int uavIndex){
	
	System::String^ tempMessage;
	StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;

	msgBuff_t outMsg;

	float x, y, z;
	float lat, lon, alt;
	float pitch, roll, yaw;
	
	unsigned long long timestamp = 0; //Timestamp (microseconds, synced to UNIX time or since system boot)	
	
	//initialise vars
	x = uavs[uavIndex]->GetX();
	y = uavs[uavIndex]->GetY();
	z = uavs[uavIndex]->GetZ();

	lat = uavs[uavIndex]->GetLat();
	lon = uavs[uavIndex]->GetLon();
	alt = uavs[uavIndex]->GetAlt();

	pitch = uavs[uavIndex]->GetPitch();
	roll = uavs[uavIndex]->GetRoll();
	yaw = uavs[uavIndex]->GetYaw();

	//should possibly work - not sure if i would need to place this esle where
	time_t ltime;
    time(&ltime);
	//printf("\n\nCurrent local time as unix timestamp: %li\n\n", ltime);
	timestamp = ltime;

	rawData = gcnew System::String(RawDataString(x, y, z, lat, lon, alt, pitch, roll, yaw).c_str());

	//This coverts the string to a format in which it can then send to the serial port standard string to system string
	//tempMessage = gcnew System::String(ComLatLonMessage(lat, lon).c_str());

	

	//Console::Write(tempMessage);
	
				
	std::string test = "";
	compileMessage(outMsg, timestamp, x, y, z, roll, pitch, yaw);

	for(int i=0;i<outMsg.byteCount;i++)	{
		char c = outMsg.inputs[i];
		
		cout << c;
		test +=c;

	}

	//std::string test = outMsg.inputs;
	
	//std::string test = reinterpret_cast<unsigned char*>(outMsg.inputs);
	
	//std::string test(outMsg.inputs,outMsg.inputs + outMsg.byteCount);

	Console::Write("\n");
	
	Console::Write("\n");
	tempMessage = gcnew System::String(test.c_str());
	Console::Write(tempMessage);
	Console::Write("\n");
	message = tempMessage;
	
	return message;

}

void ViconToGeo::SendPayload(int uavIndex, SerialWriter^ sw){
	
	System::String^ tempMessage;
	StringComparer^ stringComparer = StringComparer::OrdinalIgnoreCase;

	msgBuff_t outMsg;

	float x, y, z;
	float lat, lon, alt;
	float pitch, roll, yaw;
	
	unsigned long long timestamp = 0; //Timestamp (microseconds, synced to UNIX time or since system boot)	
	
	//initialise vars
	x = uavs[uavIndex]->GetX();
	y = uavs[uavIndex]->GetY();
	z = uavs[uavIndex]->GetZ();

	lat = uavs[uavIndex]->GetLat();
	lon = uavs[uavIndex]->GetLon();
	alt = uavs[uavIndex]->GetAlt();

	pitch = uavs[uavIndex]->GetPitch();
	roll = uavs[uavIndex]->GetRoll();
	yaw = uavs[uavIndex]->GetYaw();

	//should possibly work - not sure if i would need to place this esle where
	time_t ltime;
    time(&ltime);
	//printf("\n\nCurrent local time as unix timestamp: %li\n\n", ltime);
	timestamp = ltime;

	rawData = gcnew System::String(RawDataString(x, y, z, lat, lon, alt, pitch, roll, yaw).c_str());

	//This coverts the string to a format in which it can then send to the serial port standard string to system string
	//tempMessage = gcnew System::String(ComLatLonMessage(lat, lon).c_str());

	

	//Console::Write(tempMessage);
	
				
	std::string test = "";
	compileMessage(outMsg, timestamp, x, y, z, roll, pitch, yaw);


	sw->Send(outMsg.inputs, outMsg.byteCount);

	std::string mavlinkStr ="";

	char buff[10];

	for(int i=0;i<outMsg.byteCount;i++)	{
		unsigned char c = outMsg.inputs[i];
		
		sprintf(buff, "%02X ", c);
		mavlinkStr += buff;

	}

	//cout << (mavlinkStr);

	//std::string test = outMsg.inputs;
	
	//std::string test = reinterpret_cast<unsigned char*>(outMsg.inputs);
	
	//std::string test(outMsg.inputs,outMsg.inputs + outMsg.byteCount);


	tempMessage = gcnew System::String(mavlinkStr.c_str());

	message = tempMessage;

}


System::String^ ViconToGeo::GetStringRep(){

	return rawData;
}

string ViconToGeo::ComLatLonMessage(double lat, double lon){
	char latLonBuff[100];
	string latLonStr;
	
	sprintf_s(latLonBuff, "%Lf %Lf", lat, lon);
	latLonStr = latLonBuff;
	return latLonStr;
}



System::String^ ViconToGeo::GetSentMessage(){

	return message;
}

string ViconToGeo::RawDataString(double x, double y, double z, double lat, double lon, double alt, double pitch, double roll, double yaw){

	char stringBuff[250];
	string rawStr;
	
	sprintf_s(stringBuff,"x: %Lf \ny: %Lf \nz: %Lf \nLatitude: %Lf \nLontitude: %Lf \nAltitude: %Lf \nPitch: %Lf \nRoll: %Lf \nYaw: %Lf \n",x, y, z, lat, lon, alt, pitch, roll, yaw);

	rawStr = stringBuff;

	return rawStr;

}

string ViconToGeo::RawDataString(double x, double y, double z, double pitch, double roll, double yaw){

	char stringBuff[250];
	string rawStr;
	
	sprintf_s(stringBuff,"x: %Lf \ny: %Lf \nz: %Lf \nLatitude: %Lf \nLontitude: %Lf \nAltitude: %Lf \nPitch: %Lf \nRoll: %Lf \nYaw: %Lf \n",x, y, z, pitch, roll, yaw);

	rawStr = stringBuff;

	return rawStr;

}


//purpose is to grab the current packet of data from vicon server and assign all the information to the relavent UAVS
void ViconToGeo::GetandAssignViconData(){

	double x, y, z, pitch, roll, yaw;

	//viconConnect.GetNewFrame();

	viconConnect->GetNewFrame();

	for(int i = 0; i < numUAV; i++){

		viconConnect->GetSubjectInformation(i,x,y,z,pitch,roll,yaw);

		//viconConnect.GetSubjectInformation(i,x,y,z,pitch,roll,yaw);

		uavs[i]->SetAll(x,y,z,pitch,roll,yaw);

	}
}

void ViconToGeo::ConnectToVicon(std::string hostName){

	char *hostN = new char[hostName.length() + 1];
	strcpy(hostN, hostName.c_str());
	viconConnect = new ViconUnman(hostName);
	viconConnect->ViconConnect();
	//viconConnect.SetDefaultValues(hostN);

	//viconConnect.ViconConnect();
}

void ViconToGeo::DisconnectFromVicon(){
	
	viconConnect->ViconDisconnect();
	//viconConnect.ViconDisconnect();
}



//purpose is to convert all the vicon data for each uav to its lat lon equivalent
//void ViconToGeo::ConvertViconToGeo(){
//
//	double x, y, z;
//	double lat, lon, alt;
//
//	bool testing = true;
//
//	try {
//		
//		//Geocentric earth(Constants::WGS84_a(), Constants::WGS84_f());
//		const Geocentric& earth = Geocentric::WGS84;
//		// const double lat0 = 48 + 50/60.0, lon0 = 2 + 20/60.0; // Paris
//		const double lat0 = 0, lon0 = 0; // center
//		LocalCartesian proj(lat0, lon0, 0, earth);
//   
//		{
//
//			for(int i = 0; i < numUAV; i++){
//
//				//grab each uavs x, y, z
//				x = uavs[i]->GetX();
//				y = uavs[i]->GetY();
//				z = uavs[i]->GetZ();
//
//				//remove once vicon is implemented
//				if(testing){
//					x = x + 0.1;
//					y = y + 0.1;
//				}
//
//				proj.Reverse(x, y, z, lat, lon, alt);
//
//				//set each uavs lat, lon, alt
//				uavs[i]->SetLat(lat);
//				uavs[i]->SetLon(lon);
//				uavs[i]->SetAlt(alt);
//
//			}
//			
//		}
//	}
//		catch (const exception& e) {
//			cerr << "Caught exception: " << e.what() << "\n";
//	}
//
//}
void ViconToGeo::compileMessage(msgBuff_t &msg, unsigned long long time, float x, float y, float z, float r, float p, float q)
{
	unsigned int crcTemp = 0;
	msg.byteCount = 40;													//6+32+2

	//Header
	msg.inputs[0] = 0xFE;												//Header
	msg.inputs[1] = PAY_LEN_VICON_DATA;								//Payload Length
	msg.inputs[2] = 0x00;												//Sequence - THIS WILL NEED TO BE DYNAMICALLY CHANGED I THINK
	msg.inputs[3] = 0xFF;												//System ID
	msg.inputs[4] = 0x00;												//Component ID
	msg.inputs[5] = CMD_ID_VICON_DATA;									//Packet ID
	
	//Payload
	packVariableUint64(msg, time, 0);									//0 for the 0th position in the payload
	packVariableFloat(msg,x,8);											//This layout is only for VICON_POSITION_ESTIMATE
	packVariableFloat(msg,y,12);
	packVariableFloat(msg,z,16);
	packVariableFloat(msg,r,20);
	packVariableFloat(msg,p,24);
	packVariableFloat(msg,q,28);
	
	//Cyclic Redundancy Check
	crcTemp = crc_calculate(msg.inputs, msg.byteCount-2);				//Calculate and CRC for the gathered message, excluding the additional count for the CRC
	crcTemp = crc_accumulate(MAVLINK_MESSAGE_CRCS[msg.inputs[5]], crcTemp);	//Use magic to add to the crcTemp value. Basically just need to run the ID seed through the accumulator one final time

	msg.inputs[38] = (unsigned char)crcTemp;							//CRC LSB
	msg.inputs[39] = (unsigned char)(crcTemp>>8);						//CRC MSB
	

}

void ViconToGeo::packVariableFloat(msgBuff_t &msg, float input, unsigned char payPos)
{

	int * ptr2Flt = (int*)(&input); // this is to trick it - You make a pointer that points to the float with same number of bytes, then you shift based on the dereferenced pointer. At this point, when we manipulate *ptr, we're actually manipulating x.
	for(int i=0;i<4;i++)
	{
		msg.inputs[6+payPos+i] = (unsigned char)(*ptr2Flt>>(i*8));			//This method /may/ work...
	}
}

void ViconToGeo::packVariableUint64(msgBuff_t &msg, unsigned long long input, unsigned char payPos)
{

	for(int i=0;i<8;i++)
	{
		msg.inputs[6+payPos+i] = (unsigned char)(input>>(i*8));			//This method /may/ work...
	}
}

void ViconToGeo::packVariableUint16(msgBuff_t &msg, unsigned int input, unsigned char payPos)
{
	for(int i=0;i<2;i++)
	{
		msg.inputs[6+payPos+i] = (unsigned char)(input>>(i*8));			//This method /may/ work...
	}
}

uint16_t ViconToGeo::crc_accumulate(unsigned char b, uint16_t crc)			//More Magic
{
	unsigned char ch = (unsigned char)(b ^ (unsigned char)(crc & 0x00ff));
	ch = (unsigned char)(ch ^ (ch << 4));
	return (uint16_t)((crc >> 8) ^ (ch << 8) ^ (ch << 3) ^ (ch >> 4));
}

uint16_t ViconToGeo::crc_calculate(unsigned char* pBuffer, int length)		//Magic
{
	if (length < 1)
	{
		return 0xffff;
	}

	uint16_t crcTmp;
	int i;

	crcTmp = X25_INIT_CRC;

	for (i = 1; i < length; i++) 										// skips header
	{
		crcTmp = crc_accumulate(pBuffer[i], crcTmp);
	}

	return (crcTmp);
}

//void ViconToGeo::sendMessage(msgBuff_t outMsg)
//{	
//	unsigned int i = 0;
//	unsigned char c = 0xFE;												//Could be 0
//		
//	for(i=0;i<outMsg.byteCount;i++)
//	{
//		c = outMsg.inputs[i];
//		
//		Serial.write(c);
//	}
//}

void ViconToGeo::clearMsgBuff(msgBuff_t &msg)										//Resets the provided buffer struct
{
	msg.byteCount = 0;
	
  	for(int k=0;k<MAX_MAVLINK_LENGTH;k++)
	{
		msg.inputs[k] = 0;
	}
}