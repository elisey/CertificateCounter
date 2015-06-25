#pragma once

typedef enum	{
	motorDirection_forward,
	motorDirection_backward
} motorDirection_t;

void MotorControl_Init();
void MotorControl_Start(motorDirection_t direction);
void MotorControl_Stop();
void MotorControl_ForceStop();
