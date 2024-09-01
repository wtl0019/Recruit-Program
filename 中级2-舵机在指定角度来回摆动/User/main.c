#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Servo.h"
#include "Key.h"

float Angle,S1, Start,End;

int main(void){
	OLED_Init ();
	Servo_Init() ;
	while(1){

		Start  = 0;
		End = 90;
		Servo_Swing( S1, Start,End);
	
	}
}
