#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>
//蓝牙模块串口部分
//extern char Serial1_RxPacket1[];
//extern char Serial1_RxPacket2[];
extern uint8_t Serial1_RxFlag1;
extern uint8_t Serial1_RxFlag2;
extern double RLat;
extern double RLon;
//void Serial1_Init(void);
//void Serial1_SendByte(uint8_t Byte);
//void Serial1_SendArray(uint8_t *Array,uint16_t Length);
//void Serial1_SendString(char *String);
//uint32_t Serial_Pow(uint32_t X,uint32_t Y);
//void Serial1_SendNumber(uint32_t Number,uint8_t Length);
//extern uint8_t PlaceNorth;//标志北纬还是南纬
//extern uint8_t PlaceEast;//标志东经还是西经


//ATGM336H串口部分
extern char Serial2_RxPacket[];
extern uint8_t Serial2_RxFlag;

void Serial2_Init(void);
void Serial2_SendByte(uint8_t Byte);
void Serial2_SendArray(uint8_t *Array,uint16_t Length);
void Serial2_SendString(char *String);
void Serial2_SendNumber(uint32_t Number,uint8_t Length);

 extern char Receive;


#endif

