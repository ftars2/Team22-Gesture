#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

void MoveMouseAbsolute(int x, int y) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    // Convert screen coordinates to absolute (0 to 65535 range)
    input.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN);
    input.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN);
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    // Send the mouse event
    SendInput(1, &input, sizeof(INPUT));
}
void MoveMouse(int deltaX, int deltaY) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    // Set mouse movement
    input.mi.dx = deltaX; // Horizontal movement (relative)
    input.mi.dy = deltaY; // Vertical movement (relative)
    input.mi.dwFlags = MOUSEEVENTF_MOVE;

    // Send the mouse event
    SendInput(1, &input, sizeof(INPUT));
}

void main3() {
    std::cout << "Waiting for 5 seconds before moving the mouse to the center of the screen...\n";
    Sleep(5000); // Wait for 5 seconds

    // Move mouse to the center of the screen
    MoveMouseAbsolute(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2);
    Sleep(1000);
    MoveMouseAbsolute(GetSystemMetrics(SM_CXSCREEN) / 4, GetSystemMetrics(SM_CYSCREEN) / 4);
    MoveMouse(100, 100);
    Sleep(500);
    MoveMouse(100, 100);
    Sleep(500);
    MoveMouse(100, 100);
    Sleep(500);
    MoveMouse(100, 100);
    Sleep(500);
    MoveMouse(100, 100);
    Sleep(500);
    MoveMouse(100, 100);
    return;
}

void SimulateTyping(const std::string& text) {
    for (char c : text) {
        SHORT keyInfo = VkKeyScan(c); // Get virtual key and shift state
        if (keyInfo == -1) {
            std::cerr << "Cannot type character: " << c << '\n';
            continue;
        }

        // Extract virtual key and shift state
        BYTE virtualKey = LOBYTE(keyInfo);
        BYTE shiftState = HIBYTE(keyInfo);

        // Press Shift if required
        if (shiftState & 1) {
            INPUT shiftPress = { 0 };
            shiftPress.type = INPUT_KEYBOARD;
            shiftPress.ki.wVk = VK_SHIFT;
            SendInput(1, &shiftPress, sizeof(INPUT));
        }

        // Simulate key press
        INPUT keyPress = { 0 };
        keyPress.type = INPUT_KEYBOARD;
        keyPress.ki.wVk = virtualKey;
        SendInput(1, &keyPress, sizeof(INPUT));

        // Simulate key release
        keyPress.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &keyPress, sizeof(INPUT));

        // Release Shift if required
        if (shiftState & 1) {
            INPUT shiftRelease = { 0 };
            shiftRelease.type = INPUT_KEYBOARD;
            shiftRelease.ki.wVk = VK_SHIFT;
            shiftRelease.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &shiftRelease, sizeof(INPUT));
        }
    }
}

void main2() {
    string userInput;
    cout << "Enter text to simulate typing: ";
    Sleep(1000);
    getline(cin, userInput);

    std::cout << "Typing in 5 seconds...\n";
    Sleep(5000); // Wait before typing

    SimulateTyping(userInput);

    return;
}
// gesture keypress test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

void SimulateKeyPress(WORD virtualKey) {
    INPUT input = { 0 };

    // Key down
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = virtualKey; // Virtual-key code
    SendInput(1, &input, sizeof(INPUT));

    // Key up
    input.ki.dwFlags = KEYEVENTF_KEYUP; // Key release
    SendInput(1, &input, sizeof(INPUT));
}


void main1()
{
    cout << "Hello World!\ntyping it back for you! in 5 seconds!!";

    Sleep(5000);
    SimulateKeyPress(0x48); // H
    SimulateKeyPress(0x45); // E
    SimulateKeyPress(0x4C); // L
    SimulateKeyPress(0x4C); // L
    SimulateKeyPress(0x4F); // O

    SimulateKeyPress(VK_SPACE); // O

    SimulateKeyPress(0x57); // W
    SimulateKeyPress(0x4F); // O
    SimulateKeyPress(0x52); // R
    SimulateKeyPress(0x4C); // L
    SimulateKeyPress(0x44); // D

    // Simulate pressing Enter
    //SimulateKeyPress(VK_RETURN);
    return;
}
void main4() {
    SimulateKeyPress(0xB3);//pause play
    Sleep(5000);
    SimulateKeyPress(0xB3);//pause play
}
int main() {
    cout << "which program do you want to run:\n1) type hello world\n2) computer repeats after user\n3) cursor moves around screen\n4)pause/play media\nAnything else) close program\n";
    char val;
    cin >> val;
    cin.ignore();
    if (val == '1') {
        main1();
    }
    else if (val == '2') {
        main2();
    }
    else if (val == '3') {
        main3();
    }
    else if (val == '4') {
        main4();
    }

    return 0;

}