#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <cstdio>
#include <cstdlib>
#include "plus.h"
using namespace std;

vector<string> splitStringByPlus(const string& input) {
    vector<string> result;
    string current;
    bool inQuotes = false;

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == '"') {
            inQuotes = !inQuotes;  // Toggle quote state
            current += c;
        }
        else if (c == '+' && !inQuotes) {
            // Remove leading and trailing whitespace from current
            current.erase(current.begin(), find_if(current.begin(), current.end(), [](int ch) { return !isspace(ch); }));
            current.erase(find_if(current.rbegin(), current.rend(), [](int ch) { return !isspace(ch); }).base(), current.end());

            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        }
        else {
            current += c;
        }
    }

    // Add the last segment
    current.erase(current.begin(), find_if(current.begin(), current.end(), [](int ch) { return !isspace(ch); }));
    current.erase(find_if(current.rbegin(), current.rend(), [](int ch) { return !isspace(ch); }).base(), current.end());

    if (!current.empty()) {
        result.push_back(current);
    }

    return result;


}

bool isNumber(const std::string& str) {
    if (str.empty()) return false;

    size_t start = (str[0] == '-' || str[0] == '+') ? 1 : 0; // Allow leading +/- 

    bool hasDecimal = false;
    for (size_t i = start; i < str.size(); i++) {
        if (str[i] == '.') {
            if (hasDecimal || i == start || i == str.size() - 1) {
                return false; // Multiple decimals or '.' at start/end
            }
            hasDecimal = true;
        }
        else if (!isdigit(str[i])) {
            return false; // Invalid character found
        }
    }
    return start < str.size(); // Ensure there's at least one digit
}

/*
aproved macros:
Lclick, Rclick, onScreenKeyboard, Space, Alt, Win, Ctrl, Lshift, Caps, Tab, Esc, Enter, F1-F12, Volume_up, Volume_down, Mute, Pause/Play, Backspace, Left_arrow, Right_arrow, Up_arrow, Down_arrow, any other expected values in "".
example formats:
    Shift + Tab
    Ctrl + "s"
    Ctrl + "t" + "youtube.com" + Enter
https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
*/
//name of file
const char* filename = "settings.txt";

//default settings for settings expecting number
const std::map<std::string, std::string> NDsettings = {
    {"mouse sensitivity", "10"},
    {"mouse threshold", "10"},
    {"usable range", "1000"},
    {"gesture threshold", "4"}
};

//default settings for gestures
const std::map<std::string, std::string> GDsettings = {
    {"up", "Up_arrow"},
    {"down", "Down_arrow"},
    {"left", "Left_arrow"},
    {"right", "Right_arrow"},
    {"pressdown", "Lclick"},
    {"pressup", "Rclick"},
    {"None", "N/A"}
};

// Define the alias for a pair of maps
// template <typename T1, typename T2>
// using MapPair = pair<map<string, vector<string>>, map<string, string>>;


const set<string> GVal = {
    "Space", "Alt", "Win", "Ctrl", "Shift",
    "Caps", "Tab", "Esc", "Enter", "Backspace","F1", "F2", "F3",
    "F4", "F5", "F6", "F7", "F8", "F9", "F10",
    "F11", "F12","Volume_up", "Volume_down", "Mute",
    "Pause/Play", "Left_arrow", "Right_arrow",
    "Up_arrow","Down_arrow", "OnScreenKeybard", "N/A", "Lclick", "Rclick"
};


//creates default settings file
void createDefault() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error, can't write to " << filename << endl;
    }
    else {
        cout << "Settings.txt created\n";
        //if wanted to write settings instructions, do it here,
        file << "For gesture settings there can be multiple key presses that are pressed in order. you can type approved commands, or put string/characters in \" \". If you type a character (ex: \"a\") then it will be held. If you type a string, all previously held buttons will be released. Approved macros will be held.\nAproved macros:\nLclick, Rclick, onScreenKeyboard, Space, Alt, Win, Ctrl, Lshift, Caps, Tab, Esc, Enter, F1-F12, Volume_up, Volume_down, Mute, Pause/Play, Backspace, Left_arrow, Right_arrow, Up_arrow, Down_arrow, any other expected values in \"\".\nexample formats:\nShift + Tab\nCtrl + \"s\"\nCtrl + \"t\" + \"youtube.com\" + Enter\n\nNumerical settings require a number.\n\n";
        file << "Gesture settings (type \"N/A\" for no action):\n";
        for (const auto& pair : GDsettings) {
            file << pair.first << ": " << pair.second << "\n";
        }
        file << "\nNumerical settings (Requires a value):\n";
        for (const auto& pair : NDsettings) {
            file << pair.first << ": " << pair.second << "\n";
        }
        file.close();
    }
}

