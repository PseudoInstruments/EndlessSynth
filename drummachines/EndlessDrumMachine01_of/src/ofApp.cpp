#include "ofApp.h"
#include "Sound.h"
#include "Gui.h"

//Sound works in a separate thread, makes sound generation and electronics control
//Gui - window

//--------------------------------------------------------------
void ofApp::setup(){
	GUI.setup();
	SOUND.setup();

	instr_gui_.setup();

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
