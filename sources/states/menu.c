	#include "states/states.h"
	#include "structs.h"
	#include "helper.h"
	#include "raylib.h"
	#include <stdio.h>
	#include "state_change.h"
	#include <math.h>
	#include "defines.h"

	const int button_width = 250;
	const int button_height = 60;
	const Rectangle autospace = { .x = -1, .y = -1, .width = button_width, .height = button_height };

	int num_btns_main_menu = 5;
MenuButton main_menu[MAX_MENU_BUTTONS] = {
	{ .dims = (Rectangle){-1, -1, 0, 0}, .color = HACKER_GREEN, .text = "SUPER PONG", .id = BTN_TITLE, },
	{ .dims = autospace, .color = HACKER_GREEN, .text = "Play", .id = BTN_TO_PLAY, },
	{ .dims = autospace, .color = HACKER_GREEN, .text = "Controls", .id = BTN_TO_KEYBINDS, },
	{ .dims = autospace, .color = HACKER_GREEN, .text = "Credits", .id = BTN_CREDITS, },
	{ .dims = autospace, .color = HACKER_GREEN, .text = "Quit", .id = BTN_QUIT },
	0
};

int num_btns_controls = 1;
MenuButton controls_menu[MAX_MENU_BUTTONS] = {
	{ .dims = (Rectangle){50, SCREEN_HEIGHT-button_height-50, button_width, button_height}, .color = HACKER_GREEN, .text = "< Back", .id = BTN_TO_MAIN_MENU, },
	0
};


int num_btns_credits = 1;
MenuButton credits_menu[MAX_MENU_BUTTONS] = {
	{ .dims = (Rectangle){50, SCREEN_HEIGHT-button_height-50, button_width, button_height}, .color = HACKER_GREEN, .text = "< Back", .id = BTN_TO_MAIN_MENU, },
	0
};

int num_btns_play = 3;
MenuButton play_menu[MAX_MENU_BUTTONS] = {
	{ .dims = (Rectangle){SCREEN_WIDTH/2 - button_width - 25, SCREEN_HEIGHT/2 - button_height/2, button_width, button_height}, .color = HACKER_GREEN, .text = "One Player", .id = BTN_ONE_PLAYER, },
	{ .dims = (Rectangle){SCREEN_WIDTH/2 + 25, SCREEN_HEIGHT/2 - button_height/2, button_width, button_height}, .color = HACKER_GREEN, .text = "Two Player", .id = BTN_TWO_PLAYER, },
	{ .dims = (Rectangle){50, SCREEN_HEIGHT-button_height-50, button_width, button_height}, .color = HACKER_GREEN, .text = "< Back", .id = BTN_TO_MAIN_MENU, },
	0
};

int num_btns_pause = 2;
MenuButton pause_menu[MAX_MENU_BUTTONS] = {
	{ .dims = autospace, .color = HACKER_GREEN, .text = "Resume", .id = BTN_RESUME, },
	{ .dims = autospace, .color = HACKER_GREEN, .text = "Back to Main Menu", .id = BTN_STOP_PLAYING, },
	0
};

/** Inits the state menu */
void init_state_menu(MenuState *menu_state) {
	menu_state->hovered_id = BTN_NULL;
	menu_state->menu_id = MENU_MAIN;
	//menu_state->buttons = [];
	menu_state->bar_progress = 0;

	menu_state->rebinding = CTRL_NULL;

}

Vector2 infer_button_pos(MenuState *menu_state, MenuButton *btn, int button_idx, int num_buttons) {
	const int MARGIN = 20;

	Vector2 inferred = { .x = btn->dims.x, .y = btn->dims.y };
	if (btn->dims.x == -1) {
		inferred.x = SCREEN_WIDTH/2 - (btn->dims.width/2);
	}
	if (btn->dims.y == -1) {
		int total_height = (btn->dims.height + MARGIN) * num_buttons;
		int base_y = SCREEN_HEIGHT/2 - total_height/2;
		inferred.y = base_y + button_idx * (btn->dims.height + MARGIN);
	}
	return inferred;
}


/** STATE FNS **/

/** Finds the menu based on the menu id and sets up relevant pointers **/
void select_menu(MENU menu_id, MenuButton **buttons, int *num_buttons) {

	if (menu_id == MENU_MAIN) {
		*buttons = (MenuButton*)main_menu;
		*num_buttons = num_btns_main_menu;
	} else if (menu_id == MENU_PLAY) {
		*buttons = (MenuButton*)play_menu;
		*num_buttons = num_btns_play;
	} else if (menu_id == MENU_PAUSE) {
		*buttons = (MenuButton*)pause_menu;
		*num_buttons = num_btns_pause;
	} else if (menu_id == MENU_CONTROLS) {
		*buttons = (MenuButton*)controls_menu;
		*num_buttons = num_btns_controls;
	} else if (menu_id == MENU_CREDITS) {
		*buttons = (MenuButton*)credits_menu;
		*num_buttons = num_btns_credits;
	}

}

