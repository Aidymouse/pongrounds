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
	int nerd_glasses = state->player1->paddle->items[ITEM_NERD_GLASSES] + state->player2->paddle->items[ITEM_NERD_GLASSES];
	if (nerd_glasses > 0) {
		state->camera->zoom = 0.8 - 0.05 * (nerd_glasses-1);
	}


	refresh_paddle(state->player1->paddle, state->player2->paddle, state);
	refresh_paddle(state->player2->paddle, state->player1->paddle, state);

	state->state = PONG;
}
