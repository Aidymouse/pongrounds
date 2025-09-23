#include "structs.h"

#include "states/pick_item.h"
#include "states/pong.h"

#include <stdio.h>

void change_state_plain(enum GAME_STATES new_state, struct GameState *state) {
	
	if (new_state == PICK_ITEM) {
		state->state = new_state;
				
	} else if (new_state == PONG) {
		state->state = new_state;

	}
}

void change_state_to_pick_items(struct GameState *state, struct PaddleData *choosing) {
	struct PickItemsState *pick_items_state = state->pick_items_state;
	roll_items(pick_items_state);
	pick_items_state->choosing_paddle = choosing;
	state->state = PICK_ITEM;
}

void change_state_to_pong(struct GameState *state) {
	
	state->player1->score = 0;
	state->player2->score = 0;

	refresh_paddle(state->player1->paddle);
	refresh_paddle(state->player2->paddle);


	state->state = PONG;
}
