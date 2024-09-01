#ifndef __MPU6050_H
#define __MPU6050_H


extern float pitch;
extern float roll ;
	
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);

uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);

void MPU6050_GetData(int16_t * AccX,int16_t * AccY,int16_t * AccZ,
								int16_t * GyroX,int16_t * GyroY,int16_t *GyroZ);
uint8_t MPU6050_GetID(void);

void Pose_Calculating(int16_t AX,int16_t AY,int16_t AZ,int16_t GX,int16_t GY,int16_t GZ);
#endif
