#include "header.h"
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
        } else if (!isdigit(str[i])) {
            return false; // Invalid character found
        }
    }
    return start < str.size(); // Ensure there's at least one digit
}

