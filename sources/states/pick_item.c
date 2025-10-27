#include "defines.h"
#include "structs.h"
#include "helper.h"
#include "items.h"
#include "state_change.h"
#include "anim.h"
#include <stdio.h>
#include "states/states.h"
#include "audio.h"

void init_state_pick_items(struct PickItemsState *s) {
	s->num_item_choices = 3;
}

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

}

Rectangle get_rect_for_item_idx(int idx, int num_cards) {
	int rect_width = (SCREEN_WIDTH - 200-(20*num_cards)) / num_cards;
	int rect_height = rect_width * 1.5;
	Rectangle r = { 100 + idx*(rect_width+20), SCREEN_HEIGHT/2-rect_height/2, rect_width, rect_height };
	return r;
}

void state_pick_items(float dt, struct PickItemsState *state, struct GameState *gamestate) {

	PaddleData *choosing_paddle = state->choosing_player->paddle;

	int last_hovered = state->hovered_item;
	state->hovered_item = -1;
	for (int i=0; i<state->num_item_choices; i++) {
		// Mouse hovering
		Rectangle item_rect = get_rect_for_item_idx(i, state->num_item_choices);
		Rectangle mouse_rect = { GetMouseX(), GetMouseY(), 1, 1 };
		if (CheckCollisionRecs(item_rect, mouse_rect)) {
			state->hovered_item = i;
			if (last_hovered != i) { play_sfx(gamestate->music_mind, SFX_CARD_HIGHLIGHT); }
			break;
		}

	}

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && state->hovered_item != -1) {
		int item_idx = state->item_choices[state->hovered_item];
		choosing_paddle->items[item_idx] += 1;
		choosing_paddle->items_total[item_idx] += 1;

		play_sfx(gamestate->music_mind, SFX_CARD_SELECT);

		// Do any init of items
		if (item_idx == ITEM_CEREMONIAL_SWORD) {
			choosing_paddle->sword_timer = 0;
		}

		if (gamestate->player1->paddle->hp <= 0) {
			change_state_to_pick_items(gamestate, gamestate->player1);
		} else if (gamestate->player2->paddle->hp <= 0) {
			change_state_to_pick_items(gamestate, gamestate->player2);
		} else {
			change_state_to_pong(gamestate);
		}
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

	PaddleData *choosing_paddle = state->choosing_player->paddle;

	// Who is picking
	if (choosing_paddle->id == 1) {
		DrawTextCentered("Player 1 To Choose", SCREEN_WIDTH/2, 120, 40, choosing_paddle->color, TA_CENTER);
	} else if (choosing_paddle->id == 2) {
		DrawTextCentered("Player 2 To Choose", SCREEN_WIDTH/2, 120, 40, choosing_paddle->color, TA_CENTER);
	}

	// Show items
	for (int i=0; i<state->num_item_choices; i++) {
		bool is_hovered = i == state->hovered_item;
		Rectangle item_rect = get_rect_for_item_idx(i, state->num_item_choices);
		Color card_color = HACKER_GREEN;

		if (is_hovered) { card_color = state->choosing_player->paddle->color; } 

		DrawRectangleLines(item_rect.x, item_rect.y, item_rect.width, item_rect.height, card_color);

		float margin = (item_rect.width-150)/2;

		DrawTexturePro(
			textures[state->item_choices[i]],
			(Rectangle){state->item_anim_frames[i]*150, 0, 150, 150},
			(Rectangle){item_rect.x+margin, item_rect.y+margin, 150, 150},
			(Vector2){0, 0},
			0,
			WHITE
		);

		if (is_hovered) {
			DrawTextCentered(
				item_info[state->item_choices[i]].description,
				item_rect.x+item_rect.width/2,
				item_rect.y+item_rect.height-15,
				14,
				card_color,
				TA_BOTTOM
			);
		} else {
			DrawTextCentered(
				item_info[state->item_choices[i]].label, 
				item_rect.x+item_rect.width/2,
				item_rect.y+item_rect.height-15,
				15,
				card_color,
				TA_BOTTOM
			 );
		}

	}
}
