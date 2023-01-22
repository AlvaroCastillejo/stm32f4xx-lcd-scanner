#include "fase1.h"

int nInt;
float tOn;
float dutyCycle = 50;
float pulse_length;
int mode;

float speedLeft = 6000;
float speedRight = 7812;

//Debugging
float arr[100];
int i = 0;
float speed[100];
int j = 0;

float speedInWLeft;
float speedInWRight;

float tRight = 0;
float tLeft = 0;

float periodeLeft;
float periodeRight;

int startScan = 0;

void InitializeTMR2(void){	// Configuracio de Timer de 1ms
	TIM_TimeBaseInitTypeDef timerInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    timerInitStructure.TIM_Prescaler = 4;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 17999;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;

    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void InitializeTMR5(void){	// Configuracio de Timer de 10us
	TIM_TimeBaseInitTypeDef timerInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	timerInitStructure.TIM_Prescaler = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 899;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &timerInitStructure);
	TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

void InitializeTMR3(void){	// Configuracio de Timer per PWM de 50us
	TIM_TimeBaseInitTypeDef timerInitStructure;
	TIM_OCInitTypeDef TIM_OCStruct;

	pulse_length = (((4499 + 1) * dutyCycle) / 100) - 1;

	timerInitStructure.TIM_Prescaler = 0;
	timerInitStructure.TIM_Period = 4499;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &timerInitStructure);
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	//Channel 1 (PA6)
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_Pulse = pulse_length;

    TIM_OC1Init(TIM3, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

    //Channel 2 (PA7)
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_Pulse = pulse_length;

	TIM_OC2Init(TIM3, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_Cmd(TIM3, ENABLE);
}

void InitializeTMR10(void){	// Configuracio de Timer per PWM de 50us
	TIM_TimeBaseInitTypeDef timerInitStructure;

	float auxLeft = (16 * (speedLeft / 1000));
    periodeLeft = (1000000 / auxLeft);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);

	timerInitStructure.TIM_Prescaler = 179;
	timerInitStructure.TIM_Period = periodeLeft;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM10, &timerInitStructure);
	TIM_ARRPreloadConfig(TIM10, ENABLE);

	TIM_OCInitTypeDef TIM_OCStruct;

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_Pulse = periodeLeft / 2;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OC1Init(TIM10, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);

	TIM_Cmd(TIM10, ENABLE);
}

void InitializeTMR11(void){	// Configuracio de Timer per PWM de 50us
	TIM_TimeBaseInitTypeDef timerInitStructure;

	float auxRight = (16 * (speedRight / 1000));
	periodeRight = (1000000 / auxRight);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);

	timerInitStructure.TIM_Prescaler = 179;
	timerInitStructure.TIM_Period = periodeRight;
	timerInitStructure.TIM_ClockDivision = 0;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM11, &timerInitStructure);
	TIM_ARRPreloadConfig(TIM11, ENABLE);

	TIM_OCInitTypeDef TIM_OCStruct;

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCStruct.TIM_Pulse = periodeRight / 2;

    TIM_OC1Init(TIM11, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);

	TIM_Cmd(TIM11, ENABLE);
}

void InitializePWM3(void){
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
}

void InitializePWM10(void){ //PWM esquerra
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);
}

void InitializePWM11(void){ //PWM dret
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM11);
}

void InitializeLED(void){
	GPIO_InitTypeDef gpioStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);

    gpioStructure.GPIO_Pin = GPIO_Pin_13;
    gpioStructure.GPIO_Mode = GPIO_Mode_OUT;
    gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &gpioStructure);
    GPIO_WriteBit(GPIOG, GPIO_Pin_13, Bit_RESET);
}

void InitializeUserButton(void) {
	GPIO_InitTypeDef GPIO_InitDef;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitDef);
}

void InitializeTestPorts(void){
    GPIO_InitTypeDef gpioStructureD;
    GPIO_InitTypeDef gpioStructureE;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    gpioStructureD.GPIO_Pin = GPIO_Pin_12;
    gpioStructureD.GPIO_Mode = GPIO_Mode_OUT;
    gpioStructureD.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpioStructureD);

	gpioStructureE.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	gpioStructureE.GPIO_Mode = GPIO_Mode_OUT;
	gpioStructureE.GPIO_OType = GPIO_OType_PP;
	gpioStructureE.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioStructureE.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &gpioStructureE);

	GPIO_WriteBit(GPIOE, GPIO_Pin_10, Bit_RESET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_11, Bit_RESET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_12, Bit_RESET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_13, Bit_RESET);
    GPIO_WriteBit(GPIOE, GPIO_Pin_14, Bit_RESET);
}

