#pragma once

//Sound class
//TODO move to a separate thread

#include "ofMain.h"
#include "SoundConfig.h"

class Sound {
public:
	void setup();
	void update();
	//void draw();

	//Sending/receiving messages between GUI and Sound (in separate programs will be using shared memory or OSC)
	void send_to_gui(const string &msg, int value);
	void received_from_gui(const string &msg, int value);

};



extern Sound SOUND;