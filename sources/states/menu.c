#include "states.h"
#include "structs.h"
#include "helper.h"
#include "raylib.h"
#include <stdio.h>
#include "state_change.h"
#include <math.h>

const int button_width = 200;
const int button_height = 80;

/** */
void init_state_menu(MenuState *menu_state) {
	menu_state->current_menu = MENU_MAIN;
	menu_state->hovered = NULL;
	menu_state->bar_progress = 0;

	// Set up main menu
	menu_state->num_main_buttons = 0;
	menu_state->main_buttons[0] = (Button){
		(Rectangle){ SCREEN_WIDTH/2 - button_width/2, SCREEN_HEIGHT/2 - button_height/2, button_width, button_height, },
		HACKER_GREEN,
		"Play",
		BTN_PLAY,
	};
	menu_state->num_main_buttons += 1;

	menu_state->main_buttons[1] = (Button){
		(Rectangle){ SCREEN_WIDTH/2 - button_width/2, SCREEN_HEIGHT/2 - button_height/2 + button_height + 20, button_width, button_height, },
		HACKER_GREEN,
		"Quit",
		BTN_QUIT,
	};
	menu_state->num_main_buttons += 1;
	
	// Switch menu to current menu
	menu_state->current_menu = menu_state->main_buttons;
	menu_state->num_buttons = menu_state->num_main_buttons;
}

/** */
void state_menu(float dt, struct GameState *state) {
	MenuState *menu_state = state->menu_state;

	Vector2 mouse = GetMousePosition();
	
	menu_state->hovered = NULL;
	for (int b=0; b<menu_state->num_buttons; b++) {
		Button *btn = &menu_state->current_menu[b];
		if (CheckCollisionPointRec(mouse, btn->dims)) {
			menu_state->hovered = btn;
			//printf("Hovering over a button yeah\n");
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				// Do the option
				if (btn->id == BTN_PLAY) {
					printf("Play\n");
					
					change_state_to_pong(state);
					menu_state->hovered = NULL;
				} else if (btn->id == BTN_QUIT) {
					printf("Quit\n");
					state->quit_please = true;
				}
			}
		}
	}

	menu_state->bar_progress += MENU_SPEED * dt;	
	if (menu_state->bar_progress > MENU_LINE_DIST) {
		menu_state->bar_progress -= MENU_LINE_DIST;
	}
}


/** */
void button_draw(Button *btn, bool hovered) {
		Color h = btn->color;
		h.a = 128;
		if (hovered) {
			DrawRectangleRec(btn->dims, h);
		}
		DrawRectangleLinesEx(btn->dims, 2, btn->color);
		DrawTextCentered(btn->text, btn->dims.x + btn->dims.width/2, btn->dims.y+btn->dims.height/2, 20, btn->color);
}

void draw_menu(MenuState *menu_state) {

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
		//line_y = log(l + menu_state->bar_progress) * 70; 
		line_y = pow(1.02, l*MENU_LINE_DIST - menu_state->bar_progress);
		//printf("%f\n", line_y);
		//float top_y = 
		DrawLine(0, SCREEN_HEIGHT/2-line_y, SCREEN_WIDTH, SCREEN_HEIGHT/2-line_y, HACKER_GREEN);
		DrawLine(0, SCREEN_HEIGHT/2+line_y, SCREEN_WIDTH, SCREEN_HEIGHT/2+line_y, HACKER_GREEN);
		//DrawLine(0, SCREEN_HEIGHT-line_y, SCREEN_WIDTH, SCREEN_HEIGHT-line_y, HACKER_GREEN);
		//DrawLine(0, l*MENU_LINE_DIST, SCREEN_WIDTH, l*MENU_LINE_DIST, RED);
	}
	/*
	*/

	for (int i=0; i<10; i++) {
	}


	int bg = BG_COLOR;
	Color bg_color = GetColor(bg);
	bg_color.a = 255;
	Color bg_trans = GetColor(bg);
	bg_trans.a = 0;
	DrawRectangleGradientV(0, SCREEN_HEIGHT/2 - spacer/2 - gradient_length, SCREEN_WIDTH, gradient_length, bg_trans, bg_color);
	DrawRectangle(0, SCREEN_HEIGHT/2 - spacer/2, SCREEN_WIDTH, spacer, bg_color);
	DrawRectangleGradientV(0, SCREEN_HEIGHT/2 + spacer/2, SCREEN_WIDTH, gradient_length, bg_color, bg_trans);


	// Draw the menu
	if (menu_state->current_menu == &menu_state->main_buttons) {
		DrawTextCentered("SUPER PONG", SCREEN_WIDTH/2, 110, 100, HACKER_GREEN);
	}

	for (int b=0; b<menu_state->num_buttons; b++) {
		Button *btn = &menu_state->current_menu[b];
		button_draw(btn, menu_state->hovered == btn);
	}
}
