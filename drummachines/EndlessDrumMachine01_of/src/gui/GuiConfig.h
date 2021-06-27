#pragma once

//Gui config values
//TODO put into config.ini

#include "ofMain.h"

const string GUI_APP_TITLE = "EndlessDrumMachine 1.0";

const int GUI_APP_FPS = 60;

const int GUI_APP_W = 1280;
const int GUI_APP_H = 720;

// Knob
const int GUI_KNOB_RAD = 25;
const int GUI_KNOB_ANGLE0 = 270 - 30;
const int GUI_KNOB_ANGLE1 = -90 + 30;

// Button
const int GUI_BUTTON_RAD = 16;

// Led
const int GUI_LED_RAD = 16;




//TODO separate sound/gui in separate apps,
//Sound - no window, OSC-controlled, 100 FPS (or more)
//Gui - window, electronics control, OSC sends to sound.