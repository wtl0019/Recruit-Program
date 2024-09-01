#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

char Serial_RxPacket1[100];
char Serial_RxPacket2[100];
uint8_t Serial_RxFlag1;
uint8_t Serial_RxFlag2;

float RLat;
float RLon;
void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 ,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode  = USART_Mode_Tx| USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits  = USART_StopBits_1;
	USART_InitStructure.USART_WordLength  = USART_WordLength_8b;
	
	USART_Init (USART1,&USART_InitStructure);
	
	USART_ITConfig (USART1,USART_IT_RXNE ,ENABLE );//开启RXNE标志位到NVIC
	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );//分组
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 1;
	NVIC_Init (&NVIC_InitStructure);
	
	
	
	USART_Cmd(USART1,ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}

void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}//发送数组

void Serial_SendString(char *String)
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
		Serial_SendByte(String[i]);
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
void Serial_SendNumber(uint32_t Number,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
	}
}//发送字符形式的数字

int fputc(int ch,FILE*f)
{
	Serial_SendByte(ch);
	return ch;
}//将printf打印函数重定向到串口

void Serial_Printf(char *format,...)
{
	char String[100];
	va_list arg;//参数列表变量
	va_start(arg,format);//从format位置开始接收参数表，放在arg里面
	vsprintf(String,format,arg);//打印的位置是在 String,格式化字符串是format,参数表是arg
	va_end(arg);//释放参数表
	Serial_SendString(String);
}


//void Serial_SendPacket(void)
//{
//	Serial_SendByte (0xFF);
//	Serial_SendArray (Serial_TxPacket,4);
//	Serial_SendByte (0xFE);
//}

//uint8_t Serial_GetRxFlag(void)
//{
//	if(Serial_RxFlag ==1)
//	{
//		Serial_RxFlag= 0;
//		return 1;
//	}
//	return 0;
//}//得到标志位的值（自己设定的），是否已经触发了一次中断，判断是否接收到了数据包

uint8_t PlaceNorth;//标志北纬还是南纬
uint8_t PlaceEast;//标志东经还是西经
void USART1_IRQHandler  (void)
{
	static uint8_t RxState1 = 0;//静态变量，只能本函数使用
	static uint8_t pRxPacket1 = 0;//接收到哪一个有效数据了
	
	static uint8_t RxState2 = 0;//静态变量，只能本函数使用
	static uint8_t pRxPacket2 = 0;//接收到哪一个有效数据了
	if(USART_GetITStatus (USART1,USART_IT_RXNE )==SET)
	{
		uint8_t RxData = USART_ReceiveData(USART1);
		if(RxState1==0&&RxState2==0)
		{
			if(RxData== 'N')
			{
				Serial_RxFlag1= 0;
				memset(Serial_RxPacket1, 0,pRxPacket1);
				RxState1= 1;
				pRxPacket1= 0;
				PlaceNorth = 1;
			}
			else if(RxData== 'S')
			{
				Serial_RxFlag1= 0;
				memset(Serial_RxPacket1, 0, pRxPacket1);
				RxState1= 1;
				pRxPacket1= 0;
				PlaceNorth = 0;
			}
			else if(RxData== 'E')
			{
				Serial_RxFlag2= 0;
				memset(Serial_RxPacket2, 0, pRxPacket2);
				RxState2= 1;
				pRxPacket2= 0;
				PlaceEast = 1;
			}
			else if(RxData== 'W')
			{
				Serial_RxFlag2= 0;
				memset(Serial_RxPacket2, 0, pRxPacket2);
				RxState2= 1;
				pRxPacket2= 0;
				PlaceEast = 0;
			}
		}
		else if(RxState1==1)
		{
			if(RxData =='S'||RxData == 'N')
			{
				RxState1=0;
				Serial_RxPacket1[pRxPacket1] = '\0';
			    Serial_RxFlag1= 1;
			}
			else 
			{
			Serial_RxPacket1[pRxPacket1] = RxData;
			pRxPacket1++;
			}
			
			
		}
		else if(RxState2==1)
		{
			if(RxData =='W'||RxData =='E')
			{
				RxState2=0;
				Serial_RxPacket2[pRxPacket2] = '\0';
				Serial_RxFlag2= 1;
				
			}
			else 
			{
			Serial_RxPacket2[pRxPacket2] = RxData;
			pRxPacket2++;
			}
		}

		USART_ClearITPendingBit (USART1,USART_IT_RXNE );
	}
}


void BTDataCon(void)
{
//	RLat = 0;
//	RLon = 0;
	float tmp1 = 0;
	float tmp2 = 0;
	for (uint8_t i = 1; Serial_RxPacket1[i] != '\0'; i++)
			{

				if (Serial_RxPacket1[i] == '.')
				{
					continue;
				}
				 if (i == 1)
				{
					RLat = (Serial_RxPacket1[0] - 48) * 10 + (Serial_RxPacket1[1] - 48);
					//取出个位和十位
				}
				 else
				{
					tmp1 += (Serial_RxPacket1[i] - 48);
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
			for (uint8_t i = 2; Serial_RxPacket2[i] != '\0'; i++)
			{

				if (Serial_RxPacket2[i] == '.')
				{
					continue;
				}
				if (i == 2)
				{
					RLon = (((Serial_RxPacket2[0] - 48) * 10 + (Serial_RxPacket2[1] - 48)) * 10) + (Serial_RxPacket2[2] - 48);
					//取出个位和十位和百位
				}
				else
				{
					tmp2 += (Serial_RxPacket2[i] - 48);
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







//设定了数据包的格式，对接收逻辑进行了编写，并且支持了多次不同的接收(解决了)
//引入了数组清零函数，能够让多次的接收内容能够更新
//定义了接收数据包的标志位函数Serial_RxFlagx，接受前为0，接收到了之后为1，并且可以刷新，方便接收数据的等待过程的编写


//1.分开接收，两种经纬度
//2.多次接收
//3.数据刷新（刷新时，该如何处理；将字符转变为浮点数）