#include <string>
#include <set>
#include "gesture.h"
#include "shared.h"
#include <math.h>
using namespace std;

void action(string line) {
    cout << "\033[2K"<<line << endl;
    size_t pos;
    string key, val;
    static float Dx = 0;
    static float Dy = 0;
    int movex=0;
    int movey=0;
    static string mode = "Mouse";
    static bool gotx = false;
    static bool leftC = false, rightC = false;

    pos = line.find(": ");
    if (pos != string::npos) {
        key = line.substr(0, pos);
        val = line.substr(pos + 2);
       /* key.erase(0, key.find_first_not_of(" \t\r\n"));
        key.erase(key.find_last_not_of(" \t\r\n") + 1);
        val.erase(0, val.find_first_not_of(" \t\r\n"));
        val.erase(val.find_last_not_of(" \t\r\n") + 1);*/
        //cout << "Parsed key: [" << key << "] Value: [" << val << "]" << endl;

     //cout << "\n" << key << ":"<<val<<endl;

        if (key == "Mode") {
            mode = val;
            cout << val<<endl;
            //cout << "Mode\n\n\n";
        }
        else if (key == "Delta_X" && mode == "Mouse" && val != "nan") {
            try {
                Dx += stof(val); // Accumulate Delta_X
                gotx = true;
            }
            catch (const invalid_argument&) {
                return;
            }
        }
        else if (key == "Delta_Y" && gotx && mode == "Mouse" && val != "nan") {
            try {
                Dy += stof(val); // Accumulate Delta_Y
                gotx = false;

                float sensitivity = stof(dave.second["mouse sensitivity"]);
                int moveX = 0, moveY = 0;

                // Process X movement while it's still above threshold
                while (Dx * sensitivity > 1) {
                    moveX++;
                    Dx -= 1;
                }
                while (Dx * sensitivity < -1) {
                    moveX--;
                    Dx += 1;
                }

                // Process Y movement while it's still above threshold
                while (Dy * sensitivity > 1) {
                    moveY++;
                    Dy -= 1;
                }
                while (Dy * sensitivity < -1) {
                    moveY--;
                    Dy += 1;
                }

                // Move the mouse only if there's movement
                if (moveX != 0 || moveY != 0) {
                    MoveMouse(-1 * moveX * sensitivity, 1 * moveY * sensitivity);
                }

                // Reset Dx and Dy after movement
                Dx = 0;
                Dy = 0;
            }
            catch (const invalid_argument&) {
                cout << "error\n\n";
                exit(-1);
            }
        }

        else if (key == "Gesture" && mode == "Gesture") {
            cout << "gesture "<<val<<"\n\n\n"; 
            processKeySequence(dave.first[val]);
        }
        else if (key == "Click" && mode == "Mouse") {
            if (val == "Push") {
                if (!leftC) {
                    LeftClick(true);
                    leftC = true;
                }
                
            }
            else if(val=="Pull") {
            if (!rightC) {
                RightClick(true);
                rightC = true;
            }
            }
            else if (val == "None") {
                if (leftC) {
                    LeftClick(false);
                    leftC = false;
                }
                if (rightC) {
                    RightClick(false);
                    rightC = false;
                }
            }
        }
    }
}
