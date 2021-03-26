#include "Track.h"

const int n = 4;


//--------------------------------------------------------------
void Track::setup(int id0, string file_name, int ticks0, int speedup0) {
	id = id0;

	player_.load(file_name);

	ticks = ticks0;
	speedup = speedup0;

	rec_.resize(ticks);

}

//--------------------------------------------------------------
void Track::key_pressed() {
	//player_.play();
	int t = rec_tick_ % ticks;
	rec_[t] += 2;		//play several times
}

//--------------------------------------------------------------
void Track::update(float dt) {

}

//--------------------------------------------------------------
void Track::tick(int play_tick) {
	//record slower
	if (play_tick % speedup == 0) {
		rec_tick_ = (play_tick / speedup) % ticks;
		int t = rec_tick_;
		rec_[t] = max(rec_[t]-1, 0);  //erase record
	}

	//play
	play_tick_ = play_tick % ticks;
	if (rec_[play_tick_] > 0) {
		player_.play();
		//rec_[play_tick_] = max(rec_[play_tick_]-1, 0);  //erase record
	}
	else {
		//also play hat as metronome
		if (play_tick_ % 8 == 0 && id == 3) {
			player_.play();
		}
	}
	

}

//--------------------------------------------------------------
void Track::draw() {
	ofPushMatrix();
	int w1 = 30;
	int h1 = 30;
	ofTranslate(50, 250 + id * (h1+20));
	ofSetColor(255);
	for (int i = 0; i < ticks; i++) {
		if (!rec_[i]) ofNoFill();
		else ofFill();
		ofDrawRectangle(i * w1, 0, w1, h1);
	}
	ofFill();
	ofSetColor(0, 255, 0, 128);
	ofDrawRectangle(play_tick_ * w1, 0, w1, h1);
	ofSetColor(255, 0, 0, 128);
	ofDrawRectangle(rec_tick_ * w1, 0, w1, h1);

	ofPopMatrix();

}

//--------------------------------------------------------------
