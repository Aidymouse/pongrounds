#include "defines.h"
#include "structs.h"
#include "helper.h"
#include "items.h"
#include <stdio.h>

void roll_items(struct PickItemsState *state) {
	// Load all items into swap back array
	int items[NUM_ITEMS];
	for (int i=0; i<NUM_ITEMS; i++) {
		items[i] = i;
	}
	int items_remaining = NUM_ITEMS;
	
	// Pick item from swap back array, removing it from the array
	printf("num item choices %d\n", state->num_item_choices);
	for (int i=0; i<state->num_item_choices; i++) {
		int item_idx = randInt(0, items_remaining-1);
		state->item_choices[i] = items[item_idx];
		printf("Picked %s\n",item_labels[items[item_idx]]);
		items[item_idx] = items[items_remaining-1];
		items_remaining--;
	}
}

Rectangle get_rect_for_item_idx(int idx, int num_cards) {
	int rect_width = (SCREEN_WIDTH - 200-(20*num_cards)) / num_cards;
	int rect_height = rect_width * 1.5;
	Rectangle r = { 100 + idx*(rect_width+20), SCREEN_HEIGHT/2-rect_height/2, rect_width, rect_height };
	return r;
}

void state_pick_items(struct PickItemsState *state) {
}

void draw_pick_items(struct PickItemsState *state) {

	for (int i=0; i<state->num_item_choices; i++) {
		Rectangle item_rect = get_rect_for_item_idx(i, state->num_item_choices);
		//printf("%d\n", i);
		DrawRectangleLines(item_rect.x, item_rect.y, item_rect.width, item_rect.height, WHITE);
		DrawTextCentered( item_labels[state->item_choices[i]], item_rect.x+item_rect.width/2, item_rect.y+item_rect.height-25, 15, WHITE);

	}
}
