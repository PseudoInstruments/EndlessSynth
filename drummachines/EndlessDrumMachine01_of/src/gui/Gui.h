#pragma once

//Gui class

#include "ofMain.h"
#include "GuiConfig.h"

class Gui {
public:
	void setup();
	void update();
	void draw();
};


extern Gui GUI;