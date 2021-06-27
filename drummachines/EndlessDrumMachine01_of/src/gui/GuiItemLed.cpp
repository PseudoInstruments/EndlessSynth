#include "GuiItemLed.h"


//--------------------------------------------------------------
void GuiItemLed::setup(int2 pos) {
	GuiItem::setup(pos);
}

//--------------------------------------------------------------
bool GuiItemLed::mouseOver(int x, int y) {
	return false;
}

//--------------------------------------------------------------
void GuiItemLed::mouseMoved(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemLed::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemLed::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemLed::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemLed::mouseReleased() {

}

//--------------------------------------------------------------
void GuiItemLed::update() {

}

//--------------------------------------------------------------
void GuiItemLed::draw() {
	ofSetColor(0);
	ofNoFill();
	ofDrawCircle(pos_.x, pos_.y, GUI_LED_RAD);

}

//--------------------------------------------------------------
