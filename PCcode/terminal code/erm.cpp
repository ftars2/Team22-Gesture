#include <windows.h>
#include <iostream>

void PrintLastError() {
    DWORD error = GetLastError();
    LPVOID msgBuffer;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, error, 0, (LPSTR)&msgBuffer, 0, NULL);
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
    DCB dcbSerialParams = {0};
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
                    } else {
                        lineBuffer += buffer[i];
                    }
                }
            }
        } else {
            std::cerr << "Error reading from serial port!" << std::endl;
            PrintLastError();
            break;
        }
    }
    
    CloseHandle(hSerial);
}

int main() {
    const char* portName = "\\\\.\\COM19";  // Change if necessary (e.g., "\\.\COM10" for COM10+)
    ReadSerialPort(portName);
    return 0;
}
