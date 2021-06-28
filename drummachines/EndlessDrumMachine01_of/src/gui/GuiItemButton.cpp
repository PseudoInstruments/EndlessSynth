#include "GuiItemButton.h"


//--------------------------------------------------------------
void GuiItemButton::setup(int2 pos) {
	GuiItem::setup(pos);
}

//--------------------------------------------------------------
bool GuiItemButton::mouseOver(int x, int y) {
	return (abs(x - pos_.x) <= GUI_BUTTON_RAD && abs(y - pos_.y) <= GUI_BUTTON_RAD);
}

//--------------------------------------------------------------
void GuiItemButton::mouseMoved(int x, int y, int button) {
	GuiItem::mouseMoved(x, y, button);
}

//--------------------------------------------------------------
void GuiItemButton::mousePressed(int x, int y, int button) {
	GuiItem::mousePressed(x, y, button);
}

//--------------------------------------------------------------
void GuiItemButton::mouseDragged(int x, int y, int button) {
	GuiItem::mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void GuiItemButton::mouseReleased(int x, int y, int button) {
	GuiItem::mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void GuiItemButton::mouseReleased() {
	GuiItem::mouseReleased();
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
