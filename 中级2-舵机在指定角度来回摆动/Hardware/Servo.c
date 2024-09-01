#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "PWM.h"
#include "OLED.h"
void Servo_Init(void)
{
	PWM_Init();
	
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle/180*2000+500);
}

void Servo_Swing(float S1,float Start,float End)
{
	 S1 = Start;
	
	if(Start<=End)
	{
		while(S1<=End)
				{
				
				   PWM_SetCompare2(S1/180*2000+500);
					Delay_ms (5);
				   S1+=0.2;
				}
	
		if(S1>=End)
		{
				while(S1>=Start)
				{
					PWM_SetCompare2(S1/180*2000+500);
					Delay_ms (5);
				   S1-=0.2;
				}
		}
	}
	else
	{
				while(S1>=End)
				{
				
				   PWM_SetCompare2(S1/180*2000+500);
					Delay_ms (5);
				   S1-=0.2;
				}
	
		if(S1<=End)
		{
				while(S1<=Start)
				{
					PWM_SetCompare2(S1/180*2000+500);
					Delay_ms (5);
				   S1+=0.2;
				}
		}
		}
}


