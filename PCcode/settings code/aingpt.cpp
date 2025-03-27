#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <unordered_set>

const std::string filename = "settings.txt";

// List of valid keybindings
const std::unordered_set<std::string> validKeys = {
    "Win", "Shift", "Ctrl", "Alt", "Enter", "Esc", "Tab", "CapsLock",
    "Space", "Backspace", "Delete", "Insert", "Home", "End", "PageUp", "PageDown",
    "ArrowUp", "ArrowDown", "ArrowLeft", "ArrowRight",
    "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
    "VolumeUp", "VolumeDown", "Mute", "PlayPause", "Stop", "NextTrack", "PrevTrack",
    "OnScreenKeyboard","N/A"
};

void createDefaultSettings(std::map<std::string, std::string>& settings, bool fullDefaults) {
    if (fullDefaults) {
        settings = {
            {"push", "leftclk"},
            {"left", "Larrow"},
            {"right", "Rarrow"},
            {"up", "Uarrow"},
            {"down", "Darrow"},
            {"sensitivity", "10"},
            {"mouse threshold", "10"}
        };
    } else {
        settings = {
            {"push", "N/A"},
            {"left", "N/A"},
            {"right", "N/A"},
            {"up", "N/A"},
            {"down", "N/A"},
            {"sensitivity", "10"},
            {"mouse threshold", "10"}
        };
    }
}

void writeSettingsToFile(const std::map<std::string, std::string>& settings) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const std::string& key : {"push", "left", "right", "up", "down"}) {
            if (settings.find(key) != settings.end()) {
                file << key << ": " << settings.at(key) << "\n";
            }
        }
        for (const std::string& key : {"sensitivity", "mouse threshold"}) {
            if (settings.find(key) != settings.end()) {
                file << key << ": " << settings.at(key) << "\n";
            }
        }
        file.close();
    } else {
        std::cerr << "Error: Could not write to settings file!" << std::endl;
    }
}

bool isValidKeySequence(const std::string& command) {
    std::istringstream iss(command);
    std::string token;
    while (std::getline(iss, token, '+')) {
        if (validKeys.find(token) == validKeys.end() && token.size() != 1) {
            return false;
        }
    }
    return true;
}

void loadSettings(std::map<std::string, std::string>& settings) {
    std::ifstream file(filename);
    bool fileExists = file.is_open();
    
    if (!fileExists) {
        createDefaultSettings(settings, true);
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
            if (isValidKeySequence(value)) {
                settings[key] = value;
            } else {
                std::cerr << "Warning: Invalid key sequence for " << key << " -> " << value << std::endl;
                settings[key] = "N/A";
            }
        }
    }
    file.close();
    
    std::map<std::string, std::string> defaultSettings;
    createDefaultSettings(defaultSettings, false);
    
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
