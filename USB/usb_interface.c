#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_interface.h"
#include "usb_pwr.h"

#include "ring_buffer.h"

ringBuffer_t txBuffer, rxBuffer;

uint8_t  usbTxBuffer [USART_RX_DATA_SIZE];
uint32_t usbTxPrtIn = 0;
uint32_t usbTxPtrOut = 0;
uint32_t usbTxLength  = 0;

void Handle_USBAsynchXfer (void)
{
  
	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;

	if (usbTxPtrOut == USART_RX_DATA_SIZE) {
		usbTxPtrOut = 0;
	}

	if (usbTxPtrOut == usbTxPrtIn) {
		return;
	}

	if (usbTxPtrOut > usbTxPrtIn) /* rollback */
	{
		usbTxLength = USART_RX_DATA_SIZE - usbTxPtrOut;
	}
	else {
		usbTxLength = usbTxPrtIn - usbTxPtrOut;
	}

	if (usbTxLength > VIRTUAL_COM_PORT_DATA_SIZE) {
		USB_Tx_ptr = usbTxPtrOut;
		USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

		usbTxPtrOut += VIRTUAL_COM_PORT_DATA_SIZE;
		usbTxLength -= VIRTUAL_COM_PORT_DATA_SIZE;
	}
	else {
		USB_Tx_ptr = usbTxPtrOut;
		USB_Tx_length = usbTxLength;

		usbTxPtrOut += usbTxLength;
		usbTxLength = 0;
	}

	UserToPMABufferCopy(&usbTxBuffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
	SetEPTxCount(ENDP1, USB_Tx_length);
	SetEPTxValid(ENDP1);

}

void USB_Send_Data(uint8_t data)
{
  
  usbTxBuffer[usbTxPrtIn] = data;
  usbTxPrtIn++;

  if(usbTxPrtIn >= USART_RX_DATA_SIZE)
  {
    usbTxPrtIn = 0;
  }
}
