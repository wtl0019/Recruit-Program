#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"
#include "Serial.h"
#include "ATGM336H.h"
#include "BlueTooth.h"
#include "Azimuth.h"
#include "MPU6050.h"
#include "MyDMA.h"
int16_t AX,AY,AZ,GX,GY,GZ;


int main(void){
	OLED_Init();
	Servo_Init() ;
	Key_Init();
	ATGM_StructInit();
	Serial2_Init();
	
	DataPrase();
	DataConvet();//对接收的定位数据进行处理
	
	while(GPS.isUsefull != 1);//等待定位模块的数据处理完成
	Serial1_AD_Init();
	
	
	 
	while(1){
		
	MPU6050_GetData (&AX,&AY,&AZ,&GX,&GY,&GZ);
	Pose_Calculating(AX, AY, AZ, GX,GY, GZ);
	OLED_ShowNum (1,1,pitch*1000000,10);
	OLED_ShowNum (2,1,roll*1000000,10);
	BTDataCon();//蓝牙模块数据转换	
	
	double Azimuth =  Calculate_Azimuth( Lat,  Lon,  RLat,RLon  ) ;//方位角计算
	if (Azimuth <=180)
	{
		OLED_ShowChar(4,16,'N' );
		Servo_SetAngle(Azimuth);
	}
	else
	{
		OLED_ShowChar(4,16,'S' );
		Servo_SetAngle(Azimuth-180);
	}
	
	OLED_ShowString(1,1,GPS.latitude );
	OLED_ShowString(2,1,GPS.longitude );//显示当前的经纬度
	

}
}

