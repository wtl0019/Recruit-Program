#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "ATGM336H.h"
#include "string.h"
char Serial_RxPacket[150];
uint8_t Serial_RxFlag;
void Serial_Init(void)
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

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
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

