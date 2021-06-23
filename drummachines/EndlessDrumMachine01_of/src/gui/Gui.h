#pragma once

//Gui class

#include "ofMain.h"
#include "GuiConfig.h"
#include "GuiItem.h"
#include "GuiItemButton.h"
#include "GuiItemKnob.h"
#include "GuiItemLed.h"

class Gui {
public:
	// Main
	void setup();
	void update();
	void draw();

	// Content
	GuiItemKnob* add_knob(int2 pos);
	void add_button(int2 pos);
	void add_led(int2 pos);

	// Events
	void keyPressed(int key);
	void mouseMoved(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);


	// Sending/receiving messages between GUI and Sound (in separate programs will be using shared memory or OSC)
	void send_to_sound(const string &msg, int value);
	void received_from_sound(const string &msg, int value);

protected:
	vector<GuiItem *> items_;	//Note: it not releases memory for items automatically

};


extern Gui GUI;