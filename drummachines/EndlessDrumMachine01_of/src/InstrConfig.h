#pragma once

//Instrument config

#include "ofMain.h"

const int DRUMS_N = 16;
const int DRUMS_CONTROLS_N = 6;

//for GUI
const int INSTR_W = DRUMS_N;
const int INSTR_H = 9;




//TODO separate sound/gui in separate apps,
//Sound - no window, OSC-controlled, 100 FPS (or more)
//Gui - window, electronics control, OSC sends to sound.