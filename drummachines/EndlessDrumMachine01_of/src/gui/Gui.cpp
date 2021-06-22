#include "Gui.h"

Gui GUI;

//--------------------------------------------------------------
void Gui::setup() {
	cout << "--------------------------------------" << endl;
	cout << GUI_APP_TITLE << endl;
	cout << "--------------------------------------" << endl;
	ofSetWindowShape(GUI_APP_W, GUI_APP_H);
	ofSetWindowTitle(GUI_APP_TITLE);
	ofSetFrameRate(GUI_APP_FPS);
}

//--------------------------------------------------------------
void Gui::keyPressed(int key) {

}

//--------------------------------------------------------------
void Gui::mouseMoved(int x, int y, int button) {
	for (auto it : items_) {
		it->mouseMoved(x, y, button);
	}
}

//--------------------------------------------------------------
void Gui::mousePressed(int x, int y, int button) {
	for (auto it : items_) {
		it->mousePressed(x, y, button);
	}
}

//--------------------------------------------------------------
void Gui::mouseDragged(int x, int y, int button) {
	for (auto it : items_) {
		it->mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void Gui::mouseReleased(int x, int y, int button) {
	for (auto it : items_) {
		it->mouseReleased(x, y, button);
	}
}


//--------------------------------------------------------------
//Sending/receiving messages between GUI and Sound (in separate programs will be using shared memory or OSC)
void Gui::send_to_sound(const string &msg, int value) {

}

//--------------------------------------------------------------
void Gui::received_from_sound(const string &msg, int value) {

}

//--------------------------------------------------------------
void Gui::update() {
	for (auto it : items_) {
		it->update();
	}

}

//--------------------------------------------------------------
void Gui::draw() {
	for (auto it : items_) {
		it->draw();
	}
}

//--------------------------------------------------------------