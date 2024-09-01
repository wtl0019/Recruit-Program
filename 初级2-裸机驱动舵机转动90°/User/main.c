#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"

uint8_t KeyNum;
float Angle;


int main(void){
	OLED_Init();
	Servo_Init() ;
	Key_Init();
	
	OLED_ShowString(1,1,"Angle:");
	
	 
	while(1){

		Servo_SetAngle(90);
		OLED_ShowNum(1,7,90,2);
	}
}
