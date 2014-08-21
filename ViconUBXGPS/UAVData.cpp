#include "UAVData.h"


UAVData::UAVData(void){
	
	x = y = z = 0;
	pitch = roll = yaw = 0;
	lat = lon = 0;
}
	
void UAVData::SetX(double x){
	this->x = x;
}
void UAVData::SetY(double y){
	this->y = y;
}
void UAVData::SetZ(double z){
	this->z = z;
}
void UAVData::SetPitch(double pitch){
	this->pitch = pitch;
}
void UAVData::SetRoll(double roll){
	this->roll = roll;
}
void UAVData::SetYaw(double yaw){
	this->yaw = yaw;
}
void UAVData::SetLat(double lat){
	this->lat = lat;
}
void UAVData::SetLon(double lon){
	this->lon = lon;
}
	
void UAVData::SetAlt(double alt){
	
	this->alt = alt;

}

void UAVData::SetAll(double x, double y, double z, double pitch, double roll, double yaw){

	this->x = x;
	this->y = y;
	this->z = z;
	this->pitch = pitch;
	this->roll = roll;
	this->yaw = yaw;
	
}

double UAVData::GetX(){

	return x;
}
double UAVData::GetY(){

	return y;
}
double UAVData::GetZ(){
	return z;
}
double UAVData::GetPitch(){
	return pitch;
}
double UAVData::GetRoll(){
	return roll;
}
double UAVData::GetYaw(){
	return yaw;
}
double UAVData::GetLat(){
	return lat;
}
double UAVData::GetLon(){
	return lon;
}
double UAVData::GetAlt(){
	
	return alt;

}


UAVData::~UAVData(void){
}