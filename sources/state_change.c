#include "structs.h"

#include "states/pick_item.h"

#include <stdio.h>

void change_state(enum GAME_STATES new_state, struct GameState *state) {
	
	if (new_state == PICK_ITEM) {
		roll_items(state->pick_items_state);
		state->state = new_state;
				
	} else if (new_state == PONG) {
		state->state = new_state;

	}
}
