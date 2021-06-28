#include "Sound.h"
#include "GuiAssert.h"

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
void Sound::audio_out(vector<float> &buffer, int frames, int channels) {
	gui_assert(channels == AUDIO_CHANNELS, "Sound::audio_out - expected " + ofToString(AUDIO_CHANNELS) + " channels");
	int k = 0;
	for (int i = 0; i < frames; i++) {
		buffer[k++] = 0;
		buffer[k++] = 0;
	}
}

//--------------------------------------------------------------
//Sending/receiving messages between GUI and Sound (in separate programs will be using shared memory or OSC)
void Sound::send_to_gui(const string &msg, int value) {

}

//--------------------------------------------------------------
void Sound::received_from_gui(const string &msg, int value) {

}

//--------------------------------------------------------------
