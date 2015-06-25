#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_hal.h"
#include "usb_pwr.h"


static void HexToChar (uint32_t value , uint8_t *pbuf , uint8_t len);

void Set_USBClock(void)
{
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

void Enter_LowPowerMode(void)
{
	bDeviceState = SUSPENDED;
}

void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo = &Device_Info;

	if (pInfo->Current_Configuration != 0)	{
		bDeviceState = CONFIGURED;
	}
	else {
		bDeviceState = ATTACHED;
	}
}

void USB_Interrupts_Config(void)
{
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 16);
}

void USB_Cable_Config (FunctionalState NewState)
{

}

void Get_SerialNum(void)
{
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

	Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
	Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
	Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);

	Device_Serial0 += Device_Serial2;

	if (Device_Serial0 != 0) {
		HexToChar(Device_Serial0, &Virtual_Com_Port_StringSerial[2], 8);
		HexToChar(Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
	}
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void HexToChar (uint32_t value , uint8_t *pbuf , uint8_t len)
{
	uint8_t idx = 0;

	for (idx = 0; idx < len; idx++) {
		if (((value >> 28)) < 0xA) {
			pbuf[2 * idx] = (value >> 28) + '0';
		}
		else {
			pbuf[2 * idx] = (value >> 28) + 'A' - 10;
		}
		value = value << 4;
		pbuf[2 * idx + 1] = 0;
	}
}
