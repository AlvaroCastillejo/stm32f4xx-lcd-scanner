#include "fase3.h"

currentBuffer = 2;

int InitializeLCD(void){
    LCD_Init();

    LTDC_Cmd(ENABLE);

    LCD_DisplayOn();
    LCD_LayerInit();

    LTDC_LayerPixelFormat(LTDC_Layer1, LTDC_Pixelformat_ARGB1555);
    LTDC_ReloadConfig(LTDC_VBReload);

    LCD_SetLayer(LCD_BACKGROUND_LAYER);
    LCD_Clear(LCD_COLOR_WHITE);

    return 0;
}

void inline __attribute__((always_inline)) delay(uint32_t delay)
{
   while(delay--) __asm("");
}

void startPrinting(int curBuf){
	uint8_t poligon1s[1][BUFFERSIZE];
	int seg = 0;
	// Código duplicado en función de qué buffer contenga los nuevos datos. Más óptimo que hacer una función copiando los datos a un array común
	if(curBuf == 1){
		uint8_t lsn = bufferData2[LSN];
		uint16_t lsa = (bufferData2[LSA+1] << 8) | (bufferData2[LSA]);
		uint16_t fsa = (bufferData2[FSA+1] << 8) | (bufferData2[FSA]);

		float angle_inicial = (float) (fsa >> 1) / 64; // Float division
		float angle_final = (float) (lsa >> 1) / 64; // Float division
		float angle_dif = angle_final - angle_inicial;
		if (angle_dif < 0) {
			angle_dif += 360;
		}
		// Incremento del ángulo por cada muestra
		float angle_increment = angle_dif / (lsn - 1);

		// Arrays que contienen los datos de distancia y angulos
		float dsample[bufferData2[LSN]];
		float asample[bufferData2[LSN]];
		int j = 0;
		for(int i = 10; i < 2*bufferData2[LSN]; i=i+2){
			float mostra = (bufferData2[i+1] << 8) | (bufferData2[i]);
			dsample[j] = (float) (mostra) / 4;
			asample[j] = angle_increment * j + angle_inicial;
			j++;
		}
		float x[bufferData2[LSN]];
		float y[bufferData2[LSN]];
		// Cálculo y printado de las muestras
		for(int i = 0; i < bufferData2[LSN]; i++){
			if((asample[i] < 270+30 && asample[i] > 270-30) && dsample[i] < 1000) obstacle(1);
			//else obstacle(0); Mantiene el estatus para toda la muestra. Si se descomenta la línea el estatus cambiará para cada muestra.
			x[i] = -((((dsample[i]) * cos(asample[i]*M_PI/180))/1000)*24)/2;
			y[i] = -((((dsample[i]) * sin(asample[i]*M_PI/180))/1000)*24)/2;
			SetPixel(120-x[i], 120+y[i], 100, 255, 0, 0);
		}
	} else {
		//memcpy(&poligon1s[seg],&bufferData1,sizeof(poligon1s[seg]));
		uint8_t lsn = bufferData1[LSN];
		uint16_t lsa = (bufferData1[LSA+1] << 8) | (bufferData1[LSA]);
		uint16_t fsa = (bufferData1[FSA+1] << 8) | (bufferData1[FSA]);
		//uint8_t ct = poligon1s[seg][CT];

		//float sample_frequency = (float) (ct >> 1) / 10;
		float angle_inicial = (float) (fsa >> 1) / 64; // Float division
		float angle_final = (float) (lsa >> 1) / 64; // Float division
		float angle_dif = angle_final - angle_inicial;
		if (angle_dif < 0) {
			angle_dif += 360;
		}
		float angle_increment = angle_dif / (lsn - 1);



		float dsample[bufferData1[LSN]];
		float asample[bufferData1[LSN]];
		int j = 0;
		for(int i = 10; i < 2*bufferData1[LSN]; i=i+2){
			float mostra = (bufferData1[i+1] << 8) | (bufferData1[i]);
			dsample[j] = (float) (mostra) / 4;
			asample[j] = angle_increment * j + angle_inicial;
			j++;
		}
		float x[bufferData1[LSN]];
		float y[bufferData1[LSN]];
		for(int i = 0; i < bufferData1[LSN]; i++){
			if((asample[i] < 270+30 && asample[i] > 270-30) && dsample[i] < 1000) obstacle(1);
			//else obstacle(0);
			x[i] = -((((dsample[i]) * cos(asample[i]*M_PI/180))/1000)*24)/2;
			y[i] = -((((dsample[i]) * sin(asample[i]*M_PI/180))/1000)*24)/2;
			SetPixel(120-x[i], 120+y[i], 100, 255, 0, 0);
		}
	}




}
