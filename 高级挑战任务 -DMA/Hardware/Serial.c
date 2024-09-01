#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ATGM336H.h"
#include "string.h"

uint8_t Serial1_RxFlag1;
uint8_t Serial1_RxFlag2;

float RLat;
float RLon;




void Serial1_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial1_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial1_SendByte(Array[i]);
	}
}//发送数组

void Serial1_SendString(char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Serial1_SendByte(String[i]);
	}
}//发送字符串


uint32_t Serial_Pow(uint32_t X,uint32_t Y)
{
	uint32_t Result=1;
	while(Y--)
	{
		Result*=X;
	}
	return Result;
}//取平方的函数
void Serial1_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial1_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}//发送字符形式的数字


// uint8_t PlaceNorth;//标志北纬还是南纬
// uint8_t PlaceEast;//标志东经还是西经
//void USART1_IRQHandler  (void)
//{
//	static uint8_t RxState1 = 0;//静态变量，只能本函数使用
//	static uint8_t pRxPacket1 = 0;//接收到哪一个有效数据了
//	
//	static uint8_t RxState2 = 0;//静态变量，只能本函数使用
//	static uint8_t pRxPacket2 = 0;//接收到哪一个有效数据了
//	if(USART_GetITStatus (USART1,USART_IT_RXNE )==SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);
//		if(RxState1==0&&RxState2==0)
//		{
//			if(RxData== 'N')
//			{
//				Serial1_RxFlag1= 0;
//				memset(Serial1_RxPacket1, 0,pRxPacket1);
//				RxState1= 1;
//				pRxPacket1= 0;
//				PlaceNorth = 1;
//			}
//			else if(RxData== 'S')
//			{
//				Serial1_RxFlag1= 0;
//				memset(Serial1_RxPacket1, 0, pRxPacket1);
//				RxState1= 1;
//				pRxPacket1= 0;
//				PlaceNorth = 0;
//			}
//			else if(RxData== 'E')
//			{
//				Serial1_RxFlag2= 0;
//				memset(Serial1_RxPacket2, 0, pRxPacket2);
//				RxState2= 1;
//				pRxPacket2= 0;
//				PlaceEast = 1;
//			}
//			else if(RxData== 'W')
//			{
//				Serial1_RxFlag2= 0;
//				memset(Serial1_RxPacket2, 0, pRxPacket2);
//				RxState2= 1;
//				pRxPacket2= 0;
//				PlaceEast = 0;
//			}
//		}
//		else if(RxState1==1)
//		{
//			if(RxData =='S'||RxData == 'N')
//			{
//				RxState1=0;
//				Serial1_RxPacket1[pRxPacket1] = '\0';
//			    Serial1_RxFlag1= 1;
//			}
//			else 
//			{
//			Serial1_RxPacket1[pRxPacket1] = RxData;
//			pRxPacket1++;
//			}
//			
//			
//		}
//		else if(RxState2==1)
//		{
//			if(RxData =='W'||RxData =='E')
//			{
//				RxState2=0;
//				Serial1_RxPacket2[pRxPacket2] = '\0';
//				Serial1_RxFlag2= 1;
//				
//			}
//			else 
//			{
//			Serial1_RxPacket2[pRxPacket2] = RxData;
//			pRxPacket2++;
//			}
//		}


//		USART_ClearITPendingBit (USART1,USART_IT_RXNE );
//	}
//}




//ATGM336H的串口的初始化



char Serial2_RxPacket[150];
uint8_t Serial2_RxFlag;
void Serial2_Init(void)
{
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_USART2 ,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode  = USART_Mode_Tx| USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits  = USART_StopBits_1;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	
	USART_Init (USART2,&USART_InitStructure);
	
	USART_ITConfig (USART2,USART_IT_RXNE ,ENABLE );//开启RXNE标志位到NVIC
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );//分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 1;
	NVIC_Init (&NVIC_InitStructure);
	
	
	
	USART_Cmd(USART2,ENABLE);
}

void Serial2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial2_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial2_SendByte(Array[i]);
	}
}//发送数组

void Serial_SendString(char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Serial2_SendByte(String[i]);
	}
}//发送字符串


void Serial2_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial2_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}//发送字符形式的数字

 char Receive;
void USART2_IRQHandler  (void)
{
	 static uint8_t DataIndex = 0;
	 char Receive;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == 1) 
	{
		Receive = USART_ReceiveData(USART2);		//读取接收到的数据
	
		if(Receive == '$')DataIndex = 0;	
			
		USART_RX_BUF[DataIndex] = Receive;
		DataIndex++;
		//GNRMC\GPRMC
		if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')	
		{
			if(Receive == '\n')									   
			{
				memset(GPS.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
				memcpy(GPS.GPS_Buffer, USART_RX_BUF, DataIndex); 	//保存数据
				GPS.isGetData = 1;
				DataIndex = 0;
				memset(USART_RX_BUF, 0, 150);      //清空				
			}					
		}	 
		USART_ClearITPendingBit (USART2,USART_IT_RXNE );
   } 

}






