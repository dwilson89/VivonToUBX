#pragma once

using namespace System::IO::Ports;
using namespace System;

ref class SerialWriter
{
public:
	SerialWriter(System::String^ portName, int baudRate);
	void Open();

	Boolean IsOpen();
	void Close();
	void Send(System::String^ payload);
	System::String^ Read();
	void Send(unsigned char* c,unsigned int byteCount);

private:
	SerialPort^ _serialPort;
protected:
	~SerialWriter();
};

