#include "GuiItemKnob.h"


//--------------------------------------------------------------
void GuiItemKnob::setup(int2 pos) {
	GuiItem::setup(pos);
}

//--------------------------------------------------------------
void GuiItemKnob::mouseMoved(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemKnob::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemKnob::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemKnob::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void GuiItemKnob::update() {

}

//--------------------------------------------------------------
void GuiItemKnob::draw() {
	ofSetColor(50);
	ofNoFill();
	ofDrawCircle(pos_.x, pos_.y, GUI_KNOB_RAD);
}

//--------------------------------------------------------------
