#include "device_state_machine.h"

#include "FreeRTOS.h"
#include "task.h"

#include "usb_interface.h"

#include "inputReader.h"
#include "motor_control.h"

const uint8_t COM_IDLE = 0;
const uint8_t COM_CATCH_AND_ROLL = 1;
const uint8_t COM_ROLL_OUT = 2;

const uint8_t EVENT_CAUGHT_AND_ROLLED = 10;
const uint8_t EVENT_ROLLED_OUT = 11;

const uint8_t FAIL_CATCH = 20;
const uint8_t FAIL_JAM = 21;
const uint8_t FAIL_SWITCH = 22;

const uint8_t PING = 30;
const uint8_t PONG = 31;

typedef enum	{
	state_wait_for_command,
	state_catch_and_roll,
	state_roll_out
} state_t;

static void task();


static void stateWaitForCommand();
static void stateCatchAndRoll();
static void stateRollOut();
static bool waitForCatch(uint32_t timeToWait, bool targetState);

void DeviceStateMachine_Init()
{
	xTaskCreate(
			task,
			"Device state machine task",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY + 1,
			NULL);
}

static state_t state = state_wait_for_command;

static void task()
{


	while(1)
	{
		switch (state) {
			case state_wait_for_command:
				stateWaitForCommand();
				break;
			case state_catch_and_roll:
				stateCatchAndRoll();
				state = state_wait_for_command;
				break;
			case state_roll_out:
				stateRollOut();
				state = state_wait_for_command;
				break;
			default:
				break;
		}
	}
}

static void stateWaitForCommand()
{
	uint8_t receivedData;

	while (USB_ReceiveData(&receivedData) == false)
	{
		vTaskDelay(1);
	}

	if (receivedData == COM_IDLE)	{
		MotorControl_Stop();
	}
	else if (receivedData == COM_CATCH_AND_ROLL)	{
		state = state_catch_and_roll;
	}
	else if (receivedData == COM_ROLL_OUT)	{
		state = state_roll_out;
	}
	else if (receivedData == PING)	{
		USB_Send_Data(PONG);
	}
	else	{
		USB_Send_Data('9');
	}

}

static void stateCatchAndRoll()
{
/*	if (InputReader_GetCatchInputState() == false)	{		//false - пустая оптопара.
		USB_Send_Data(FAIL_SWITCH);
		return;
	}*/
	int i;
	for (i = 0; i < 3; ++i) {
		//Захват форсированный
		MotorControl_Start(motorDirection_backward);
		vTaskDelay(50);
		MotorControl_ForceStop();
		vTaskDelay(50);
		MotorControl_Stop();

		//Протяжка бумаги до срабатывания концевика
		MotorControl_Start(motorDirection_forward);

		if (waitForCatch(500, false) == true)	{
			//Протяжка бумаги до отпускания концевика
			if (waitForCatch(1000, true) == true)	{

				MotorControl_Stop();
				USB_Send_Data(EVENT_CAUGHT_AND_ROLLED);
				return;
			}
			else	{
				//захват был, но бумага не вышла на сканирование.
				MotorControl_Stop();
				USB_Send_Data(FAIL_JAM);
				return;
			}
		}
		MotorControl_Stop();
		vTaskDelay(50);
	}
	USB_Send_Data(FAIL_CATCH);
}

static void stateRollOut()
{
	MotorControl_Start(motorDirection_forward);

	vTaskDelay(400);
	MotorControl_Stop();
	USB_Send_Data(EVENT_ROLLED_OUT);
}

static bool waitForCatch(uint32_t timeToWait, bool targetState)
{
	uint32_t time = 0;

	while(time < timeToWait)	{
		if (InputReader_GetCatchInputState() == targetState)	{		//false - пустая оптопара.
			return true;
		}
		time++;
		vTaskDelay(1);
	}
	return false;
}
