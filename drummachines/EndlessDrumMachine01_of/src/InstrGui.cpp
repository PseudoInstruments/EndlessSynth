#include "InstrGui.h"
#include "xmath.h"
#include "InstrConfig.h"
#include "Gui.h"

//--------------------------------------------------------------
void InstrGui::setup() {
	//Global control
	for (int i = 0; i < 4; i++) {
		GUI.add_knob(get_pos(i, 0));
	}

	//Drums
	for (int x = 0; x < DRUMS_N; x++) {
		//knobs
		for (int y = 0; y < DRUMS_CONTROLS_N; y++) {
			GUI.add_knob(get_pos(x, y + 1));
		}
		//beat button
		GUI.add_button(get_pos(x, INSTR_H - 1));
	}

	int x1 = 0;
	int y1 = INSTR_H - 2;
	//Sound control
	GUI.add_knob(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1++, y1));

	//Play control
	GUI.add_button(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1++, y1));
	GUI.add_led(get_pos(x1++, y1));

	//Presets banks
	GUI.add_led(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1, y1));
	GUI.add_button(get_pos(x1++, y1, 1));
	GUI.add_button(get_pos(x1, y1));
	GUI.add_button(get_pos(x1++, y1, 1));
	GUI.add_button(get_pos(x1++, y1));

	//Presets
	GUI.add_button(get_pos(x1, y1));
	GUI.add_button(get_pos(x1++, y1, 1));
	GUI.add_button(get_pos(x1, y1));
	GUI.add_button(get_pos(x1++, y1, 1));
	GUI.add_button(get_pos(x1++, y1));
	GUI.add_button(get_pos(x1++, y1));

}

//--------------------------------------------------------------
//position of element, subx=1 - gives middle position x+0.5
int2 InstrGui::get_pos(int x, int y, int subx) {
	int2 res;
	res.x = int(ofMap(x + 1 + subx*0.5f + (x/4)*0.5, 0, INSTR_W + 2.5, 0, GUI_APP_W));
	
	res.y = xmapi(y + 1, 0, INSTR_H + 1, 0, GUI_APP_H);
	return res;
}

//--------------------------------------------------------------
void InstrGui::update() {


}

//--------------------------------------------------------------
