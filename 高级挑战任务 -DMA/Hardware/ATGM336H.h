#ifndef __ATGM336H_H
#define __ATGM336H_H

extern char USART_RX_BUF[100];     //接收缓冲
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

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

extern GNRMC GPS;
extern double Lat;
extern double Lon;

void ATGM_StructInit(void);
void DataPrase(void);
void DataConvet(void);
#endif
