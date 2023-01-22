#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stdio.h"
#include "math.h"

typedef enum {
   NO_OK = 0,
   OK = !NO_OK
}RetSt ;


RetSt SetPixel(uint16_t col, uint16_t fila, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval);

uint32_t GetPixel(uint16_t col, uint16_t fila);

RetSt DibuixaLiniaHoritzontal(uint16_t col_inici, uint16_t col_fi, uint16_t fila, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval);

RetSt DibuixaLiniaVertical(uint16_t col, uint16_t fila_inici, uint16_t fila_fi, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval);

RetSt DibuixaCircumferencia(uint16_t ccol, uint16_t cfila, uint16_t radi, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval);

RetSt EsborraPantalla(uint8_t Rval, uint8_t Gval, uint8_t Bval);

void DibuixaLinea(int x1, int x2, int y1, int y2, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval);

void printAxis();

void obstacle(int value);

#endif
