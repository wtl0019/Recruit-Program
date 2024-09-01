#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "MyDMA.h"
void BTDataCon(void)
{

	double tmp1 = 0;
	 double tmp2 = 0;
	if(Serial1_RxPacket1[2]=='.')
	{
	for (uint8_t i = 1; Serial1_RxPacket1[i] != '\0'; i++)
			{

				if (Serial1_RxPacket1[i] == '.')
				{
					continue;
				}
				 if (i == 1)
				{
					RLat = (Serial1_RxPacket1[0] - 48) * 10 + (Serial1_RxPacket1[1] - 48);
					//取出个位和十位
				}
				 else
				{
					tmp1 += (Serial1_RxPacket1[i] - 48);
					tmp1 *= 10;//先将后面的位换算为整型
				}
			}
			if(PlaceNorth ==0)
			{
				RLat= -(RLat + tmp1/1000000);
					}
	        else {
		 RLat = RLat + tmp1/1000000;
			                        }
								}
	else if(Serial1_RxPacket1[1]=='.')
	{
		for (uint8_t i = 0; Serial1_RxPacket1[i] != '\0'; i++)
			{

				if (Serial1_RxPacket1[i] == '.')
				{
					continue;
				}
				 if (i == 0)
				{
					RLat = (Serial1_RxPacket1[0] - 48);
					//取出个位
				}
				 else
				{
					tmp1 += (Serial1_RxPacket1[i] - 48);
					tmp1 *= 10;//先将后面的位换算为整型
				}
			}
			if(PlaceNorth ==0)
			{
				RLat= -(RLat + tmp1/1000000);
					}
	        else {
		 RLat = RLat + tmp1/1000000;
			                        }
	}
									
		if(Serial1_RxPacket2[3]=='.')
		{
			for (uint8_t i = 2; Serial1_RxPacket2[i] != '\0'; i++)
			{

				if (Serial1_RxPacket2[i] == '.')
				{
					continue;
				}
				if (i == 2)
				{
					RLon = (((Serial1_RxPacket2[0] - 48) * 10 + (Serial1_RxPacket2[1] - 48)) * 10) + (Serial1_RxPacket2[2] - 48);
					//取出个位和十位和百位
				}
				else
				{
					tmp2 += (Serial1_RxPacket2[i] - 48);
					tmp2 *= 10;
				}
			}
				if(PlaceEast ==0)
			{
				RLon= -(RLon + tmp2/1000000);
					}
	        else {
		 RLon = RLon + tmp2/1000000;
			                        }
								}
		else if(Serial1_RxPacket2[2]=='.')
		{
			for (uint8_t i = 1; Serial1_RxPacket2[i] != '\0'; i++)
			{
			if (Serial1_RxPacket2[i] == '.')
				{
					continue;
				}
				 if (i == 1)
				{
					RLon = (Serial1_RxPacket2[0] - 48) * 10 + (Serial1_RxPacket2[1] - 48);
					//取出个位和十位
				}
				 else
				{
					tmp2 += (Serial1_RxPacket2[i] - 48);
					tmp2 *= 10;//先将后面的位换算为整型
				}
			}
			if(PlaceEast ==0)
			{
				RLon= -(RLon + tmp2/1000000);
					}
	        else {
		 RLon = RLon + tmp1/1000000;
			                        }
			
		}
		else if(Serial1_RxPacket2[1]=='.')
		{
			for (uint8_t i = 0; Serial1_RxPacket2[i] != '\0'; i++)
			{

				if (Serial1_RxPacket2[i] == '.')
				{
					continue;
				}
				 if (i == 0)
				{
					RLon = (Serial1_RxPacket2[0] - 48);
					//取出个位
				}
				 else
				{
					tmp2 += (Serial1_RxPacket2[i] - 48);
					tmp2 *= 10;//先将后面的位换算为整型
				}
			}
			if(PlaceEast ==0)
			{
				RLon= -(RLon + tmp2/1000000);
					}
	        else {
		 RLon = RLon + tmp2/1000000;
			                        }
		}
}    

