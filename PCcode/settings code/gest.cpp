#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <winuser.h>

using namespace std;

// Map predefined actions to virtual key codes
map<string, int> predefinedKeys = {
    {"Space", VK_SPACE}, {"Alt", VK_MENU}, {"Win", VK_LWIN}, {"Ctrl", VK_CONTROL}, {"Shift", VK_SHIFT},
        {"Caps", VK_CAPITAL}, {"Tab", VK_TAB}, {"Esc", VK_ESCAPE}, {"Enter", VK_RETURN},{"Delete", VK_DELETE}, {"Backspace", VK_BACK},
        {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4}, {"F5", VK_F5}, {"F6", VK_F6}, 
        {"F7", VK_F7}, {"F8", VK_F8}, {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
        {"Volume_up", VK_VOLUME_UP}, {"Volume_down", VK_VOLUME_DOWN}, {"Mute", VK_VOLUME_MUTE},
        {"Pause/Play", VK_MEDIA_PLAY_PAUSE}, {"Left_arrow", VK_LEFT}, {"Right_arrow", VK_RIGHT}, 
        {"Up_arrow", VK_UP}, {"Down_arrow", VK_DOWN},{"N/A",0XFE},{"OnScreenKeyboard",0XFF}
    };

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
            if(key==0XFF){
                pressKey(VK_CONTROL, true);
            pressKey(VK_LWIN, true);
            pressKey('O', true);
            pressKey('O', false);
            pressKey(VK_LWIN, false);
            pressKey(VK_CONTROL, false);
            }
            else if(key==0XFE){
                //does nothing by design
            }
            else{
                pressKey(key, true);
                heldKeys.push_back(key);
            }
        } 
        else if (command.front() == '"' && command.back() == '"') {
            string content = command.substr(1, command.size() - 2);
            if (content.size() == 1) {
                // Single character in quotes, hold it
                WORD key = VkKeyScan(content[0]);
                pressKey(key, true);
                heldKeys.push_back(key);
            } else {
                // Phrase in quotes, tap each character
                for (char c : content) {
                    WORD key = VkKeyScan(c);
                    pressKey(key, false);
                }
            }
        }
    }

    // Release all held keys at the end
    for (int key : heldKeys) {
        releaseKey(key);
    }
}

int main() {
    string user;
    vector<string> commands;
    cout<<"type end to end input\n";
    while(cin>>user){
        if (user=="end"){
            break;
        }
        commands.push_back(user);    
    }
    processKeySequence(commands);
    return 0;
}
