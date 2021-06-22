#pragma once

//Gui class

#include "ofMain.h"
#include "GuiConfig.h"
#include "GuiItem.h"

class Gui {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mouseMoved(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);

	vector<GuiItem *> items_;	//Note: it not releases memory for items automatically


	//Sending/receiving messages between GUI and Sound (in separate programs will be using shared memory or OSC)
	void send_to_sound(const string &msg, int value);
	void received_from_sound(const string &msg, int value);
};


extern Gui GUI;