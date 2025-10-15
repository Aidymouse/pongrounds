#include "states/states.h"
#include "helper.h"
#include <stdio.h>
#include "state_change.h"

void init_state_victory(VictoryState *victory_state) {
	//victory_state->victor 
	victory_state->deciding = true;
}

const int bw = 200;
const int bh = 80;
const Button btn_continue = {
	.dims = (Rectangle){ SCREEN_WIDTH/2 - bw - 30, 400, bw, bh},
	.color = HACKER_GREEN,
	.text = "Let it continue.\n",
	.id = 0,
};

const Button btn_end = {
	.dims = (Rectangle){ SCREEN_WIDTH/2 + 30, 400, bw, bh},
	.color = HACKER_GREEN,
	.text = "It's over.",
	.id = 0,
};

void state_victory(float dt, struct GameState *state) {
	Vector2 mouse = GetMousePosition();

	if (state->victory_state->deciding) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			if (CheckCollisionPointRec(mouse, btn_continue.dims)) {
				
				state->pong_state->points_to_win += NUM_ROUNDS;
				state->pong_state->fuck_you_timer = FUCK_YOU_DURATION;
				state->pong_state->fuck_you_size = 100;
				sprintf(state->pong_state->fuck_you_text, "FIRST TO %d", state->pong_state->points_to_win);
				
				// Go on to pick items as expected
				if (state->victory_state->victor == state->player1) {
					change_state_to_pick_items(state, state->player2->paddle);
				} else {
					change_state_to_pick_items(state, state->player1->paddle);
				}
			} else if (CheckCollisionPointRec(mouse, btn_end.dims)) {
				state->victory_state->deciding = false;
				state->victory_state->flash_timer = VICTORY_FLASH_DURATION;
			
			}
		}
	} else {
		state->victory_state->flash_timer -= dt;
		if (state->victory_state->flash_timer <= 0) {
			change_state_to_menu(state);
		}
	}
}

void draw_victory(VictoryState *victory_state) {
	
	// Darken the background

	char pn[32];
	sprintf(pn, "Player %d", victory_state->victor->paddle->id);

	if (victory_state->deciding) {
		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0,0,0,128});

		char n[128];
		sprintf(n, "Player %d has acquired the necessary amount of points\nto be determined the victor.", victory_state->victor->paddle->id);
		float w = MeasureText(n, 20);
		DrawTextCentered(n, SCREEN_WIDTH/2, 140, 20, HACKER_GREEN);
		DrawText(pn, SCREEN_WIDTH/2-w/2, 140-10, 20, victory_state->victor->paddle->color);
		DrawText("And yet the clock is still ticking.", SCREEN_WIDTH/2-w/2, 240, 20, HACKER_GREEN);
		DrawText("Show mercy and prolong the game?", SCREEN_WIDTH/2-w/2, 320, 20, HACKER_GREEN);

		Vector2 mouse = GetMousePosition();
		button_draw((Button*)&btn_continue, CheckCollisionPointRec(mouse, btn_continue.dims));
		button_draw((Button*)&btn_end, CheckCollisionPointRec(mouse, btn_end.dims));
	} else {
		unsigned int b = BG_COLOR;
		Color bg = GetColor(b);

		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg);

		if (victory_state->flash_timer > 1) {
			if ((int)(victory_state->flash_timer / 0.5) % 2 == 1) {
				DrawTextCentered(pn, SCREEN_WIDTH/2, SCREEN_HEIGHT/2-100, 100, victory_state->victor->paddle->color);
			}
			DrawTextCentered("GLORIOUS VICTOR", SCREEN_WIDTH/2, SCREEN_HEIGHT/2+100, 80, HACKER_GREEN);
		}
	}
}
