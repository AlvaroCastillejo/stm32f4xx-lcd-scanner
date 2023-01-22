#ifndef _FASE1_H_
#define _FASE1_H_

#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "fase2.h"

void InitializeTMR2(void);

void InitializeTMR5(void);

void InitializeTMR3(void);

void InitializeTMR10(void);

void InitializeTMR11(void);

void InitializePWM3(void);

void InitializePWM10(void);

void InitializePWM11(void);

void InitializeLED(void);

void InitializeUserButton(void);

void InitializeTestPorts(void);

void EnableInterrupts(void);

void EnableTimerInterrupt();

void EXTI0_IRQHandler(void);

void EXTI2_IRQHandler(void);

void TIM2_IRQHandler(void);

void TIM5_IRQHandler(void);

void calculateSpeedLeft(void);

void calculateSpeedRight(void);

void setNInt(int val);

void setTRight(int val);

void setTLeft(int val);

void setMode(int val);

void setTOn(int val);

void setDutyCycle(int val);

int getMode(void);

#endif
