#ifndef _FASE3_H_
#define _FASE3_H_

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "math.h"
#include "screen.h"
#include <string.h>
//#include "fase2.h"

#define BUFFERSIZE 520

#define MAX_TRAMA 520 //10 header bytes and 255*2 Info bytes

uint16_t bufferData1[BUFFERSIZE];
uint16_t bufferData2[BUFFERSIZE];
int currentBuffer;

#define PH 0
#define CT 2
#define LSN 3
#define FSA 4
#define LSA 6
#define CS 8

int InitializeLCD(void);
void startPrinting(int curBuf);
#endif
