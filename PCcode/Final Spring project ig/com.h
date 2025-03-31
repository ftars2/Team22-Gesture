#pragma once
#ifndef COM_H
#define COM_H

#include <windows.h>
#include <setupapi.h>
#include <string>

#pragma comment(lib, "setupapi.lib")

std::wstring GetDeviceProperty(HDEVINFO deviceInfoSet, SP_DEVINFO_DATA& deviceInfoData, DWORD property);
std::wstring GetComPort(HDEVINFO deviceInfoSet, SP_DEVINFO_DATA& deviceInfoData);
std::string FindComPortForDevice(const std::wstring& targetDeviceDescription);
void ReadSerialPort(const char* portName);
void PrintLastError();
int com_main();
void SendSettingsData(HANDLE hSerial);
#endif // COM_PORT_H