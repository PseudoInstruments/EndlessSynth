#include "GuiItemKnob.h"
#include "SoundConfig.h"
#include "xmath.h"
#include "GuiAssert.h"

vector<int2> knob_vectors_;	//KNOB_RANGE-sized vector with positions for knob pointer
//--------------------------------------------------------------
void init_knob_vectors() {
	if (knob_vectors_.empty()) {
		int n = KNOB_RANGE;
		knob_vectors_.resize(n);
		for (int i = 0; i < n; i++) {
			float a = ofMap(i, 0, n - 1, GUI_KNOB_ANGLE0, GUI_KNOB_ANGLE1) * DEG_TO_RAD;
			float x = cos(a) * GUI_KNOB_RAD;
			float y = -sin(a) * GUI_KNOB_RAD;
			knob_vectors_[i] = int2(int(x), int(y));
		}
	}
}

//--------------------------------------------------------------
void GuiItemKnob::setup(int2 pos) {
	init_knob_vectors();

	GuiItem::setup(pos);
}

//--------------------------------------------------------------
bool GuiItemKnob::mouseOver(int x, int y) {
	return (abs(x - pos_.x) <= GUI_KNOB_RAD && abs(y - pos_.y) <= GUI_KNOB_RAD);
}

//--------------------------------------------------------------
void GuiItemKnob::mouseMoved(int x, int y, int button) {
	GuiItem::mouseMoved(x, y, button);
}

//--------------------------------------------------------------
void GuiItemKnob::mousePressed(int x, int y, int button) {
	GuiItem::mousePressed(x, y, button);
	mouse_value0_ = value_;
}

//--------------------------------------------------------------
void GuiItemKnob::mouseDragged(int x, int y, int button) {
	int delta = x - mouse_pos_.x - (y - mouse_pos_.y);
	set_value(mouse_value0_ + delta);

	GuiItem::mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void GuiItemKnob::mouseReleased(int x, int y, int button) {
	mouseDragged(x, y, button);
	GuiItem::mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void GuiItemKnob::mouseReleased() {
	GuiItem::mouseReleased();
}

//--------------------------------------------------------------
void GuiItemKnob::set_value(int v) {
	value_ = xclampi(v, 0, KNOB_RANGE-1);
}

//--------------------------------------------------------------
void GuiItemKnob::update() {

}

//--------------------------------------------------------------
void GuiItemKnob::draw() {
	ofSetColor(0);
	ofNoFill();
	ofDrawCircle(pos_.x, pos_.y, GUI_KNOB_RAD);

	//pointer
	ofSetColor(0);
	gui_assert(value_ >= 0 && value_ < KNOB_RANGE, "GuiItemKnob::draw() - bad value_");
	const auto &p = knob_vectors_[value_];
	ofDrawLine(pos_.x, pos_.y, pos_.x + p.x, pos_.y + p.y);
}

//--------------------------------------------------------------
