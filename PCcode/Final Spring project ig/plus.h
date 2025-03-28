#pragma once

#ifndef header_H
#define header_H
#include <vector>
#include <cstdbool>
#include <string.h>
using namespace std;
std::vector<std::string> splitStringByPlus(const std::string& input);
pair<map<string, vector<string>>, map<string, string>> checkSettings();
bool isNumber(const std::string& str);
void printSettings(const map<string, vector<string>>& GestureFinal, const map<string, string>& NumberS);
void faultyFile(fstream& file);
void createDefault();
#endif