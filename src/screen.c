#include "screen.h"

RetSt DibuixaLiniaHoritzontal(uint16_t col_inici, uint16_t col_fi, uint16_t fila,uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval){
    for(int i = 0; i < col_fi - col_inici; i++){
    	SetPixel(col_inici + i, fila, alfa, Rval, Gval, Bval);
    }
    return OK;
}

RetSt DibuixaLiniaVertical(uint16_t col, uint16_t fila_inici, uint16_t fila_fi,uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval){
    while(fila_inici <= fila_fi){
        SetPixel(col, fila_inici, alfa, Rval, Gval, Bval);
        fila_inici++;
    }
    return OK;
}

uint32_t GetPixel (uint16_t col, uint16_t fila){
	uint32_t toReturn = 0;
	if(col > LCD_PIXEL_HEIGHT || fila > LCD_PIXEL_WIDTH || col < 0 || fila < 0){
		return toReturn;
	}
    uint16_t color = (SDRAM_BANK_ADDR + 0x50000 + (2*(LCD_PIXEL_WIDTH * col + fila)));
    toReturn = 0xFFFF << 16 | color;
    return toReturn;
}

RetSt DibuixaCircumferencia(uint16_t ccol, uint16_t cfila, uint16_t radi, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval){
    int x;
    int y;
    int d;
    x = 0;
    y = radi;
    d = 3 - (radi << 1);
    while (x <= y) {
        SetPixel( x + ccol, y + cfila, alfa, Rval, Gval, Bval);
        SetPixel(-x + ccol, y + cfila, alfa, Rval, Gval, Bval);
        SetPixel( x + ccol, -y + cfila, alfa, Rval, Gval, Bval);
        SetPixel(-x + ccol, -y + cfila, alfa, Rval, Gval, Bval);
        SetPixel( y + ccol, x + cfila, alfa, Rval, Gval, Bval);
        SetPixel(-y + ccol, x + cfila, alfa, Rval, Gval, Bval);
        SetPixel( y + ccol, -x + cfila, alfa, Rval, Gval, Bval);
        SetPixel(-y + ccol, -x + cfila, alfa, Rval, Gval, Bval);
        if(d < 0) {                         /* Selecciona el punt A */
            d += (x << 2) + 6;                  /* 4*x + 6              */
        }else{                               /* Selecciona el punt B */
            d += ((x - y) << 2) + 10;             /* 4*(x-y) + 10         */
            y--;                              /* y = y-1              */
        }
        x++;
    }
    return OK;
}

RetSt SetPixelWithColor(uint16_t col, uint16_t fila, uint16_t color){
	if(col > LCD_PIXEL_HEIGHT || fila > LCD_PIXEL_WIDTH || col < 0 || fila < 0){
		return NO_OK;
	}
    *(uint16_t *) (SDRAM_BANK_ADDR + 0x50000 + (2 * (LCD_PIXEL_WIDTH * col + fila))) = color;
    return OK;
}

RetSt EsborraPantalla(uint8_t Rval, uint8_t Gval, uint8_t Bval){
    uint16_t col = 0;
    uint16_t fila = 0;

    uint16_t color;
    color = (255) << 15 | (255) << 10 | (255) << 5 | (255);

    while(col < 320){
    	fila = 0;
        while(fila < 240){
            SetPixelWithColor(col, fila, color);
            fila++;
        }
        col++;
    }
    return OK;
}

RetSt SetPixel(uint16_t col, uint16_t fila, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval){
	if(col > LCD_PIXEL_HEIGHT || fila > LCD_PIXEL_WIDTH || col < 0 || fila < 0){
		return NO_OK;
	}
    uint16_t color;
    color = (alfa) << 15 | (Rval) << 10 | (Gval) << 5 | (Bval);
    *(uint16_t *) (SDRAM_BANK_ADDR + 0x50000 + (2 * (LCD_PIXEL_WIDTH * col + fila))) = color;
    return OK;
}

void DibuixaLinea(int x1, int x2, int y1, int y2, uint8_t alfa, uint8_t Rval, uint8_t Gval, uint8_t Bval){
    float m, c;
    int x, y;

    if ((x2-x1) != 0) {        //No considera el cas d'un segment vertical
        m = (float) (y2 - y1) / (float) (x2 - x1);
        c = y1-(m * x1);
        for (x = x1; x <= x2; x++){
            y = m * x + c;
            SetPixel(x, y, alfa, Rval,Gval,Bval);
        }
    }
}

void printAxis(){
	DibuixaLiniaHoritzontal(0,240,120,1,0,0,0);

	//Square horitzontal: 250-310
	//		 vertical:
	DibuixaLiniaHoritzontal(250,310,220,1,0,0,0);
	DibuixaLiniaHoritzontal(250,310,180,1,0,0,0);
	DibuixaLiniaVertical(250,180,220,1,0,0,0);
	DibuixaLiniaVertical(310,180,220,1,0,0,0);


	DibuixaLiniaVertical(120,0,239,1,0,0,0);
	DibuixaLiniaVertical(240,0,239,1,0,0,0);

	DibuixaCircumferencia(120, 120, 24, 1, 0, 0,255);
	DibuixaCircumferencia(120, 120, 48, 1, 0, 0,255);
	DibuixaCircumferencia(120, 120, 72, 1, 0, 0,255);
	DibuixaCircumferencia(120, 120, 96, 1, 0, 0,255);

	obstacle(0);

}

void obstacle(int value){
	uint16_t color;
	if(value) color = (1) << 15 | (255) << 10 | (0) << 5 | (0);
	else color = (1) << 15 | (0) << 10 | (255) << 5 | (0);

	for(int row = 219; row > 180; row--){
		for(int col = 251; col < 310;col++){
			SetPixelWithColor(col,row,color);
		}
	}

}

void printMeasure(uint16_t col, uint16_t fila){
	SetPixel(col, fila, 100, 255, 0, 0);
}
