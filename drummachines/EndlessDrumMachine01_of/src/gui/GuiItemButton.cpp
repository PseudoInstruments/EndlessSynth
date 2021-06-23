#include "GuiItemButton.h"


//--------------------------------------------------------------
void GuiItemButton::setup(int2 pos) {
	GuiItem::setup(pos);
}

//--------------------------------------------------------------
void GuiItemButton::mouseMoved(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemButton::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemButton::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemButton::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemButton::update() {

}

//--------------------------------------------------------------
void GuiItemButton::draw() {
	ofSetColor(0);
	ofNoFill();
	ofDrawCircle(pos_.x, pos_.y, GUI_BUTTON_RAD);

}

//--------------------------------------------------------------
