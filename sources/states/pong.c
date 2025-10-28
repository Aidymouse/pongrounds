#include "states/pong.h"
#include <stdio.h>
#include "defines.h"
#include "controls.h"
#include "structs.h"
#include "state_change.h"
#include "obj/obj.h"
#include "raylib.h"
#include "textures.h"
#include "states/states.h"

#include "Vec2.h"
#include "helper.h"

#define NUM_FY_STRINGS 12

const char* fuck_you_strings[] = {
	"VICTORY",
	"неудачник", // Renders as a bunch of question marks but it's funny
	"CALCULATED",
	"I WIN",
	"YOU LOSE",
	"ANOTHER?",
	"FAIR",
	//"失败者",
	"MORE",
	"LOSER",
	"SKILL ISSUE",
	"TASTE THE PAIN",
	"BITE ME",
};

const int fuck_you_size[] = {
	150, // Victory
	150,
	100, // Calculated
	150, // I win
	120, // You lose
	120, // Another
	150,
	150,
	150,
	100, // Skill Issue
	75, // Taste the Pain
	120, // Taste the Pain
};

void display_fuck_you(struct PongState *pong_state) {
	const char* fy_string = pong_state->fuck_you_text;
	int font_size = pong_state->fuck_you_size;
	float string_width = MeasureText(fy_string, font_size);

	Vector2 offset = { .x = 0, .y = 0 };
	if (pong_state->fuck_you_timer > FUCK_YOU_DURATION/2) {
		offset.x = randInt(-5, 5);
		offset.y = randInt(-2, 2);
		
	}
	
	DrawTextCentered(fy_string, SCREEN_WIDTH/2+offset.x, SCREEN_HEIGHT/2+offset.y, font_size, RED, TA_CENTER);
	DrawRectangleLinesEx((Rectangle){
		SCREEN_WIDTH/2-(string_width+60)/2,
		SCREEN_HEIGHT/2-(font_size+20)/2,
		string_width+60,
		font_size+20
	}, 5, RED);

	Color flash = {.r = 128, .g = 0, .b = 0, .a = 128 };

	DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, flash);
}

void init_state_pong(struct PongState *pong_state) {
	pong_state->current_round = 0;
	pong_state->num_balls = 0;
	ball_init(&(pong_state->balls[0]));
	//init_ball(&(pong_state->balls[1]));
	pong_state->ball_respawn_timer = 0;
	pong_state->score_timer = 0;
	pong_state->end_round_timer = 0;
	pong_state->num_rockets = 0;
	pong_state->num_explosions = 0;
	pong_state->num_paddles = 2;
	pong_state->fuck_you_timer = FUCK_YOU_DURATION;
	sprintf(pong_state->fuck_you_text, "FIRST TO %d", NUM_ROUNDS);
	//pong_state->fuck_you_text = "FIRST TO 10";
	pong_state->fuck_you_size = 100;
	pong_state->points_to_win = NUM_ROUNDS;
	pong_state->ball_respawn_timer = BALL_RESPAWN_DELAY;

	pong_state->num_decorations = 0;
}

void display_items(PaddleData *p, int x, int y, int dir, Texture2D *textures) {

	const int SMALL_ITEM_SIZE = 32;

	int item_type_count = 0;
	if (dir == -1) { item_type_count = 1; }
	for (int i=0; i<NUM_ITEMS; i++) {
		int item_x = x;
		int item_y = y+item_type_count*SMALL_ITEM_SIZE*dir;

		Color item_color = WHITE;
		Color text_color = HACKER_GREEN;
		if (p->items[i] <= 0) { item_color = RED; text_color = RED; }
		if (p->item_use_timers[i] > 0) { item_color = BLUE; text_color = BLUE; }
		if (p->item_cooldown_timers[i] > 0) { item_color = GRAY; text_color = GRAY; }

		if (p->items_total[i] > 0 || p->items[i] > 0) {
			DrawTexturePro(
				textures[i], 
				(Rectangle){0, 0, SMALL_ITEM_DIMS_PX, SMALL_ITEM_DIMS_PX},
				(Rectangle){item_x, item_y, SMALL_ITEM_SIZE, SMALL_ITEM_SIZE},
				(Vector2){0, 0},
				0,
				item_color
			);
			item_type_count += 1;
		}


		if (p->items_total[i] > 1 || (p->items_total[i] != 0 && p->items[i] != 1)) {
			char s[16];
			sprintf(s, "x%d", p->items[i]);
			DrawText(s, item_x, item_y, 9, text_color);
		}


		// TODO: color stolen items different
	}
}

