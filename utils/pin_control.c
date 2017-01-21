#include "pin_control.h"

static void initRcc(pin_t *this);

void Pin_InitInput(pin_t *this, GPIO_TypeDef* gpio, uint16_t pin, pinPull_t pull)
{
	this->gpio = gpio;
	this->pin = pin;
	this->pinDirection = pinDirection_Input;

	initRcc(this);

	GPIO_InitTypeDef sGpio;

	switch (pull)
	{
	case pinPull_Floating:
		sGpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;
	case pinPull_PullDown:
		sGpio.GPIO_Mode = GPIO_Mode_IPD;
		break;
	case pinPull_PullUp:
		sGpio.GPIO_Mode = GPIO_Mode_IPU;
		break;
	default:

		break;
	}
	sGpio.GPIO_Speed = GPIO_Speed_50MHz;
	sGpio.GPIO_Pin = pin;
	GPIO_Init(gpio, &sGpio);
}

void Pin_InitOutput(pin_t *this, GPIO_TypeDef* gpio, uint16_t pin, bool initState)
{
	this->gpio = gpio;
	this->pin = pin;
	this->pinDirection = pinDirection_Output;
	this->state = initState;

	initRcc(this);

	GPIO_InitTypeDef sGpio;

	sGpio.GPIO_Mode = GPIO_Mode_Out_PP;
	sGpio.GPIO_Speed = GPIO_Speed_50MHz;
	sGpio.GPIO_Pin = pin;
	GPIO_Init(gpio, &sGpio);

	Pin_Write(this, initState);
}

bool Pin_Read(pin_t *this)
{
	if ((this->gpio->IDR & this->pin) != (uint32_t) 0)
	{
		return true;
	}
	else {
		return false;
	}
}

void Pin_Write(pin_t *this, bool newState)
{
	if (newState == true)
	{
		this->gpio->BSRR = this->pin;
	}
	else {
		this->gpio->BRR = this->pin;
	}
	this->state = newState;
}

void Pin_Tougle(pin_t *this)
{
	Pin_Write(this, !(this->state));
}

static void initRcc(pin_t *this)
{
	if (this->gpio == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (this->gpio == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if (this->gpio == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if (this->gpio == GPIOD)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if (this->gpio == GPIOE)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if (this->gpio == GPIOF)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if (this->gpio == GPIOG)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
}
