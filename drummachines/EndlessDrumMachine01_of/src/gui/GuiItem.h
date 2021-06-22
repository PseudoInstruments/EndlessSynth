#pragma once

//Gui abstract item

#include "ofMain.h"
#include "GuiConfig.h"

class GuiItem {
public:
	void setup();
	void update();
	void draw();


	virtual void mouseMoved(int x, int y, int button);
	virtual void mousePressed(int x, int y, int button);
	virtual void mouseDragged(int x, int y, int button);
	virtual void mouseReleased(int x, int y, int button);
};

