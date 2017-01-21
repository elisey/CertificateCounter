#pragma once

#include "stm32f10x.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum	{
	pinDirection_Input,
	pinDirection_Output
} pinDirection_t;

typedef enum	{
	pinPull_Floating,
	pinPull_PullUp,
	pinPull_PullDown
} pinPull_t;

typedef struct	{
	GPIO_TypeDef* gpio;
	uint16_t pin;
	pinDirection_t pinDirection;
	bool state;
} pin_t;

void Pin_InitInput(pin_t *this, GPIO_TypeDef* gpio, uint16_t pin, pinPull_t pull);
void Pin_InitOutput(pin_t *this, GPIO_TypeDef* gpio, uint16_t pin, bool initState);

bool Pin_Read(pin_t *this);
void Pin_Write(pin_t *this, bool newState);
void Pin_Tougle(pin_t *this);
