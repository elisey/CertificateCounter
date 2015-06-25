#include "FreeRTOS.h"
#include "task.h"

#include "usb_lib.h"
//#include "usb_desc.h"
#include "hw_config.h"
//#include "usb_pwr.h"

#include "usb_istr.h"

#include <stdint.h>
#include <stdio.h>

#include "inputReader.h"
#include "motor_control.h"

bool waitForCatch(uint32_t timeToWait, bool targetState);


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

void leds(void *param)
{
	while(1)
	{
		MotorControl_Start(motorDirection_backward);
		vTaskDelay(50);

		MotorControl_ForceStop();
		vTaskDelay(50);
		MotorControl_Stop();


		MotorControl_Start(motorDirection_forward);

		if (waitForCatch(500, false) == true)	{

/*			MotorControl_Stop();
			vTaskDelay(500);*/

			MotorControl_Start(motorDirection_forward);

			if (waitForCatch(2000, true) == true)	{

				MotorControl_Stop();

				vTaskDelay(400);


/*				MotorControl_Start(motorDirection_forward);

				vTaskDelay(400);*/



			}

		}

		MotorControl_Stop();
		//vTaskDelay(200);

		//USB_Send_Data('A');
	}
}

bool waitForCatch(uint32_t timeToWait, bool targetState)
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

void USB_IRQ (void *param)
{
	//usbSem = xSemaphoreCreateBinary();

	//Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	while(1)
	{
		//xSemaphoreTake(usbSem, portMAX_DELAY);
		USB_Istr();
	}
}

int main(void)
{
	InputReader_Init();
	MotorControl_Init();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	xTaskCreate(
			leds,
			"leds",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY + 1,
			NULL);

	vTaskStartScheduler();
}

void vApplicationMallocFailedHook(void)
{
	while(1);
}

void vApplicationStackOverflowHook(void)
{
	while(1);
}
