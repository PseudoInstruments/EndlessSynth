#include "InstrGui.h"
#include "xmath.h"
#include "InstrConfig.h"
#include "Gui.h"

//--------------------------------------------------------------
void InstrGui::setup() {
	for (int x = 0; x < DRUMS_N; x++) {
		for (int y = 0; y < DRUMS_CONTROLS_N; y++) {
			GUI.add_knob(get_pos(x, y + 1));
		}
	}
}

//--------------------------------------------------------------
//position of element, subx=1 - gives middle position x+0.5
int2 InstrGui::get_pos(int x, int y, int subx) {
	int2 res;
	res.x = int(ofMap(x + 1 + subx*0.5f + (x/4)*0.5, 0, INSTR_W + 2.5, 0, GUI_APP_W));
	
	res.y = xmapi(y + 1, 0, INSTR_H + 2, 0, GUI_APP_H);
	return res;
}

//--------------------------------------------------------------
void InstrGui::update() {


}

//--------------------------------------------------------------
