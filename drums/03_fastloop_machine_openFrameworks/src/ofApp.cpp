#include "ofApp.h"
#include "Track.h"

ofTrueTypeFont font_;

const int n = 4;
Track track[n];

float tempo_BPM = 140;  
float tick_period = 1.0 / 16 * (120.0 / tempo_BPM);
int ticks = 32;
int speedup = 2;


string file_names[4] = {
	"38898__sandyrb__hmg-kick-001.wav",
	"36249__sandyrb__h134-snare-01.wav",
	"89500__menegass__sd06.wav",
	"801__bdu__closehat.aiff"
};

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowTitle("Fastloop machine");
	ofSetFrameRate(60);

	font_.load("fonts/verdana.ttf", 16);


	for (int i = 0; i < n; i++) {
		track[i].setup(i, file_names[i], ticks, speedup);
	}
}

//--------------------------------------------------------------
float time_ = 0;
float tick_time_ = 0;

int play_tick_ = 0;

void ofApp::update() {
	float time = ofGetElapsedTimef();
	float dt = time - time_;
	time_ = time;
	for (int i = 0; i < n; i++) {
		track[i].update(dt);
	}

	if (time >= tick_time_ + tick_period) {
		tick_time_ += tick_period;
		for (int i = 0; i < n; i++) {
			track[i].tick(play_tick_);
		}
		play_tick_++;
	}


	ofSoundUpdate();

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	for (int i = 0; i < n; i++) {
		track[i].draw();
	}

	//tick
	{
		ofSetColor(255, 0, 0);
		if (play_tick_ % 8 == 0) ofFill();
		else ofNoFill();
		ofDrawRectangle(50, 50, 150, 150);
	}

	ofSetColor(255);
	font_.drawString("Press 1,2,3,4 - drums", 40, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') track[0].key_pressed();
	if (key == '2') track[1].key_pressed();
	if (key == '3') track[2].key_pressed();
	if (key == '4') track[3].key_pressed();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
