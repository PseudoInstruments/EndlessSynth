#pragma once

//Gui abstract item

#include "ofMain.h"
#include "GuiConfig.h"
#include "int2.h"

class GuiItem {
public:
	virtual void setup(int2 pos);
	virtual void update();
	virtual void draw();


	virtual bool mouseOver(int x, int y);
	virtual void mouseMoved(int x, int y, int button);
	virtual void mousePressed(int x, int y, int button);
	virtual void mouseDragged(int x, int y, int button);
	virtual void mouseReleased(int x, int y, int button);
	virtual void mouseReleased();
protected:
	int2 pos_;
	bool moused_ = false;
	int2 mouse_pos_;
};

