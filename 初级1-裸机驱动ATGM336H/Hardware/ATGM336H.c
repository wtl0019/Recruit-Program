#include "stm32f10x.h"                  // Device header
#include "string.h"

 char USART_RX_BUF[150];     //接收缓冲
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 
float Lat;
float Lon;
typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	
	uint8_t  isGetData;		//数据获取完成标志位
	uint8_t  isParseData;	//解析完成标志位
	
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];	//经度
	char E_W[E_W_Length];		//E/W
	
	uint8_t  isUsefull;		//信息有效标志位
}GNRMC;

GNRMC GPS;//定义结构体的名称
void ATGM_StructInit(void)
{
	GPS.isGetData = 0;
	GPS.isParseData = 0;
	GPS.isUsefull = 0;
	memset(GPS.GPS_Buffer, 0, GPS_Buffer_Length);    
	memset(GPS.UTCTime, 0, UTCTime_Length);
	memset(GPS.latitude, 0, latitude_Length);
	memset(GPS.N_S, 0, N_S_Length);
	memset(GPS.longitude, 0, longitude_Length);
	memset(GPS.E_W, 0, E_W_Length);
}


//进行数据的切割

void DataPrase(void)
{
	char *Comma;
	char *CommaNext;
	if(GPS.isGetData ==1)
	{
		Comma = strstr(GPS.GPS_Buffer, ",");
	for(uint8_t i=1;i<=6;i++){
		Comma++;
		if((CommaNext= strstr(Comma, ",")) != NULL)
		{
			char usefulBuffer[2]; 
			switch(i)
					{
						case 1:memcpy(GPS.UTCTime, Comma,  CommaNext -  Comma);break;	
						case 2:
						{
							memcpy(usefulBuffer,  Comma,  CommaNext -  Comma);//有效标志位
							if(usefulBuffer[0] == 'A')
								GPS.isUsefull = 1;
							else if(usefulBuffer[0] == 'V')
								GPS.isUsefull = 0;	
							break;
						}	
						case 3:memcpy(GPS.latitude,  Comma,  CommaNext -  Comma);break;	
						case 4:memcpy(GPS.N_S,  Comma,  CommaNext -  Comma);break;	
						case 5:memcpy(GPS.longitude,  Comma,  CommaNext -  Comma);break;	
						case 6:memcpy(GPS.E_W,  Comma,  CommaNext -  Comma);break;	
						default:break;
					}
				Comma = CommaNext;	
		}
		
	}
	
	}
	GPS.isParseData = 1;
}



//数据转换
void DataConvet(void)
{
	if(GPS.isParseData ==1)
	{
		float tmp = 0;//暂存后面的位数，便于计算
		for (uint8_t i = 1; GPS.latitude[i] != '\0'; i++)
			{

				if (GPS.latitude[i] == '.')
				{
					continue;
				}
				if (i == 1)
				{
					Lat = (GPS.latitude[0] - 48) * 10 + (GPS.latitude[1] - 48);
					//取出个位和十位
				}
				else
				{
					tmp += (GPS.latitude[i] - 48);
					tmp *= 10;//先将后面的位换算为整型
				}
			}
		tmp = tmp/100000;
		tmp = tmp/60;
        Lat +=tmp;
			
		//转换后的数又转变回字符
        GPS.latitude[2]	 = '.';

		uint8_t Lats;//取出每次取整后的那一位数
		float Latss;//中间量，为了不影响Lat的值
		Lats = (int)Lat;
		Latss  =Lat -Lats;
		for (uint8_t j = 3; j < 10; j++)
			{
				Latss *= 10;
				Lats = (int)Latss;
				GPS.latitude[j] = Lats + '0';
				Latss -= Lats;
			}	
		tmp = 0;
		
		for (uint8_t i = 2; GPS.longitude[i] != '\0'; i++)
			{

				if (GPS.longitude[i] == '.')
				{
					continue;
				}
				if (i == 2)
				{
					Lon = (((GPS.longitude[0] - 48) * 10 + (GPS.longitude[1] - 48)) * 10) + (GPS.longitude[2] - 48);
					//取出个位和十位和百位
				}
				else
				{
					tmp += (GPS.longitude[i] - 48);
					tmp *= 10;
				}
			}
		tmp = tmp/100000;
		tmp = tmp/60;
        Lon +=tmp;
			
		GPS.longitude[3]='.';
			
		uint8_t Lons;//取出每次取整后的那一位数
		float Lonss;//中间量，为了不影响Lat的值
		Lons = (int)Lon;
		Lonss  =Lon -Lons;
		for (uint8_t j = 4; j < 11; j++)
			{
				Lonss *= 10;
				Lons = (int)Lonss;
				GPS.latitude[j] = Lats + '0';
				Latss -= Lats;
			}	
		tmp = 0;
		GPS.isUsefull = 1;
	}
}











