#include "Sound.h"

Sound SOUND;

//--------------------------------------------------------------
void Sound::setup() {
	cout << "--------------------------------------" << endl;
	cout << SOUND_APP_TITLE << endl;
	cout << "--------------------------------------" << endl;

}

//--------------------------------------------------------------
void Sound::update() {

}

//--------------------------------------------------------------
//Sending/receiving messages between GUI and Sound (in separate programs will be using shared memory or OSC)
void Sound::send_to_gui(const string &msg, int value) {

}

//--------------------------------------------------------------
void Sound::received_from_gui(const string &msg, int value) {

}

//--------------------------------------------------------------
