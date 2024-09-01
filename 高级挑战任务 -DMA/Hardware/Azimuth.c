#include "stm32f10x.h"                  // Device header
#include <math.h>

#include "Serial.h"
#include "ATGM336H.h"
#define DEG_TO_RAD(x) ((x) * 3.14159 / 180)
//Lat,Lon是定位模块的；RLat,RLon是蓝牙模块的

uint8_t Servo_Initial_Direction  = 1;
double Calculate_Azimuth(double lat1, double lon1, double lat2, double lon2) 
{
     double phi1 = DEG_TO_RAD(lat1);
     double lambda1 = DEG_TO_RAD(lon1);
     double phi2 = DEG_TO_RAD(lat2);
     double lambda2 = DEG_TO_RAD(lon2);
	//转换为弧度
	
	double delta_lambda = lambda2 - lambda1;//计算经度差
	
	double y = sin(delta_lambda) * cos(phi2);
	double x = cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(delta_lambda);
	double azimuth = atan2(y, x);//计算方位角
	
	azimuth = azimuth * (180.0 / 3.14159);
	//azimuth计算出来的范围是-180°到180°
	if (azimuth < 0)
	{
		Servo_Initial_Direction = 0;
		azimuth += 360;
	}
	else if (azimuth >=0&&azimuth <=180)
	{
		Servo_Initial_Direction = 1;
	}
	if(azimuth>360)
	{
		azimuth -=360;
	}
	return azimuth;
}
