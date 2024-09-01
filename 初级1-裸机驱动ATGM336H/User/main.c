#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "LED.h"
#include <string.h>
#include "ATGM336H.h"

int main(void){
	OLED_Init();
	LED_Init();
	Serial_Init();
	ATGM_StructInit();
	
	
	OLED_ShowString (1,1,"Latitude:");
	OLED_ShowString (3,1,"Longitude:");
	
	
	while(1){
		//这个程序中还不需要数据的转换	
//		DataPrase();
//		DataConvet();
		OLED_ShowString(2,1, GPS.latitude );
		OLED_ShowString(4,1,GPS.longitude  );

	}
	
}
