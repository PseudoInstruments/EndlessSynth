#include "ofApp.h"
#include "Sound.h"
#include "Gui.h"
#include "GuiConsole.h"

//Sound works in a separate thread, makes sound generation and electronics control
//Gui - window

//--------------------------------------------------------------
void ofApp::setup(){
	GUI.setup();
	SOUND.setup();

	instr_gui_.setup();

	load();

	start_audio();
}

//--------------------------------------------------------------
void ofApp::save() {
	gui_log("Save");

}

//--------------------------------------------------------------
void ofApp::load() {
	gui_log("Load");

}

//--------------------------------------------------------------
void ofApp::exit() {
	save();
	gui_log("Exiting");
}

//--------------------------------------------------------------
void ofApp::start_audio() {
	cout << "Starting audio" << endl;
	//soundStream_.printDeviceList();

	ofSoundStreamSettings settings;
	// if you want to set the device id to be different than the default:
	//	auto devices = soundStream.getDeviceList();
	//	settings.setOutDevice(devices[3]);

	// you can also get devices for an specific api:
	//	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
	//	settings.setOutDevice(devices[0]);

	auto devices = soundStream_.getDeviceList(ofSoundDevice::Api::MS_DS);
	settings.setOutDevice(devices[0]);

	// or get the default device for an specific api:
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name:
	//	auto devices = soundStream.getMatchingDevices("default");
	//	if(!devices.empty()){
	//		settings.setOutDevice(devices[0]);
	//	}

#ifdef TARGET_LINUX
	// Latest linux versions default to the HDMI output
	// this usually fixes that. Also check the list of available
	// devices if sound doesn't work
	auto devices = soundStream.getMatchingDevices("default");
	if (!devices.empty()) {
		settings.setOutDevice(devices[0]);
	}
#endif

	settings.setOutListener(this);
	settings.sampleRate = SAMPLE_RATE;
	settings.numOutputChannels = AUDIO_CHANNELS;
	settings.numInputChannels = 0;
	settings.numBuffers = AUDIO_BUFFERS;
	settings.bufferSize = AUDIO_BUFFER_SIZE;
	soundStream_.setup(settings);
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer) {
	int frames = buffer.getNumFrames();
	int ch = buffer.getNumChannels();
	SOUND.audio_out(buffer.getBuffer(), frames, ch);
}

//--------------------------------------------------------------
void ofApp::update(){
	GUI.update();
	instr_gui_.update();

	SOUND.update();

}


//--------------------------------------------------------------
void ofApp::draw(){
	GUI.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	GUI.keyPressed(key);
	if (key == 's') save();
	if (key == 'l') load();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	GUI.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	GUI.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	GUI.mouseReleased(x, y, button);
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
