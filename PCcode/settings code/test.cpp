#include <vector>
#include <string>
#include <algorithm>

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

#include <iostream>

int main() {
    auto test1 = splitStringByPlus("Shift + Tab");
    auto test2 = splitStringByPlus("Ctrl + \"s\"");
    auto test3 = splitStringByPlus("Ctrl + \"t\" + \"youtube.com\" + Enter");
    
    for (const auto& s : test1) std::cout << "'" << s << "' ";  // Output: 'Shift' 'Tab'
    std::cout << "\n";
    for (const auto& s : test2) std::cout << "'" << s << "' ";  // Output: 'Ctrl' '"s"'
    std::cout << "\n";
    for (const auto& s : test3) std::cout << "'" << s << "' ";  // Output: 'Ctrl' '"t"' '"youtube.com"' 'Enter'
    std::cout << "\n";
    
    return 0;
}