#include "inputReader.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "pin_control.h"

#define NUM_OF_INPUTS	(2)

typedef enum	{
	catch = 0,
	broach = 1
} inputName_t;

struct	{
	pin_t input;
	bool state;
	bool prevState;
} inputData[NUM_OF_INPUTS];

static void task(void* param);

void InputReader_Init()
{
	inputData[0].prevState = true;
	inputData[0].state = true;
	inputData[1].prevState = true;
	inputData[1].state = true;

	Pin_InitInput(&(inputData[0].input), GPIOB, GPIO_Pin_8, pinPull_PullUp);
	Pin_InitInput(&(inputData[1].input), GPIOB, GPIO_Pin_9, pinPull_PullUp);

	xTaskCreate(
			task,
			"inputReaderTask",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY + 1,
			NULL);
}

bool InputReader_GetCatchInputState()
{
	return inputData[catch].state;
}

bool InputReader_GetBroachInputState()
{
	return inputData[broach].state;
}

static void task(void* param)
{
	while(1)
	{
		int i;

		for (i = 0; i < NUM_OF_INPUTS; ++i) {

			bool currentState = Pin_Read(&(inputData[i].input));

			if (inputData[i].prevState != inputData[i].state)	{
				if (currentState == inputData[i].prevState)		{
					inputData[i].state = currentState;
				}
			}
			inputData[i].prevState = currentState;
		}
		vTaskDelay(20);
	}
}
