#include <iostream>
#include <fstream>
#include <map>
#include <string>

/*
aproved macros:
Space, Alt, Win, Ctrl, Lshift, Caps, Tab, Esc, Enter, F1-F12, Volume_up, Volume_down, Mute, Pause/Play, Backspace, Left_arrow, Right_arrow, Up_arrow, Down_arrow, any other expected values in "".
example formats: 
    Shift + Tab
    Ctrl + "s"
    ctrl + "t" + "youtube.com" + Enter
https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes 
*/

const std::string filename = "settings.txt";
const std:: map<std::string, std::string> settings={
    {"push", "leftclk"},
    {"left", "Larrow"},
    {"right", "Rarrow"},
    {"up", "Uarrow"},
    {"down", "Darrow"},
    {"sensitivity", "10"},
    {"mouse threshold", "10"}
};

// void createDefaultSettings(std::map<std::string, std::string>& settings) {
//     settings = {
//         {"push", "leftclk"},
//         {"left", "Larrow"},
//         {"right", "Rarrow"},
//         {"up", "Uarrow"},
//         {"down", "Darrow"},
//         {"sensitivity", "10"},
//         {"mouse threshold", "10"}
//     };
// }
void writeSettingsToFile(const std::map<std::string, std::string>& settings) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : settings) {
            file << pair.first << ": " << pair.second << "\n";
        }
        file.close();
    } else {
        std::cerr << "Error: Could not write to settings file!" << std::endl;
    }
}

void loadSettings(std::map<std::string, std::string>& settings) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // createDefaultSettings(settings);
        writeSettingsToFile(settings);
        std::cout << "Settings set to default." << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find(": ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 2);
            settings[key] = value;
        }
    }
    file.close();
    
    std::map<std::string, std::string> defaultSettings;
    // createDefaultSettings(defaultSettings);
    
    bool updated = false;
    std::string updatedSettings;
    for (const auto& pair : defaultSettings) {
        if (settings.find(pair.first) == settings.end()) {
            settings[pair.first] = pair.second;
            updatedSettings += pair.first + "\n";
            updated = true;
        }
    }
    
    if (updated) {
        writeSettingsToFile(settings);
        std::cout << "Missing settings added to file:\n" << updatedSettings << std::endl;
    }
    
    std::cout << "Settings loaded." << std::endl;
}

int main() {
    std::map<std::string, std::string> settings;
    loadSettings(settings);
    
    std::cout << "Loaded Settings:\n";
    for (const auto& pair : settings) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }
    
    return 0;
}
