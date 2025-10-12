#include "structs.h"

#include "states/pick_item.h"
#include "states/pong.h"
#include "obj/obj.h"
#include "helper.h"

#include <stdio.h>

void change_state_plain(enum GAME_STATES new_state, struct GameState *state) {
	
	if (new_state == STATE_PICK_ITEM) {
		state->state = new_state;
				
	} else if (new_state == STATE_PONG) {
		state->state = new_state;

	}
}

void change_state_to_pick_items(struct GameState *state, PaddleData *choosing) {
	struct PickItemsState *pick_items_state = state->pick_items_state;
	roll_items(pick_items_state);
	pick_items_state->choosing_paddle = choosing;
	state->state = STATE_PICK_ITEM;
}

/** TO PONG **/

void handle_sentient_hand(PaddleData *thief, PaddleData *victim) {
	while (thief->items[ITEM_SENTIENT_HAND] > 0) {
		int num_to_steal = 0;
		int steal_choices[NUM_ITEMS] = {0};
		for (int item_idx=0; item_idx<NUM_ITEMS; item_idx++) {
			if (victim->items[item_idx] > 0) {
				steal_choices[num_to_steal] = item_idx;
				num_to_steal += 1;
			}
		}
		
		if (num_to_steal <= 0) { return; }

		int stolen = randInt(0, num_to_steal-1);
		int stolen_item_idx = steal_choices[stolen];
		victim->items[stolen_item_idx] -= 1;
		thief->items[stolen_item_idx] += 1;
		thief->items[ITEM_SENTIENT_HAND] -= 1;
	}
	
	//steal_item(state->player1->paddle, state->player2->paddle);		
}

/** */
void change_state_to_pong(struct GameState *state) {
	int nerd_glasses = state->player1->paddle->items[ITEM_NERD_GLASSES] + state->player2->paddle->items[ITEM_NERD_GLASSES];
	if (nerd_glasses > 0) {
		state->camera->zoom = 0.8 - 0.05 * (nerd_glasses-1);
	}

	if (state->state == STATE_PICK_ITEM) {
		state->pong_state->ball_respawn_timer = BALL_RESPAWN_DELAY;
	}

	// Get rid of paddle clones
	for (int i=2; i<state->pong_state->num_paddles; i++) {
		state->pong_state->paddles[i].delete_me = true;
	}
	paddle_cleanup(state->pong_state);
	
	paddle_refresh(state->player1->paddle, state->player2->paddle, state);
	paddle_refresh(state->player2->paddle, state->player1->paddle, state);
	//ball_refresh(&(state->pong_state->balls[0])); // No ball refresh cos pongs ball respawn handles it. Also it didnt work? idk why

	// Sentient Hand
	// TODO: UX on sentient hand is terrible - really hard to tell who stole what
	if (state->player1->paddle->items[ITEM_SENTIENT_HAND] > 0) {
		handle_sentient_hand(state->player1->paddle, state->player2->paddle);
	}
	if (state->player2->paddle->items[ITEM_SENTIENT_HAND] > 0) {
		handle_sentient_hand(state->player2->paddle, state->player1->paddle);
	}

	state->state = STATE_PONG;
}

void change_state_to_victory(struct GameState *state, struct PlayerData *victor) {
	state->victory_state->victor = victor;
	state->state = STATE_VICTORY;
}

void change_state_to_menu(struct GameState *state) {
	state->state = STATE_MENU;
}
