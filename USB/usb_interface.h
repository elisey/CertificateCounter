#pragma once

#include "usb_type.h"
#include <stdbool.h>
#define USART_RX_DATA_SIZE   64

void UsbInterface_Init();

void Handle_USBAsynchXfer (void);
void USB_Send_Data(uint8_t data);

bool USB_ReceiveData(uint8_t *data);
void USB_PutReceivedData(uint8_t data);

