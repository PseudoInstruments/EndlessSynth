#pragma once

//Gui config values
//TODO put into config.ini

#include "ofMain.h"

const string GUI_APP_TITLE = "EndlessDrumMachine Gui 1.0";

const int SCR_W = 1280;
const int SCR_H = 720;
const int SCR_FPS = 30;

//TODO separate sound/gui in separate apps,
//Sound - no window, OSC-controlled, 100 FPS (or more)
//Gui - window, electronics control, OSC sends to sound.