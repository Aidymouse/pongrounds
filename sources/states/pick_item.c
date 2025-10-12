#include "defines.h"
#include "structs.h"
#include "helper.h"
#include "items.h"
#include "state_change.h"
#include "anim.h"
#include <stdio.h>

void roll_items(struct PickItemsState *state) {
	// Load all items into swap back array
	int items[NUM_ITEMS];
	for (int i=0; i<NUM_ITEMS; i++) {
		items[i] = i;
	}
	int items_remaining = NUM_ITEMS;
	
	// Pick item from swap back array, removing it from the array
	for (int i=0; i<state->num_item_choices; i++) {
		int item_idx = randInt(0, items_remaining-1);
		state->item_choices[i] = items[item_idx];
		items[item_idx] = items[items_remaining-1];
		items_remaining--;
		state->item_anim_timers[i] = FRAME_TIME;
		state->item_anim_frames[i] = 0;
	}

	

	//state->item_choices[0] = ITEM_RUSSIAN_SECRETS;
}

Rectangle get_rect_for_item_idx(int idx, int num_cards) {
	int rect_width = (SCREEN_WIDTH - 200-(20*num_cards)) / num_cards;
	int rect_height = rect_width * 1.5;
	Rectangle r = { 100 + idx*(rect_width+20), SCREEN_HEIGHT/2-rect_height/2, rect_width, rect_height };
	return r;
}

void state_pick_items(float dt, struct PickItemsState *state, struct GameState *gamestate) {

	state->hovered_item = -1;
	for (int i=0; i<state->num_item_choices; i++) {
		// Mouse hovering
		Rectangle item_rect = get_rect_for_item_idx(i, state->num_item_choices);
		Rectangle mouse_rect = { GetMouseX(), GetMouseY(), 1, 1 };
		if (CheckCollisionRecs(item_rect, mouse_rect)) {
			state->hovered_item = i;
			break;
		}

	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state->hovered_item != -1) {
		int item_idx = state->item_choices[state->hovered_item];
		state->choosing_paddle->items[item_idx] += 1;
		state->choosing_paddle->items_total[item_idx] += 1;

		// Do any init of items
		if (item_idx == ITEM_CEREMONIAL_SWORD) {
			state->choosing_paddle->sword_timer = 0;
		}

		change_state_to_pong(gamestate);
	}
	
	// Update animations
	for (int i=0; i<state->num_item_choices; i++) {
		state->item_anim_timers[i] -= dt;
		if (state->item_anim_timers[i] <= 0 && state->item_anim_frames[i] < CARD_ANIM_FRAMES-1) {
			state->item_anim_frames[i] += 1;
			state->item_anim_timers[i] = FRAME_TIME;
		}
	}
}

void draw_pick_items(struct PickItemsState *state, Texture2D *textures) {

	// Who is picking
	if (state->choosing_paddle->id == 1) {
		DrawTextCentered("Player 1 To Choose", SCREEN_WIDTH/2, 120, 40, state->choosing_paddle->color);
	} else if (state->choosing_paddle->id == 2) {
		DrawTextCentered("Player 2 To Choose", SCREEN_WIDTH/2, 120, 40, state->choosing_paddle->color);
	}

	// Show items
	for (int i=0; i<state->num_item_choices; i++) {
		Rectangle item_rect = get_rect_for_item_idx(i, state->num_item_choices);
		Color card_color = HACKER_GREEN;
		if (i==state->hovered_item) { card_color = RED; } 

		DrawRectangleLines(item_rect.x, item_rect.y, item_rect.width, item_rect.height, card_color);

		float margin = (item_rect.width-150)/2;

		//draw_from_animation(textures[ITEM_TIME_WIZARDS_CHRONOMETER], state->item_animations[i], item_rect);
		DrawTexturePro(
			textures[state->item_choices[i]],
			(Rectangle){state->item_anim_frames[i]*150, 0, 150, 150},
			(Rectangle){item_rect.x+margin, item_rect.y+margin, 150, 150},
			(Vector2){0, 0},
			0,
			WHITE
		);

		DrawTextCentered( item_labels[state->item_choices[i]], item_rect.x+item_rect.width/2, item_rect.y+item_rect.height-25, 15, card_color);

	}
}
