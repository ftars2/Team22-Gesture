#include "com.h"
#include <iostream>
#include <vector>
#include <locale>
#include <codecvt>
#include "action.h"

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

void PrintLastError() {
    DWORD error = GetLastError();
    LPVOID msgBuffer;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, error, 0, (LPWSTR)&msgBuffer, 0, NULL);
    std::cerr << "Error: " << (char*)msgBuffer << std::endl;
    LocalFree(msgBuffer);
}

void ReadSerialPort(const char* portName) {
    HANDLE hSerial = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port!" << std::endl;
        PrintLastError();
        return;
    }

    // Configure serial port settings
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state!" << std::endl;
        PrintLastError();
        CloseHandle(hSerial);
        return;
    }

    dcbSerialParams.BaudRate = 460800;  // Match Tera Term baud rate
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.StopBits = ONESTOPBIT;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state!" << std::endl;
        PrintLastError();
        CloseHandle(hSerial);
        return;
    }

    // Clear serial buffers before reading
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    char buffer[256];
    DWORD bytesRead;
    std::string lineBuffer;

    std::cout << "Listening on " << portName << "..." << std::endl;
    while (true) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            if (bytesRead > 0) {
                for (DWORD i = 0; i < bytesRead; ++i) {
                    if (buffer[i] == '\n') {  // Print only when a newline character is received
                        //std::cout << "Received: " << lineBuffer << std::endl;
                        action(lineBuffer);
                        lineBuffer.clear();
                    }
                    else {
                        lineBuffer += buffer[i];
                    }
                }
            }
        }
        else {
            std::cerr << "Error reading from serial port!" << std::endl;
            PrintLastError();
            break;
        }
    }

    CloseHandle(hSerial);
}

int com_main() {
    std::wstring targetDeviceDescription = L"STMicroelectronics STLink Virtual COM Port";
    std::string comPort = FindComPortForDevice(targetDeviceDescription);
    printf("\033[2J\033[H");
    while (comPort == "bad") {
        std::cout << "Device not connected, will try again in 5 seconds.\n";
        Sleep(5000);
        comPort = FindComPortForDevice(targetDeviceDescription);
    }
    printf("\033[2J\033[H");
    const char* portName = comPort.c_str();  // Change if necessary (e.g., "\\.\COM10" for COM10+)
    ReadSerialPort(portName);
    return 0;
}
