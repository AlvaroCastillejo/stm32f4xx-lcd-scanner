#ifndef _FASE2_H_
#define _FASE2_H_

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f429i_discovery.h"
#include <stdio.h>
#include <strings.h>
#include "fase3.h"

int InitializeLCD(void);

void InitializeDMA(uint16_t localx[BUFFERSIZE],uint16_t bufferDatax[BUFFERSIZE]);

void InitializeDMA_MemToMem(void);

void DMA2_Stream1_IRQHandler(void);

void DMA2_Stream0_IRQHandler(void);

void InitializeUSART1(void);

void USART1_IRQHandler(void);

void InitializeDMA1(void);

void InitializeUSART2(void);

void fnc_sendUSART(USART_TypeDef *USARTx, uint8_t poligon1 [MAX_TRAMA]);


#endif
