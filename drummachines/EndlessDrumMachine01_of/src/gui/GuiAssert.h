#pragma once

//Errors control at Gui level

#include "ofMain.h"

#define gui_assert(x, message) { if (!(x)) ofSystemAlertDialog(string("GUI Error\n") + message); }


