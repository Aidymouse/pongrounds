#include "defines.h"
#include "structs.h"
#include "helper.h"
#include "items.h"

void roll_items(struct PickItemsState *state) {
	for (int i=0; i<state->num_item_choices; i++) {
		int item_idx = randInt(0, NUM_ITEMS);
		state->item_choices[i] = item_idx;
		
	}
}

void state_pick_items(struct PickItemsState *state) {
}

void draw_pick_items() {
}
