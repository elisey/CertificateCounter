#include "motor_control.h"
#include "pin_control.h"

pin_t motorEnable;
pin_t motorA;
pin_t motorB;

void MotorControl_Init()
{
	Pin_InitOutput(&motorEnable, GPIOA, GPIO_Pin_5, false);
	Pin_InitOutput(&motorA, GPIOA, GPIO_Pin_6, false);
	Pin_InitOutput(&motorB, GPIOA, GPIO_Pin_7, false);
}

void MotorControl_Start(motorDirection_t direction)
{
	Pin_Write(&motorEnable, false);

	if (direction == motorDirection_forward)	{
		Pin_Write(&motorA, true);
		Pin_Write(&motorB, false);
	}
	else	{
		Pin_Write(&motorA, false);
		Pin_Write(&motorB, true);
	}

	Pin_Write(&motorEnable, true);
}

void MotorControl_Stop()
{
	Pin_Write(&motorEnable, false);
}

void MotorControl_ForceStop()
{
	Pin_Write(&motorA, true);
	Pin_Write(&motorB, true);
}