void perform_button_action(BUTTON button_id, MenuState *menu_state, GameState *state) {
	if (button_id == BTN_TO_PLAY) {
		menu_state->menu_id = MENU_PLAY;
	} else if (button_id == BTN_ONE_PLAYER) {
		// TODO
		printf("One Player TODO\n");
		change_state_to_pong(state);
		state->player1->paddle->brain = PB_COMPUTER;
	} else if (button_id == BTN_TWO_PLAYER) {
		change_state_to_pong(state);
	} else if (button_id == BTN_QUIT) {
		state->quit_please = true;

	} else if (button_id == BTN_CREDITS) {
		menu_state->menu_id = MENU_CREDITS;
		
	} else if (button_id == BTN_TO_MAIN_MENU) {
		menu_state->menu_id = MENU_MAIN;
	
	} else if (button_id == BTN_TO_KEYBINDS) {
		menu_state->menu_id = MENU_CONTROLS;
	} else if (button_id == BTN_RESUME) {
		state->state = state->pause_state->prior_state;
	} else if (button_id == BTN_STOP_PLAYING) {
		//state->state = state->pause_state->prior_state;
		change_state_to_menu(state);
	}

	menu_state->hovered_id = BTN_NULL;
}

/** State fn for the menu */
void state_menu(float dt, MenuState *menu_state, GameState *state) {

	Vector2 mouse = GetMousePosition();
	
	menu_state->hovered_id = BTN_NULL;

	MenuButton *buttons;
	int num_buttons;
	select_menu(menu_state->menu_id, &buttons, &num_buttons);

	for (int b=0; b<num_buttons; b++) {
		MenuButton *btn = &buttons[b];
		Vector2 inferred = infer_button_pos(menu_state, btn, b, num_buttons);
		Rectangle collision = btn->dims;
		if (btn->dims.x == -1) { collision.x = inferred.x; }
		if (btn->dims.y == -1) { collision.y = inferred.y; }

		if (CheckCollisionPointRec(mouse, collision)) {
			menu_state->hovered_id = btn->id;

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				perform_button_action(btn->id, menu_state, state);
			}
		}
	}

	menu_state->bar_progress += MENU_SPEED * dt;	
	if (menu_state->bar_progress > MENU_LINE_DIST) {
		menu_state->bar_progress -= MENU_LINE_DIST;
	}
}





/** DRAW FNS **/

void draw_cool_background(MenuState *menu_state) {
	// Draw cool background
	float num_lines = 6;
	int gradient_length = 200;
	//int spacer = SCREEN_HEIGHT/2;
	int spacer = 20;
	float space = (SCREEN_WIDTH) / (num_lines-1);
	//float space = 100;
	//float innerspace = space/SCREEN_WIDTH
	


	float mid = SCREEN_WIDTH/2;
	for (int l=-50; l<num_lines+50; l++) {
		
		float line_x = space * l;
		float dist_from_mid = mid-line_x;
		float inner_x = mid - dist_from_mid*0.002;
		DrawLine(line_x, 0, inner_x, SCREEN_HEIGHT/2, HACKER_GREEN);
		DrawLine(line_x, SCREEN_HEIGHT, inner_x, SCREEN_HEIGHT/2, HACKER_GREEN);
	}

	float line_y = 0;
 	for (int l=0; line_y < (SCREEN_HEIGHT/2); l++) {
		line_y = pow(1.02, l*MENU_LINE_DIST - menu_state->bar_progress);

		DrawLine(0, SCREEN_HEIGHT/2-line_y, SCREEN_WIDTH, SCREEN_HEIGHT/2-line_y, HACKER_GREEN);
		DrawLine(0, SCREEN_HEIGHT/2+line_y, SCREEN_WIDTH, SCREEN_HEIGHT/2+line_y, HACKER_GREEN);
	}



	int bg = BG_COLOR;
	Color bg_color = GetColor(bg);
	Color bg_trans = GetColor(bg);
	bg_trans.a = 0;
	DrawRectangleGradientV(0, SCREEN_HEIGHT/2 - spacer/2 - gradient_length, SCREEN_WIDTH, gradient_length, bg_trans, bg_color);
	DrawRectangle(0, SCREEN_HEIGHT/2 - spacer/2, SCREEN_WIDTH, spacer, bg_color);
	DrawRectangleGradientV(0, SCREEN_HEIGHT/2 + spacer/2, SCREEN_WIDTH, gradient_length, bg_color, bg_trans);
}

/** */
void button_draw(MenuButton *btn, bool hovered) {
		Color h = btn->color;
		h.a = 128;
		if (hovered) {
			DrawRectangleRec(btn->dims, h);
		}
		DrawRectangleLinesEx(btn->dims, 2, btn->color);
		DrawTextCentered(btn->text, btn->dims.x + btn->dims.width/2, btn->dims.y+btn->dims.height/2, 20, btn->color, TA_CENTER);
}

