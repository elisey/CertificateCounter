#include "device_state_machine.h"

typedef enum	{
	state_wait_for_command,
	state_catch_and_roll,
	state_roll_out
} state_t;

static void task();

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

static void task()
{
	static state_t state = state_wait_for_command;

	while(1)
	{
		switch (state) {
			case state_wait_for_command:




				break;
			case state_catch_and_roll:

				break;
			case state_roll_out:

				break;
			default:
				break;
		}
	}
}
