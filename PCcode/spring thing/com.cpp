#include "com.h"
#include <iostream>
#include <vector>
#include <locale>
#include <codecvt>

// Define GUID_DEVCLASS_PORTS if not already defined
#ifndef GUID_DEVCLASS_PORTS
DEFINE_GUID(GUID_DEVCLASS_PORTS, 0x4D36E978, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);
#endif

std::wstring GetDeviceProperty(HDEVINFO deviceInfoSet, SP_DEVINFO_DATA& deviceInfoData, DWORD property)
{
    DWORD requiredSize = 0;
    SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, property, nullptr, nullptr, 0, &requiredSize);

    std::vector<BYTE> buffer(requiredSize);
    if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, property, nullptr, buffer.data(), requiredSize, nullptr))
    {
        return std::wstring(reinterpret_cast<wchar_t*>(buffer.data()));
    }
    return L"";
}

std::wstring GetComPort(HDEVINFO deviceInfoSet, SP_DEVINFO_DATA& deviceInfoData)
{
    HKEY deviceKey = SetupDiOpenDevRegKey(deviceInfoSet, &deviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
    if (deviceKey == INVALID_HANDLE_VALUE)
    {
        return L"";
    }

    WCHAR portName[256];
    DWORD size = sizeof(portName);
    DWORD type = 0;
    if (RegQueryValueExW(deviceKey, L"PortName", nullptr, &type, reinterpret_cast<LPBYTE>(portName), &size) == ERROR_SUCCESS && type == REG_SZ)
    {
        RegCloseKey(deviceKey);
        return std::wstring(portName);
    }

    RegCloseKey(deviceKey);
    return L"";
}

std::string FindComPortForDevice(const std::wstring& targetDeviceDescription)
{
    HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT | DIGCF_PROFILE);
    if (deviceInfoSet == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to get device information set." << std::endl;
        return "bad";
    }

    SP_DEVINFO_DATA deviceInfoData = {};
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    std::wstring comPort;
    for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); ++i)
    {
        std::wstring deviceDescription = GetDeviceProperty(deviceInfoSet, deviceInfoData, SPDRP_FRIENDLYNAME);
        std::wstring portName = GetComPort(deviceInfoSet, deviceInfoData);

        if (deviceDescription.find(targetDeviceDescription) != std::wstring::npos && !portName.empty())
        {
            // Format the COM port as "\\.\COMX" for COM10 and above
            if (portName.find(L"COM") == 0)
            {
                comPort = L"\\\\.\\" + portName;
            }
            break;
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);

    // Convert std::wstring to std::string (narrow string)
    static std::string narrowComPort;
    if (!comPort.empty())
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        narrowComPort = converter.to_bytes(comPort);
    }

    // Return as const char*
    return narrowComPort.empty() ? "bad" : narrowComPort;
}