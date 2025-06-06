// spring thing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "com.h"
#include <windows.h>
#include <iostream>

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
                        std::cout << "Received: " << lineBuffer << std::endl;
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

int main() {
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