void EnableInterrupts(void) {
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_SensorE;
	EXTI_InitTypeDef EXTI_SensorD;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //Enable clock for GPIOD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //Enable clock for SYSCFG

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; //Declara com a input
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Tell system that you will use PD0 for EXTI_Line0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);

	// PD0 is connected to EXTI_Line0
	EXTI_SensorE.EXTI_Line = EXTI_Line0;
	EXTI_SensorE.EXTI_LineCmd = ENABLE; //Enable interrupt
	EXTI_SensorE.EXTI_Mode = EXTI_Mode_Interrupt; //Interrupt mode
	EXTI_SensorE.EXTI_Trigger = EXTI_Trigger_Rising; //Triggers on rising and falling edge
	EXTI_Init(&EXTI_SensorE); //Add to EXTI

	// Add IRQ vector to NVIC
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; 	//Set priority
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00; //Set sub priority
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //Enable interrupt
	NVIC_Init(&NVIC_InitStruct); //Add to NVIC

	// PD1 is connected to EXTI_Line2
	EXTI_SensorD.EXTI_Line = EXTI_Line2;
	EXTI_SensorD.EXTI_LineCmd = ENABLE; //Enable interrupt
	EXTI_SensorD.EXTI_Mode = EXTI_Mode_Interrupt; //Interrupt mode
	EXTI_SensorD.EXTI_Trigger = EXTI_Trigger_Rising; //Triggers on rising and falling edge
	EXTI_Init(&EXTI_SensorD); //Add to EXTI

	// Add IRQ vector to NVIC
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; //Set priority
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01; //Set sub priority
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //Enable interrupt
	NVIC_Init(&NVIC_InitStruct); //Add to NVIC
}

void EnableTimerInterrupt(){
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

    NVIC_InitTypeDef nvicStructure5;
	nvicStructure5.NVIC_IRQChannel = TIM5_IRQn;
	nvicStructure5.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure5.NVIC_IRQChannelSubPriority = 1;
	nvicStructure5.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure5);
}

void EXTI0_IRQHandler(void){   //Rutina de interrupcio de la Linia0 (Sensor esquerra)
	//Mesura de temps d'execucio
	//GPIO_ToggleBits(GPIOE, GPIO_Pin_12);

	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		calculateSpeedLeft();
		tLeft = 0;
		EXTI_ClearITPendingBit(EXTI_Line0);	//Netejem flag d'interrupcio
    }
    //GPIO_ToggleBits(GPIOE, GPIO_Pin_12);
}


void EXTI2_IRQHandler(void){   //Rutina de interrupcio de la Linia2 (Sensor dret)
	if (EXTI_GetITStatus(EXTI_Line2) != RESET){
		calculateSpeedRight();
		tRight = 0;
		EXTI_ClearITPendingBit(EXTI_Line2);	//Netejem flag d'interrupcio
	}
}

void TIM2_IRQHandler(void){     //1ms Timer interrupt
	//Mesura de temps d'execucio (1.2us aprox.)
	//GPIO_ToggleBits(GPIOE, GPIO_Pin_10);

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
  		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    	nInt++;
    	if((nInt % 2) == 0 && mode == 0){
    		//Multipliquem per 100 per poder fer un valor de 0-100, tal com demana la formula
    		dutyCycle = (tOn / 20000) * 100;
    		InitializeTMR3();
    	}
    	if (nInt == 200) {
    		GPIO_ToggleBits(GPIOG, GPIO_Pin_13);
        	nInt = 0;
        }
		//Controlar el bit start LIDAR
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)){
			startScan = 1; //Inici LIDAR
		}
    }
 	//GPIO_ToggleBits(GPIOE, GPIO_Pin_10);
}

void TIM5_IRQHandler(void){
	//Mesura de temps d'execucio (1us aprox.)
	GPIO_ToggleBits(GPIOE, GPIO_Pin_11);

	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
		tRight++;
		tLeft++;
		tOn++;
		if(tOn == 19998){
			tOn = 0;
		}
    }
    //GPIO_ToggleBits(GPIOE, GPIO_Pin_10);
}

void calculateSpeedLeft(void){
	//Multipliquem per 10, ja que cada tick correspon a 10us
	speedInWLeft = (1 / (16 * ((10 * tLeft) / 1000000))) * 1000;
	/*if(j < 100){
		speed[j] = speedInWLeft;
		j++;
	}else{
		j = 100;
	}*/
}

void calculateSpeedRight(void){
	//Multipliquem per 10, ja que cada tick correspon a 10us
	speedInWRight = (1 / (16 * ((10 * tRight) / 1000000))) * 1000;
}

void setNInt(int val){
	nInt = val;
}

void setTRight(int val){
	tRight = val;
}

void setTLeft(int val){
	tLeft = val;
}
void setMode(int val){
	mode = val;
}
void setTOn(int val){
	tOn = val;
}
void setDutyCycle(int val){
	dutyCycle = val;
}

int getMode(void){
	return mode;
}

