// Final Spring project ig.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <cstdio>
#include <cstdlib>
#include "com.h"
#include "plus.h"
//#include "gesture.h"
using namespace std;

pair<map<string, vector<string>>, map<string, string>> dave; 
int main()
{
    dave = checkSettings();
    printSettings(dave.first, dave.second);
    cout << "Attempting to connect to device:\n";
    com_main();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
