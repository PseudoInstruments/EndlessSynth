#pragma once

//Gui config values
//TODO put into config.ini

#include "ofMain.h"

const string GUI_APP_TITLE = "EndlessDrumMachine Gui 1.0";

const int GUI_APP_FPS = 60;

const int GUI_APP_W = 1280;
const int GUI_APP_H = 720;

const int GUI_KNOB_RAD = 30;
const int GUI_LED_RAD = 30;
const int GUI_BUTTON_RAD = 30;



//TODO separate sound/gui in separate apps,
//Sound - no window, OSC-controlled, 100 FPS (or more)
//Gui - window, electronics control, OSC sends to sound.