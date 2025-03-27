#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <string>
#include <tchar.h>

void ListSerialPortsAndConnectToDevice(const std::string& targetDeviceName) {
    HDEVINFO deviceInfoSet;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD i;

    // Get device information set for all installed COM ports
    deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to get device information." << std::endl;
        return;
    }

    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // Loop through all devices in the device information set
    for (i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); i++) {
        char deviceID[1024];
        if (SetupDiGetDeviceInstanceId(deviceInfoSet, &deviceInfoData, deviceID, sizeof(deviceID), NULL)) {
            // If this device matches the target device name, open its COM port
            std::string deviceIDStr(deviceID);
            if (deviceIDStr.find(targetDeviceName) != std::string::npos) {
                std::cout << "Found target device: " << deviceID << std::endl;

                // Now, get the associated COM port from the device's properties
                char portName[1024];
                if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME,
                                                     NULL, (PBYTE)portName, sizeof(portName), NULL)) {
                    std::cout << "Device Friendly Name: " << portName << std::endl;

                    // Now connect to the serial port (e.g., COM3, COM4, etc.)
                    std::string comPort = "COM3";  // Replace this with the exact COM port if you know it
                    HANDLE hComm = CreateFile(
                        comPort.c_str(),
                        GENERIC_READ | GENERIC_WRITE,
                        0,    // No sharing
                        NULL, // No security attributes
                        OPEN_EXISTING, // Open existing port only
                        0,    // No overlapped I/O
                        NULL  // No template file
                    );

                    if (hComm == INVALID_HANDLE_VALUE) {
                        std::cerr << "Failed to open the COM port: " << comPort << std::endl;
                    } else {
                        std::cout << "Successfully connected to " << comPort << std::endl;
                        // Now you can use hComm for communication
                    }
                }
                break;
            }
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
}

int main() {
    std::string targetDeviceName = "YourDeviceName";  // Replace with the name or part of the name of your device
    ListSerialPortsAndConnectToDevice(targetDeviceName);
    return 0;
}
