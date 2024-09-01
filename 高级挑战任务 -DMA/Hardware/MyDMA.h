#ifndef __MYDMA_H
#define __MYDMA_H


extern uint8_t PlaceNorth;//标志北纬还是南纬
extern uint8_t PlaceEast;//标志东经还是西经
extern char Serial1_BUFF[100];
extern char Serial1_RxPacket1[100];
extern char Serial1_RxPacket2[100];


void Serial1_AD_Init(void);

#endif