void display_health(struct PlayerData *p, int x, int y) {
	PaddleData *paddle = p->paddle;

	int width = paddle->max_hp;

	DrawRectangleLines(x - width/2, y, width, 10, HACKER_GREEN);
	DrawRectangle(x - width/2, y, paddle->hp, 10, HACKER_GREEN);
}

void display_score(struct PlayerData *pl, int x, int y, float alpha) {
	char score[32];
	sprintf(score, "%d", pl->num_points);
	Color score_color = HACKER_GREEN;
	score_color.a = alpha;
	DrawTextCentered(score, x, y, 200, score_color, TA_CENTER);
}


void state_pong(float dt, struct GameState *state) {


	struct PongState *pong_state = state->pong_state;

	struct PlayerData *player1 = state->player1;
	struct PlayerData *player2 = state->player2;
	PaddleData *p1 = player1->paddle;
	PaddleData *p2 = player2->paddle;

	float world_top = GetScreenToWorld2D((Vector2){0, 0}, *state->camera).y;
	float world_bottom = GetScreenToWorld2D((Vector2){0, SCREEN_HEIGHT}, *state->camera).y;
	float world_left = GetScreenToWorld2D((Vector2){0, 0}, *state->camera).x;
	float world_right = GetScreenToWorld2D((Vector2){SCREEN_WIDTH, 0}, *state->camera).x;
	WorldBorders world_borders = {
		.top = world_top,
		.bottom = world_bottom,
		.left = world_left,
		.right = world_right
	};

	// Update //
	// Move balls
	for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
		struct BallData *ball = &(state->pong_state->balls[b_idx]);
		if (ball->delete_me) continue;
		ball_move(dt, ball, state);
	}

	// Update paddles
	for (int p_idx=0; p_idx < state->pong_state->num_paddles; p_idx++) {
		paddle_update(dt, &state->pong_state->paddles[p_idx], state, world_borders);
	}
	
	// Sword 
	if (p1->items[ITEM_CEREMONIAL_SWORD] > 0) { sword_swing(dt, p1); }
	if (p2->items[ITEM_CEREMONIAL_SWORD] > 0) { sword_swing(dt, p2); }

	// Rockets 
	for (int i=0; i<pong_state->num_rockets; i++) {
		RocketData *r = &pong_state->rockets[i];	
		rocket_fly(dt, r, state);
	}

	// Explosions 
	for (int i=0; i<pong_state->num_explosions; i++) {
		Explosion *e = &pong_state->explosions[i];	
		explosion_update(dt, e);
	}

	// Decorations
	for (int d_idx = 0; d_idx < pong_state->num_decorations; d_idx++) {
		Decoration *de = &(pong_state->decorations[d_idx]);
		decoration_update(dt, de);
	}

	// Clean up deleted things
	paddle_cleanup(state->pong_state);
	ball_cleanup(state->pong_state);
	rocket_cleanup(pong_state);
	explosion_cleanup(pong_state);
	decoration_cleanup(pong_state);

	/** Collisions */
	// Rockets
	for (int i=0; i<pong_state->num_rockets; i++) {
		RocketData *r = &pong_state->rockets[i];	
		rocket_check_collisions(r, world_borders, state);
	}

	// Paddles
	for (int pI = 0; pI < state->pong_state->num_paddles; pI++) {
		PaddleData *paddle = &state->pong_state->paddles[pI];
		paddle_check_collisions(paddle, state);
	}

	// Balls
	for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
		// Left and right of screen
		struct BallData *ball = &(state->pong_state->balls[b_idx]);

		ball_check_collisions(ball, state, world_borders);
	}



	/** /COLLISIONS **/		

	if (state->pong_state->fuck_you_timer > 0) {
		state->pong_state->fuck_you_timer -= dt;
	}


	// If all balls are destroyed, set the timer to respawn the ball
	bool all_destroyed = true;
	for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
		struct BallData *ball = &(state->pong_state->balls[b_idx]);
		if (ball->delete_me == false) {
			all_destroyed = false;
			break;
		}
	}
	
	if (all_destroyed && state->pong_state->ball_respawn_timer <= 0) {
			state->pong_state->ball_respawn_timer = BALL_RESPAWN_DELAY;
			int idx = randInt(0, NUM_FY_STRINGS-1);
			state->pong_state->fuck_you_timer = FUCK_YOU_DURATION;
			state->pong_state->fuck_you_size = fuck_you_size[idx];
			sprintf(state->pong_state->fuck_you_text, "%s", fuck_you_strings[idx]);
			//state->pong_state->fuck_you_text = 
	}

	// Respawn ball if timer's up
	if (pong_state->ball_respawn_timer > 0 && pong_state->end_round_timer <= 0) {
		pong_state->ball_respawn_timer -= dt;
		if (pong_state->ball_respawn_timer <= 0) {
			pong_state->num_balls = 1;
			ball_respawn(&(state->pong_state->balls[0]));
		}
	}

	if ((player1->paddle->hp <= 0 || player2->paddle->hp <= 0) && pong_state->end_round_timer <= 0) {
		pong_state->end_round_timer = BALL_RESPAWN_DELAY;
	} 

	// Detect end of round
	if (pong_state->end_round_timer > 0) {

		// If the round can't actually end, don't!
		bool round_can_end = all_destroyed 
			&& pong_state->num_rockets == 0
			&& pong_state->num_explosions == 0;

		pong_state->end_round_timer -= dt;
		if (!round_can_end) {
			pong_state->end_round_timer = BALL_RESPAWN_DELAY;
		}

		if (pong_state->end_round_timer <= 0 && round_can_end) {
			
			if (player1->paddle->hp <= 0) {
				player2->num_points += 1;
			}

			if (player2->paddle->hp <= 0) {
				player1->num_points += 1;
			}
			
			// TODO: if both players surpass the score threshold at the same time, player 1 will get to choose if they win before player 2
			if (player1->paddle->hp <= 0) {
				if (player2->num_points >= state->pong_state->points_to_win) {
					change_state_to_victory(state, player2);
				} else {
					change_state_to_pick_items(state, player1);
				}
			} else if (player2->paddle->hp <= 0) {
				if (player1->num_points >= state->pong_state->points_to_win) {
					change_state_to_victory(state, player1);
				} else {
					change_state_to_pick_items(state, player2);
				}
			} 	
		}
	}

	if (state->pong_state->score_timer > 0) {
		state->pong_state->score_timer -= dt;
	}


}

