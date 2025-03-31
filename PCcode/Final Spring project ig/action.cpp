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
    static string click = "none";

    pos = line.find(": ");
    if (pos != string::npos) {
        key = line.substr(0, pos);
        val = line.substr(pos + 2);
        if (key == "Mode") {
            if (val == "Gesture") {

            
                if (click == "Push") {
                    click = "None";
                    LeftClick(false);
               }
                else if (click == "Pull") {
                    click = "None";
                    RightClick(false);
                }
            }
            mode = val;
        }
        else if ((key == "AVG_X" || key == "AVG_Y") && mode == "Mouse" && val != "nan" && val!="x") {
            try {
                float position = stof(val) - 3.5; // Convert to joystick-style movement
                float sensitivity = stof(dave.second["mouse sensitivity"]);
                float threshold = 0.5; // Adjust this value for a larger/smaller dead zone

                // Ignore small movements within the dead zone
                if (position > -threshold && position < threshold) {
                    return; // No movement if inside the dead zone
                }

                int moveAmount = static_cast<int>((position-threshold)*sensitivity); // Scale with sensitivity

                if (key == "AVG_X") {
                    MoveMouse(-1 * moveAmount, 0); // Move horizontally
                }
                else if (key == "AVG_Y") {
                    MoveMouse(0, 1 * moveAmount); // Move vertically
                }
            }
            catch (const invalid_argument&) {
                cout << "error\n\n";
                exit(-1);
            }
        }

        else if (key == "Gesture" && mode == "Gesture") {
            //cout << "gesture "<<val<<"\n\n\n"; 
            processKeySequence(dave.first[val]);
        }
        else if (key == "Click" && mode == "Mouse") {
            if (val == "Push") {
                if (click == "Push") {
                    // Does nothing
                }
                else if (click == "None") {
                    click = val;
                    LeftClick(true);
                }
                else if (click == "Pull") {
                    click = val;
                    RightClick(false);
                    LeftClick(true);
                }
            }
            else if (val == "Pull") {
                if (click == "Pull") {
                    // Does nothing
                }
                else if (click == "None") {
                    click = val;
                    RightClick(true);
                }
                else if (click == "Push") {
                    click = val;
                    LeftClick(false);
                    RightClick(true);
                }
            }
            else if (val == "None") {
                if (click == "Push") {
                    LeftClick(false);
                }
                else if (click == "Pull") {
                    RightClick(false);
                }
                click = val;
            }
        }
    }
        }