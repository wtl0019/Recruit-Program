#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <string.h>



uint8_t RxData;
int main(void){
	OLED_Init();
	LED_Init();
	Serial_Init();
	
	

	
	while(1){


OLED_ShowString (1,1,Serial_RxPacket1);
OLED_ShowCN(2,1,(uint8_t)(Serial_RxPacket2[0])-48);
OLED_ShowCN(2,2,(uint8_t)(Serial_RxPacket2[1])-48);
OLED_ShowCN(2,3,(uint8_t)(Serial_RxPacket2[2])-48);
	}
	
}