void draw_pong(struct GameState *state) {

		// UI in the background: score
		if (state->pong_state->score_timer > 0) {
			display_score(state->player1, SCREEN_WIDTH/2, SCREEN_HEIGHT*0.25, 64);
			display_score(state->player2, SCREEN_WIDTH/2, SCREEN_HEIGHT*0.75, 64);
		} 

		BeginMode2D(*state->camera);

		for (int d_idx=state->pong_state->num_decorations-1; d_idx>=0; d_idx--) {
			Decoration d = state->pong_state->decorations[d_idx];
			decoration_draw(&d);
		}

		// Paddles
		for (int p_idx=state->pong_state->num_paddles-1; p_idx>=0; p_idx--) {
			PaddleData p = state->pong_state->paddles[p_idx];
			paddle_draw(&p, false);
		}

		// Items //
		for (int i=0; i<state->pong_state->num_rockets; i++) {
			rocket_draw(&state->pong_state->rockets[i], false);
		}

		for (int i=0; i<state->pong_state->num_explosions; i++) {
			explosion_draw(&state->pong_state->explosions[i], false);
		}

		// Ball
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
			if (ball->delete_me) { continue; }
			ball_draw(ball, false);
		}

		EndMode2D();
	
		if (state->pong_state->fuck_you_timer > 0 && state->state != STATE_PICK_ITEM && state->pause_state->prior_state != STATE_PICK_ITEM) {
			display_fuck_you(state->pong_state);
		}

		// UI (health, items, score) //
		display_items(&state->pong_state->paddles[0], 0, 0, 1, tex_small_items);
		display_health(state->player1, SCREEN_WIDTH/2, 10);

		display_items(&state->pong_state->paddles[1], SCREEN_WIDTH-32, SCREEN_HEIGHT, -1, tex_small_items);
		display_health(state->player2, SCREEN_WIDTH/2, SCREEN_HEIGHT-10-10);

		

}
