#include <stdio.h>
#include <stdlib.h>
#include "controls.h"
#include "structs.h"

struct PaddleControls p1_default = { P1_LEFT_KEY, P1_RIGHT_KEY, P1_DASH_KEY, P1_UP_KEY, P1_DOWN_KEY, P1_ITEM_KEY };
struct PaddleControls p2_default = { P2_LEFT_KEY, P2_RIGHT_KEY, P2_DASH_KEY, P2_UP_KEY, P2_DOWN_KEY, P2_ITEM_KEY };

void use_default_controls(PlayerData *p1, PlayerData *p2) {
	p1->controls = p1_default;
	p2->controls = p2_default;
}

void fallback_to_default_controls(PlayerData *p1, PlayerData *p2, FILE *file) {
	printf("!!! FAILED TO PARSE CONFIG, USING DEFAULT !!!\n");
	use_default_controls(p1, p2);
	fclose(file);
}


/** Reads 7 lines, emitting controls into PaddleControls.
 * Returns the control code on success. 0 if there's no control
 * */
int parse_control(char line[200]) {
	PaddleControls pc;

	char control_code[4] = {0};
	char control_name[16] = {0};

	int reading = 0; // 0 = control name , 1 = control code

	char cur = line[0];
	int i = 0;
	int line_idx = 0;
	while (cur != '\n') {
		if (reading == 0) {
			if (cur == ' ') {
				reading += 1;
				i = 0;
			} else {
				control_name[i] = cur;
				i++;
			}
		} else if (reading == 1) {
			if (i >= 3) {
				reading += 1;
				i = 0;
			} else if (cur == ' ') { 
				continue; 

			} else {
				control_code[i] = cur;
				i++;
			}

		}
		line_idx++;
		cur = line[line_idx];
		//printf("%c\n", cur);
	}

	//printf("%s: %s\n", control_name, control_code);
	
	int control = atoi(control_code);
	return control;
}

/** Loads paddle controls into paddles by reference */
void load_controls(PlayerData *p1, PlayerData *p2) {

	// I couldn't be bothered getting rebindable controls working so I commented out the code that reads in from a file.
	use_default_controls(p1, p2);

	/*
	FILE *ini;
	ini = fopen("superpong.ini", "r");

	if (ini == NULL) {
		fallback_to_default_controls(p1, p2, ini);
		return;
	}

	PaddleControls pc1;
	PaddleControls pc2;
	PaddleControls *cur_pc = &pc1;

	char ini_text[200];

	int config_idx = 0; // Only increments on valid config lines
	while (fgets(ini_text, 200, ini) != NULL) {

		if (ini_text[0] == '[' || ini_text[0] == '\n') {
			continue;
		}

		if (config_idx < 12) {
			int control = parse_control(ini_text);

			if (control == 0) {
				fallback_to_default_controls(p1, p2, ini);
				return;
			}

			int ctrl = config_idx % 6; // num controls

			if (ctrl == 0) {
				cur_pc->left = control;
			} else if (ctrl == 1) {	
				cur_pc->right = control;
			} else if (ctrl == 2) {
				cur_pc->up = control;
			} else if (ctrl == 3) {
				cur_pc->down = control;
			} else if (ctrl == 4) {
				cur_pc->item = control;
			} else if (ctrl == 5) {
				cur_pc->dash = control;
			}

			if (ctrl == 5) {
				cur_pc = &pc2;
			}

		}

		config_idx += 1;
	}


	p1->controls = pc1;
	p2->controls = pc2;
	//fallback_to_default_controls(p1, p2, ini);
	//fclose(ini);
	*/
}


