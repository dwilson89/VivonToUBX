#include "SerialWriter.h"


SerialWriter::SerialWriter(String^ portName, int baudRate)
{
	// TODO: Fix variables

	_serialPort = gcnew SerialPort();
	//prolly need some default values here eventually

	// Allow the user to set the appropriate properties.
    _serialPort->PortName = portName;
    _serialPort->BaudRate = baudRate;
    //_serialPort->Parity = (Parity)Enum::Parse(Parity::typeid, "None");
    _serialPort->DataBits = 8;
    //_serialPort->StopBits = (StopBits)Enum::Parse(StopBits::typeid, "One");
    //_serialPort->Handshake = (Handshake)Enum::Parse(Handshake::typeid, "None");

    // Set the read/write timeouts
    _serialPort->ReadTimeout = 200;
    _serialPort->WriteTimeout = 200;
	Open();
}

void SerialWriter::Open(){
	if (!IsOpen()){
		_serialPort->Open();
	}
}

SerialWriter::~SerialWriter(){
	Close();
}

Boolean SerialWriter::IsOpen(){
	return _serialPort->IsOpen;
}

void SerialWriter::Close(){
	if (IsOpen()){
		_serialPort->Close();
	}
}

void SerialWriter::Send(String^ payload){
	if (IsOpen()){
		_serialPort->Write(payload);
	} else {
		// TODO: Deal with it
	}
}

void SerialWriter::Send(unsigned char* c,unsigned int byteCount){
	
	array<unsigned char>^ texBufArray = gcnew array<unsigned char>(byteCount);
	for (int i=0; i<byteCount; i++)
		texBufArray[i] = c[i];

	if (IsOpen()){
		_serialPort->Write(texBufArray,0,byteCount);

	} else {
		// TODO: Deal with it
	}
}

String^ SerialWriter::Read(){
	String^ message;
    try
    {
        message = _serialPort->ReadLine();
        //Console::WriteLine(message);
		
    }
    catch (TimeoutException ^) { }
	return message;
}