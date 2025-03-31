// gesture implementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <windows.h>

using namespace std;

// Map predefined actions to virtual key codes
map<string, int> predefinedKeys = {
    {"Space", VK_SPACE}, {"Alt", VK_MENU}, {"Win", VK_LWIN}, {"Ctrl", VK_CONTROL}, {"Shift", VK_SHIFT},
    {"Caps", VK_CAPITAL}, {"Tab", VK_TAB}, {"Esc", VK_ESCAPE}, {"Enter", VK_RETURN}, {"Backspace", VK_BACK},
    {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6},
    {"F7", VK_F7}, {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
    {"Volume_up", VK_VOLUME_UP}, {"Volume_down", VK_VOLUME_DOWN}, {"Mute", VK_VOLUME_MUTE},
    {"Pause/Play", VK_MEDIA_PLAY_PAUSE}, {"Left_arrow", VK_LEFT}, {"Right_arrow", VK_RIGHT},
    {"Up_arrow", VK_UP}, {"Down_arrow", VK_DOWN}, {"N/A", 0XFE}, {"OnScreenKeyboard", 0XFF}
};


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
void LeftClick(bool Ldown) {//send once to push LMB down, another to release
    INPUT inputs[1] = {};
    if (Ldown) {
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    }
    else {
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }
    SendInput(1, inputs, sizeof(INPUT));

}
void RightClick(bool Rdown) { // Send once to press RMB down, another time to release
    INPUT inputs[1] = {};
    inputs[0].type = INPUT_MOUSE;

    if (Rdown) {
        inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN; // Corrected
    }
    else {
        inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;   // Corrected
    }

    SendInput(1, inputs, sizeof(INPUT));
}

// Function to press a key
void pressKey(WORD key, bool hold) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));

    if (!hold) {
        Sleep(50); // Small delay for a tap action
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

// Function to release a key
void releaseKey(WORD key) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

// Function to process the vector of strings
void processKeySequence(const vector<string>& sequence) {
    vector<int> heldKeys;

    for (const auto& command : sequence) {
        if (predefinedKeys.count(command)) {
            // Predefined action (press and hold)
            int key = predefinedKeys[command];
            if (key == 0XFF) {
                // Special custom action: Ctrl + Win + "O"
                pressKey(VK_CONTROL, true);
                pressKey(VK_LWIN, true);
                pressKey('O', true);
                pressKey('O', false);
                pressKey(VK_LWIN, false);
                pressKey(VK_CONTROL, false);
            }
            else if (key == 0XFE) {
                // "N/A" does nothing by design
            }
            else {
                pressKey(key, true);
                heldKeys.push_back(key);
            }
        }
        else if (command.front() == '"' && command.back() == '"') {
            // If the command is a multi-character string enclosed in quotes
            string content = command.substr(1, command.size() - 2);

            // Release all previously held keys before typing the new string
            for (int key : heldKeys) {
                releaseKey(key);
            }
            heldKeys.clear();

            // Add a delay before typing the string (500 ms)
            Sleep(500);  // Delay before typing the string

            // Type each character of the string
            for (char c : content) {
                WORD key = VkKeyScan(c);
                pressKey(key, false);  // Press and release each character
            }
        }
        else if (command.size() == 1 && command.front() == '"') {
            // If it's a single character enclosed in quotes, hold it down
            WORD key = VkKeyScan(command[1]);
            pressKey(key, true);
            heldKeys.push_back(key);
        }
    }

    // Release all held keys at the end
    for (int key : heldKeys) {
        releaseKey(key);
    }
}

////int main() {
////    string user;
////    vector<string> commands;
////    cout << "type in commands as described in the settings file, type end to end input and execute commands\n";
////    while (cin >> user) {
////        if (user == "end") {
////            break;
////        }
////        commands.push_back(user);
////    }
////    processKeySequence(commands);
////    return 0;
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
