#pragma once
#ifndef GESTURE_H
#define GESTURE_H
#include <windows.h>  // For WORD
#include <vector>     // For std::vector
#include <string>     // For std::string
using namespace std;

void pressKey(WORD key, bool hold);
void releaseKey(WORD key);
void processKeySequence(const vector<string>& sequence);
void LeftClick(bool Ldown);
void RightClick(bool Rdown);
void MoveMouse(int deltaX, int deltaY);


#endif 