#include "InstrGui.h"
#include "xmath.h"
#include "InstrConfig.h"

//--------------------------------------------------------------
void InstrGui::setup() {
	for (int x = 0; x < INSTR_W; x++) {
		for (int y = 0; y < INSTR_H; y++) {

		}
	}
}

//--------------------------------------------------------------
//position of element, subx=1 - gives middle position x+0.5
int2 InstrGui::get_pos(int x, int y, int subx) {
	int2 res;
	res.x = xmapi(x + 1, 0, INSTR_W + 2, 0, GUI_APP_W);
	return res;
}

//--------------------------------------------------------------
void InstrGui::update() {


}

//--------------------------------------------------------------
