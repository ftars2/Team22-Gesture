/*
 * usb_handler.c
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#include "usb_handler.h"
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

void USB_HID_Send_Mouse(int8_t x, int8_t y, uint8_t button) {
    uint8_t mouse_buffer[4] = {button, x, y, 0}; // HID Mouse Report
    USBD_HID_SendReport(&hUsbDeviceFS, mouse_buffer, sizeof(mouse_buffer));
}

void USB_HID_Send_Keyboard(uint8_t key, uint8_t modifier) {
    uint8_t keyboard_buffer[8] = {modifier, 0, key, 0, 0, 0, 0, 0}; // HID Keyboard Report
    USBD_HID_SendReport(&hUsbDeviceFS, keyboard_buffer, sizeof(keyboard_buffer));
}

