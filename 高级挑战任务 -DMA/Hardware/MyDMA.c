#include "stm32f10x.h"                  // Device header


uint8_t PlaceNorth;//标志北纬还是南纬
 uint8_t PlaceEast;//标志东经还是西经
 char Serial1_BUFF[22];
 char Serial1_RxPacket1[100];
char Serial1_RxPacket2[100];
void Serial1_AD_Init(void)
{
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 ,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_AHBPeriphClockCmd (RCC_AHBPeriph_DMA1 ,ENABLE );
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
	
//	USART_ITConfig (USART1,USART_IT_RXNE ,ENABLE );//开启RXNE标志位到NVIC
//	NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );//分组
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority  = 2;
//	NVIC_Init (&NVIC_InitStructure);
	
	
	
	
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t )&USART1->DR ;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;//不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t )Serial1_BUFF;
	DMA_InitStructure.DMA_MemoryDataSize  = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc  = DMA_MemoryInc_Enable ;
	DMA_InitStructure.DMA_DIR  = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 22;
	DMA_InitStructure.DMA_Mode  = DMA_Mode_Circular;
	DMA_InitStructure.DMA_M2M  = DMA_M2M_Disable ;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	
	DMA_Init (DMA1_Channel5,&DMA_InitStructure);
	
	// 启用DMA1_Channel5的传输完成中断
DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
NVIC_PriorityGroupConfig (NVIC_PriorityGroup_2 );

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

//几个部分的使能
	DMA_Cmd(DMA1_Channel5 ,ENABLE);
	USART_DMACmd (USART1,USART_DMAReq_Rx,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	
}
//蓝牙模块的串口和DMA的初始化

void DMA1_Channel5_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC5)==1)
    { 
		
		
		// 处理接收到的数据
		uint8_t i;
		for(i=1;Serial1_BUFF[i]!='N'||Serial1_BUFF[i]!='S';i++)
		{
			Serial1_RxPacket1 [i-1] = Serial1_BUFF[i];
		}
		if(Serial1_BUFF[0]=='N')
		{
			PlaceNorth = 1;
		}
		else if(Serial1_BUFF[0]=='S')
		{
			PlaceNorth = 0;
		}
		i =i+3;
		
		if(Serial1_BUFF[i-1]=='E')
		{
				PlaceEast = 1;
		}
		else if(Serial1_BUFF[i-1]=='S')
		{
			PlaceEast = 0;
		}
		
		
	for(uint8_t j=0;Serial1_BUFF[i]!='E'||Serial1_BUFF[i]!='W';j++)
		{
			Serial1_RxPacket2 [j] = Serial1_BUFF[i];
			i++;
		}
		
        // 清除中断标志
        DMA_ClearITPendingBit(DMA1_IT_TC5);
        
       
       
    }
}





