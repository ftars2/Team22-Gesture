// Mousemovement test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
void MoveMouse(int deltaX, int deltaY) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;

    // Set mouse movement
    input.mi.dx = deltaX; // Horizontal movement (relative)
    input.mi.dy = deltaY; // Vertical movement (relative)
    input.mi.dwFlags = MOUSEEVENTF_MOVE;

    // Send the mouse event
    SendInput(1, &input, sizeof(INPUT));
}
void LeftClick(bool Ldown) {//send once to push LMB down, another to release
    INPUT inputs[1] = {};
    if (Ldown) {
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    }
    else{
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    }
    SendInput(1, inputs, sizeof(INPUT));

}
void RightClick(bool Rdown) {//send once to push RMB down, another to release
    INPUT inputs[1] = {};
    if (Rdown) {
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    }
    else {
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    }
    SendInput(1, inputs, sizeof(INPUT));
}
int main()
{
    int R= 1;//default ratio
    bool l =false, r=false;
    cout << "Enter change in xy position as \"X Y\"\n if you want to change sensitivity, then type \":\" followed by the the ratio you waant to change it to ( default is 1.0)\n if you want to left/right click, type l or r to click, then send it again to release\n to double click send it twice\n";
    string line, temp, temp2, temp3;
    while (getline(cin,line)) {
        if (line[0] == ':') {
            line.erase(0,1);
            if (line.empty()) {
                cout << "invalid ratio, try again\n";
            }
            else {
            R = stoi(line);
            cout << "ration changed to " << R <<endl;
            }
            cin.clear();
        }
        else if ((line.length() > 1) && (isdigit(line[0])|| line[0]=='-') && line.find(" ") != string::npos) {
            cout << "moving cursor\n";
            istringstream s(line);
            s >> temp>>temp2;
            cout << "moving cursor " << stoi(temp)*R << ", " << R*stoi(temp2) << " pixels\n";
            cout << "last chance \n";
            MoveMouse(R*stoi(temp), R*stoi(temp2));
        }
        else if (line[0] == 'l') {
            if (line[1] == 'l') {
                LeftClick(0);
                LeftClick(1);
                LeftClick(0);
                LeftClick(1);
                cout << "double clicked left\n";
            }
            LeftClick(l);
            if (l) {
                cout << "released left click\n";
            }
            else {
                cout << "clicked left click\n";
            }
            l = !l;
            cin.clear();
        }
        else if (line[0] == 'r') {
            if (line[1] == 'r') {
                RightClick(0);
                RightClick(1);
                RightClick(0);
                RightClick(1);
                cout << "double clicked right";
            }
            RightClick(r);
            if (r) {
                cout << "released right click\n";
            }
            else {
                cout << "clicked right click\n";
            }
            r = !r;
            cin.clear();
        }
        else {
            cout << "cant read input, try again\n";
        }
        //cin >> temp3;
    }
    return 0;
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
