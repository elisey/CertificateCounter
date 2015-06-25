#pragma once

#include "usb_type.h"

#define USART_RX_DATA_SIZE   64

void Handle_USBAsynchXfer (void);
void USB_Send_Data(uint8_t data);

