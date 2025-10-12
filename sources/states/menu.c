#include "states.h"
#include "structs.h"
#include "helper.h"
#include "raylib.h"
#include <stdio.h>
#include "state_change.h"

const int button_width = 200;
const int button_height = 80;

/** */
void init_state_menu(MenuState *menu_state) {
	menu_state->current_menu = MENU_MAIN;
	menu_state->hovered = NULL;

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
void state_menu(struct GameState *state) {
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
	if (menu_state->current_menu == &menu_state->main_buttons) {
		DrawTextCentered("SUPER PONG", SCREEN_WIDTH/2, 110, 100, HACKER_GREEN);
	}

	for (int b=0; b<menu_state->num_buttons; b++) {
		Button *btn = &menu_state->current_menu[b];
		button_draw(btn, menu_state->hovered == btn);
	}

	// Draw cool background
}
