#pragma once

#include "ofMain.h"

class Track {

public:
	void setup(int id0, string file_name, int ticks0, int speedup0);
	void update(float dt);

	void tick(int play_tick);
	void draw();

	void key_pressed();

	int id;

	ofSoundPlayer player_;
	int ticks;
	int speedup;

	int tick_;

	vector<int> rec_;	//recording
	int rec_tick_ = 0;
	int play_tick_ = 0;

};
