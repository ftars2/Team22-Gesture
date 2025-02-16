/*
 * usb_handler.h
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#ifndef USB_HANDLER_H
#define USB_HANDLER_H

#include "usbd_hid.h"  // USB HID definitions

void USB_HID_Send_Mouse(int8_t x, int8_t y, uint8_t button);
void USB_HID_Send_Keyboard(uint8_t key, uint8_t modifier);

#endif /* USB_HANDLER_H */