void draw_menu(MenuState *menu_state) {


	// Draw the menu
	if (menu_state->menu_id != MENU_PAUSE) {
		draw_cool_background(menu_state);
	}

	MenuButton *buttons;
	int num_buttons;
	select_menu(menu_state->menu_id, &buttons, &num_buttons);
	
	if (num_buttons == 0) { return; }

	for (int b=0; b<num_buttons; b++) {
		MenuButton btn = buttons[b];
		if (btn.id == BTN_TITLE) {
			DrawTextCentered("SUPER PONG", SCREEN_WIDTH/2, 110, 100, HACKER_GREEN, TA_CENTER);
		} else {
			MenuButton copy = btn;
			Vector2 inferred = infer_button_pos(menu_state, &btn, b, num_buttons);
			copy.dims.x = inferred.x;
			copy.dims.y = inferred.y;

			/*
			if (menu_state->menu_id == MENU_CONTROLS) {
				if (b == 0) { 
					copy.text = "Left"; 
					DrawText("Left", copy.dims.x, copy.dims.y, 20, HACKER_GREEN);
					glfwGetKeyName();
				}
			}
			*/

			button_draw(&copy, menu_state->hovered_id == btn.id);
		}
	}

	/** Controls **/
	Color bg_controls = GetColor(BG_COLOR);
	bg_controls.a = 128;

	const int col_width = (SCREEN_WIDTH-200)/3;
	const int total_width = col_width*3;
	int y = 100;
	const int font_size = 30;
	const int row_height = font_size + 15;
	const int col1x = SCREEN_WIDTH/2 - total_width/2;
	const int p1x = col1x + col_width;
	const int p2x = p1x + col_width;
	if (menu_state->menu_id == MENU_CONTROLS) {
		DrawRectangle(50, 50, SCREEN_WIDTH-100, SCREEN_HEIGHT - 100 - button_height - 50, bg_controls);

		DrawText("Player 1", p1x, y, font_size, ORANGE);
		DrawText("Player 2", p2x, y, font_size, BLUE);

		y += row_height;
		DrawText("Left", col1x, y, font_size, HACKER_GREEN);
		DrawText("A", p1x, y, font_size, HACKER_GREEN);
		DrawText("Left", p2x, y, font_size, HACKER_GREEN);

		y += row_height;
		DrawText("Right", col1x, y, font_size, HACKER_GREEN);
		DrawText("D", p1x, y, font_size, HACKER_GREEN);
		DrawText("Right", p2x, y, font_size, HACKER_GREEN);

		y += row_height;
		DrawText("Up", col1x, y, font_size, HACKER_GREEN);
		DrawText("W", p1x, y, font_size, HACKER_GREEN);
		DrawText("Up", p2x, y, font_size, HACKER_GREEN);

		y += row_height;
		DrawText("Down", col1x, y, font_size, HACKER_GREEN);
		DrawText("S", p1x, y, font_size, HACKER_GREEN);
		DrawText("Down", p2x, y, font_size, HACKER_GREEN);

		y += row_height;
		DrawText("Use Items", col1x, y, font_size, HACKER_GREEN);
		DrawText("Space", p1x, y, font_size, HACKER_GREEN);
		DrawText("P", p2x, y, font_size, HACKER_GREEN);

		y += row_height;
		DrawText("Dash", col1x, y, font_size, HACKER_GREEN);
		DrawText("Shift", p1x, y, font_size, HACKER_GREEN);
		DrawText("O", p2x, y, font_size, HACKER_GREEN);
	}

	/** Credits **/
	y = 90;
	if (menu_state->menu_id == MENU_CREDITS) {
		DrawRectangle(50, 50, SCREEN_WIDTH-100, SCREEN_HEIGHT - 100 - button_height - 50, bg_controls);

		DrawTextCentered("PROGRAMMING", SCREEN_WIDTH/2, y, 40, HACKER_GREEN, TA_CENTER);
		DrawTextCentered("Aidan Habedank", SCREEN_WIDTH/2, y+40, 40, HACKER_GREEN, TA_CENTER);

		y += 130;
		DrawTextCentered("GRAPHICS", SCREEN_WIDTH/2, y, 40, HACKER_GREEN, TA_CENTER);
		DrawTextCentered("Jeffrey Yu", SCREEN_WIDTH/2, y+40, 40, HACKER_GREEN, TA_CENTER);

		y += 130;
		DrawTextCentered("MUSIC", SCREEN_WIDTH/2, y, 40, HACKER_GREEN, TA_CENTER);
		DrawTextCentered("Jacob Wesley Tantleff", SCREEN_WIDTH/2, y+40, 40, HACKER_GREEN, TA_CENTER);
	}


}

