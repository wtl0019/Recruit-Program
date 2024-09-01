#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

extern char Serial_RxPacket1[];
extern char Serial_RxPacket2[];
extern uint8_t Serial_RxFlag1;
extern uint8_t Serial_RxFlag2;
extern float RLat;
extern float RLon;
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array,uint16_t Length);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X,uint32_t Y);
void Serial_SendNumber(uint32_t Number,uint8_t Length);
int fputc(int ch,FILE*f);
void Serial_Printf(char *format,...);
void BTDataCon(void);

extern uint8_t PlaceNorth;//标志北纬还是南纬
extern uint8_t PlaceEast;//标志东经还是西经
//uint8_t Serial_GetRxFlag(void);

#endif
