#ifndef COM_H
#define COM_H

#include <windows.h>
#include <setupapi.h>
#include <string>

#pragma comment(lib, "setupapi.lib")

std::wstring GetDeviceProperty(HDEVINFO deviceInfoSet, SP_DEVINFO_DATA& deviceInfoData, DWORD property);
std::wstring GetComPort(HDEVINFO deviceInfoSet, SP_DEVINFO_DATA& deviceInfoData);
std::string FindComPortForDevice(const std::wstring& targetDeviceDescription);

#endif // COM_PORT_H