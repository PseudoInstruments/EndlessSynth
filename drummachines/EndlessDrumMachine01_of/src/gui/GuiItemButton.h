#pragma once

//Gui button

#include "ofMain.h"
#include "GuiConfig.h"
#include "GuiItem.h"

class GuiItemButton: public GuiItem {
public:
	void setup();
	void update();
	void draw();


	void mouseMoved(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
};

