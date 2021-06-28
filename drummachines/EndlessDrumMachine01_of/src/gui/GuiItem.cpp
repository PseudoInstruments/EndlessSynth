#include "GuiItem.h"


//--------------------------------------------------------------
void GuiItem::setup(int2 pos) {
	pos_ = pos;
}

//--------------------------------------------------------------
bool GuiItem::mouseOver(int x, int y) {
	return false;
}

//--------------------------------------------------------------
void GuiItem::mouseMoved(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItem::mousePressed(int x, int y, int button) {
	moused_ = true;
	mouse_pos_ = int2(x, y);
}

//--------------------------------------------------------------
void GuiItem::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItem::mouseReleased(int x, int y, int button) {
	moused_ = false;
}

//--------------------------------------------------------------
void GuiItem::mouseReleased() {
	moused_ = false;
}

//--------------------------------------------------------------
void GuiItem::update() {

}

//--------------------------------------------------------------
void GuiItem::draw() {

}

//--------------------------------------------------------------
