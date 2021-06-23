#pragma once

//Class for visual representing the drum machine.

#include "ofMain.h"
#include "int2.h"
#include "GuiConfig.h"
#include "InstrConfig.h"

class InstrGui {
public:
	void setup();
	void update();
protected:
	int2 get_pos(int x, int y, int subx = 0);	//position of element, subx=1 - gives middle position x+0.5


};

