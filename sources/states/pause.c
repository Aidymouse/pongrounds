#include "states/states.h"
#include "states/pick_item.h"
#include "raylib.h"
#include "textures.h"

void init_state_pause(PauseState *pause_state) {
	init_state_menu(&pause_state->inner_menu);
	pause_state->inner_menu.menu_id = MENU_PAUSE;

	pause_state->prior_state = STATE_PONG;
	pause_state->pause_lock = false;
}

void unpause(GameState *game_state) {
	game_state->state = game_state->pause_state->prior_state;
}

void state_pause(float dt, GameState *game_state) {
	PauseState *pause_state = game_state->pause_state;

	state_menu(dt, &pause_state->inner_menu, game_state);

	if (IsKeyPressed(KEY_ESCAPE)) {
		if (!pause_state->pause_lock) { unpause(game_state); }
	} else {
		pause_state->pause_lock = false;
	}
}


void draw_pause(GameState *game_state) {
	PauseState *pause_state = game_state->pause_state;

	if (pause_state->prior_state == STATE_PICK_ITEM) {
		draw_pick_items(game_state->pick_items_state, tex_item_cards);
	}

	Color bg = GetColor(BG_COLOR);
	bg.a = 256*0.6;

	DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg);

	draw_menu(&(game_state->pause_state->inner_menu));
	
}
