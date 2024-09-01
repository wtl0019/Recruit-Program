#ifndef __SERVO_H
#define __SERVO_H
void Servo_Init(void);

void Servo_SetAngle(float Angle);

void Servo_Swing(float S1,float Start,float End);
#endif
