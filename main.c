#include "FreeRTOS.h"
#include "task.h"

#include "usb_lib.h"
//#include "usb_desc.h"
#include "ring_buffer.h"
//#include "usb_pwr.h"
#include "usb_hal.h"

#include "usb_istr.h"
#include "usb_interface.h"

#include <stdint.h>
#include <stdio.h>

#include "inputReader.h"
#include "motor_control.h"
#include "device_state_machine.h"

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

void leds(void *param)
{
/*	while(1)
	{
		MotorControl_Start(motorDirection_backward);
		vTaskDelay(50);

		MotorControl_ForceStop();
		vTaskDelay(50);
		MotorControl_Stop();


		MotorControl_Start(motorDirection_forward);

		if (waitForCatch(500, false) == true)	{



			MotorControl_Start(motorDirection_forward);

			if (waitForCatch(2000, true) == true)	{

				MotorControl_Stop();

				vTaskDelay(400);






			}

		}

		MotorControl_Stop();
	}*/
}

int main(void)
{
	InputReader_Init();
	MotorControl_Init();
	DeviceStateMachine_Init();

	UsbInterface_Init();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

/*	xTaskCreate(
			leds,
			"leds",
			configMINIMAL_STACK_SIZE,
			NULL,
			tskIDLE_PRIORITY + 1,
			NULL);*/

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
