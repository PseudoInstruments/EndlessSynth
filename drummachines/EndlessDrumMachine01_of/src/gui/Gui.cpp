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
void Gui::add_item(GuiItem *item, int2 pos) {
	item->setup(pos);
	items_.push_back(item);
}

//--------------------------------------------------------------
GuiItemKnob* Gui::add_knob(int2 pos) {
	auto *item = new GuiItemKnob();
	add_item(item, pos);
	return item;
}

//--------------------------------------------------------------
GuiItemButton* Gui::add_button(int2 pos) {
	auto *item = new GuiItemButton();
	add_item(item, pos);
	return item;
}

//--------------------------------------------------------------
GuiItemLed* Gui::add_led(int2 pos) {
	auto *item = new GuiItemLed();
	add_item(item, pos);
	return item;
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
	ofBackground(230);
	for (auto it : items_) {
		it->draw();
	}
}

//--------------------------------------------------------------