void faultyFile(fstream& file) {
    string user;
    cerr << "file formatted incorrectly, type \"yes\" to set file to default\n";
    cin >> user;
    if (user == "yes") {
        file.close();
        remove(filename);
        createDefault();
    }
    else {
        exit(-1);
    }
}
//checks if there is a settings file
pair<map<string, vector<string>>, map<string, string>> checkSettings() {
    fstream file(filename);
    if (!file.is_open()) {
        cout << "settings not present, creating default settings.\n";
        createDefault();
    }
    else {//file is present, check if all settings have valid values 
        int type = 0;
        string line;
        string key;
        string val;
        size_t pos;
        map<string, string> GestureS;//contains read gesture settigns
        map<string, string> NumberS;//contains read numerical settings
        map<string, vector<string>> GestureFinal;//contains gestre settigns separated by action
        while (!file.eof()) {
            getline(file, line);
            if (line.length() == 0) {
                continue;
            }
            else if (type == 0) {//instructions
                if (string::npos != line.find("Gesture settings", 0, 16)) {
                    type = 1;
                }

            }
            else if (type == 1) {//gesture settings
                //store settings in 2 maps, then compare with supported values respectively

                pos = line.find(": ");
                if (pos != string::npos) {
                    key = line.substr(0, pos);
                    val = line.substr(pos + 2);
                    GestureS[key] = val;

                }
                else if (line.find("Numerical settings", 0, 19)) {
                    type = 2;
                    continue;
                }
                else {
                    cout << "numerical settings above this\n";
                    faultyFile(file);
                }
            }
            else if (type == 2) {
                pos = line.find(": ");
                if (pos != string::npos) {
                    key = line.substr(0, pos);
                    val = line.substr(pos + 2);
                    NumberS[key] = (val);

                }
                else if (line.length() == 0) {
                    continue;
                }
                else {
                    cout << line << endl;
                    cout << "type 2 above this\n";
                    faultyFile(file);
                }
            }

        }

        /* cout << "Parsed GestureS settings:\n";
         for (const auto& pair : GestureS) {
             cout << pair.first << " -> " << pair.second << endl;
         }*/

         //first check if a gesture is mapped, if it is not set it to default. if gesture mapped but incorrectly set it to default as well.
        for (auto G : GDsettings) {
            if (GestureS.count(G.first)) {
                //proceed to check value. create another for loop that compares each part of gesture macro with 
                vector<string> compare = splitStringByPlus(GestureS[G.first]);
                for (auto& i : compare) {
                    if (GVal.find(i) != GVal.end() || (i.front() == '"' && (i.back() == '"'))) {

                    }
                    else {
                        //unknown setting found, replace settings file.
                        cout << i << endl;
                        cout << "unknown settings found in gesture big loop\n";
                        faultyFile(file);
                    }

                }
                GestureFinal[G.first] = compare;
            }
            else {
                //unknown setting found, replace settings file.
                cout << G.first << "blabla" << endl;
                cout << "unknown settings found in gesture\n";
                faultyFile(file);
            }
        }
        //check if all numerical settings are set, then if they are set to numbers.
        for (auto N : NDsettings) {
            if (NumberS.count(N.first)) {
                if (isNumber(NumberS[N.first])) {
                    //value is good
                }
                else {
                    //unknown setting found, replace settings file.
                    cout << "tada " << N.first << "unknown settings found in number val\n";
                    faultyFile(file);
                }
            }
            else {
                //unknown setting found, replace settings file.
                cout << "tada " << "unknown settings found in number setting\n";
                faultyFile(file);
            }
        }
        //if you get here, both maps are good.
        cout << "it worked ig\n";
        return make_pair(GestureFinal, NumberS);
    }
}
void printSettings(const map<string, vector<string>>& GestureFinal, const map<string, string>& NumberS) {
    cout << "Gesture Settings:\n";
    for (const auto& pair : GestureFinal) {
        cout << pair.first << " -> ";
        for (const auto& action : pair.second) {
            cout << action << " ";
        }
        cout << endl;
    }

    cout << "\nNumerical Settings:\n";
    for (const auto& pair : NumberS) {
        cout << pair.first << " -> " << pair.second << endl;
    }
    char user;
    cout << "type anything to continue\n";
    cin >> user;
}

//int main() {
//    pair<map<string, vector<string>>, map<string, string>> dave = checkSettings();
//    printSettings(dave.first, dave.second);
    //char user;
    //cin >> user;
//    return 0;
//}