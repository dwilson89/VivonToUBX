#pragma once
ref class UAVData
{
public:
	
	UAVData(void);
	
	void SetX(double x);
	void SetY(double y);
	void SetZ(double z);
	void SetPitch(double pitch);
	void SetRoll(double roll);
	void SetYaw(double yaw);
	void SetLat(double lat);
	void SetLon(double lon);
	void SetAlt(double alt);
	void SetAll(double x, double y, double z, double pitch, double roll, double yaw);


	double GetX();
	double GetY();
	double GetZ();
	double GetPitch();
	double GetRoll();
	double GetYaw();
	double GetLat();
	double GetLon();
	double GetAlt();

private:

	double x,y,z;
	double pitch, roll, yaw;
	double lat, lon, alt;

protected:

	~UAVData(void);
};

