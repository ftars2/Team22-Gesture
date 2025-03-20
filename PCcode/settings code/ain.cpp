#include <iostream>
#include <fstream>
#include <map>
#include <string>

const std::string filename = "settings.txt";

void createDefaultSettings(std::map<std::string, std::string>& settings) {
    settings = {
        {"push", "leftclk"},
        {"left", "Larrow"},
        {"right", "Rarrow"},
        {"up", "Uarrow"},
        {"down", "Darrow"},
        {"sensitivity", "10"}
    };
    
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& pair : settings) {
            file << pair.first << ": " << pair.second << "\n";
        }
        file.close();
        std::cout << "Settings set to default." << std::endl;
    } else {
        std::cerr << "Error: Could not create settings file!" << std::endl;
    }
}

void loadSettings(std::map<std::string, std::string>& settings) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        createDefaultSettings(settings);
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