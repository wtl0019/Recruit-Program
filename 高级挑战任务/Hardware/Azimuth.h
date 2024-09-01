#ifndef __AZIMUTH_H
#define __AZIMUTH_H
#define DEG_TO_RAD(x) ((x) * M_PI / 180.0)
double Calculate_Azimuth(double lat1, double lon1, double lat2, double lon2) ;
 extern uint8_t Servo_Initial_Direction;
#endif

