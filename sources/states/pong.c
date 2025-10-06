#include "pong.h"
#include <stdio.h>
#include "defines.h"
#include "controls.h"
#include "structs.h"
#include "state_change.h"
#include "obj/obj.h"
#include "raylib.h"
#include "textures.h"

#include "Vec2.h"
#include "helper.h"

// Called on point score

void display_items(struct PaddleData *p, int x, int y, int dir, Texture2D *textures) {
	/*
	for (int i=0; i<16; i++) {
		char s[16];
		//sprintf(p1score, "%d", player1.score);
		sprintf(s, "%d", p->items[i]);
		DrawText(s, x, y+i*10, 10, WHITE);
		
	}
	*/

	int item_type_count = 0;
	if (dir == -1) { item_type_count = 1; }
	for (int i=0; i<NUM_ITEMS; i++) {
		int item_x = x;
		int item_y = y+item_type_count*50*dir;

		Color item_color = WHITE;
		if (p->items[i] <= 0) { item_color = RED; }
		if (p->item_use_timers[i] > 0) { item_color = BLUE; }

		if (p->items_total[i] > 0) {
			DrawTexturePro(
				textures[i], 
				(Rectangle){0, 0, SMALL_ITEM_DIMS_PX, SMALL_ITEM_DIMS_PX},
				(Rectangle){item_x, item_y, 50, 50},
				(Vector2){0, 0},
				0,
				item_color
			);
			item_type_count += 1;
		}

		if (p->items_total[i] > 1 || (p->items_total[i] != 0 && p->items[i] != 1)) {
			char s[16];
			sprintf(s, "x%d", p->items[i]);
			DrawText(s, item_x, item_y, 9, item_color);
		}
	}
}

void display_health(struct PlayerData *p, int x, int y) {
	struct PaddleData *paddle = p->paddle;

	int width = paddle->max_hp;

	DrawRectangleLines(x - width/2, y, width, 10, WHITE);
	DrawRectangle(x - width/2, y, paddle->hp, 10, WHITE);
}


void state_pong(float dt, struct GameState *state) {

		struct PongState *pong_state = state->pong_state;

		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;
		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

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
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
			if (ball->destroyed) continue;
			ball_move(dt, ball, state);
		}

		paddle_update(dt, p1);
		paddle_update(dt, p2);

		/** Player control **/
		struct PaddleControls p1_controls = { P1_LEFT_KEY, P1_RIGHT_KEY, P1_DASH_KEY, P1_UP_KEY, P1_DOWN_KEY };
		paddle_move(dt, p1, p1_controls, state);
		struct PaddleControls p2_controls = { P2_LEFT_KEY, P2_RIGHT_KEY, P2_DASH_KEY, P2_UP_KEY, P2_DOWN_KEY };
		paddle_move(dt, p2, p2_controls, state);

		if (IsKeyPressed(P1_ITEM_KEY)) {
			paddle_activate_items(dt, p1, state->pong_state);
		}
		if (IsKeyPressed(P2_ITEM_KEY)) {
			paddle_activate_items(dt, p2, state->pong_state);
		}
		
		// Sword //
		if (p1->items[ITEM_CEREMONIAL_SWORD] > 0) { sword_swing(dt, p1); }
		if (p2->items[ITEM_CEREMONIAL_SWORD] > 0) { sword_swing(dt, p2); }

		// Rockets //
		rocket_cleanup(pong_state);

		for (int i=0; i<pong_state->num_rockets; i++) {
			RocketData *r = &pong_state->rockets[i];	
			rocket_fly(dt, r);
		}

		// Explosions //
		explosion_cleanup(pong_state);
		for (int i=0; i<pong_state->num_explosions; i++) {
			Explosion *e = &pong_state->explosions[i];	
			explosion_update(dt, e);
		}



		/** Collisions **/
		// Rockets
		for (int i=0; i<pong_state->num_rockets; i++) {
			RocketData *r = &pong_state->rockets[i];	
			rocket_check_collisions(r, world_borders, state);
		}

		// Balls
		struct PaddleData *paddles[2] = { p1, p2 };
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			// Left and right of screen
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
	
			ball_check_collisions(ball, state, world_borders);

		}

		/** /COLLISIONS **/		

		// If all balls are destroyed, set the timer to respawn the ball
		bool all_destroyed = true;
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
			if (ball->destroyed == false) {
				all_destroyed = false;
				break;
			}
		}

		if (all_destroyed && state->pong_state->ball_respawn_timer <= 0) {
			state->pong_state->ball_respawn_timer = BALL_RESPAWN_DELAY;
		}
		
		// Respawn ball if timer's up
		if (pong_state->ball_respawn_timer > 0) {
			pong_state->ball_respawn_timer -= dt;
			if (pong_state->ball_respawn_timer <= 0) {
				pong_state->num_balls = 1;
				ball_respawn(&(state->pong_state->balls[0]));
				
				// Detect end of round (after respawn delay for game feel)
				if (player1->paddle->hp <= 0) {
					change_state_to_pick_items(state, player1->paddle);
				} else if (player2->paddle->hp <= 0) {
					change_state_to_pick_items(state, player2->paddle);
				}

			}
		}

}

void draw_pong(struct GameState *state) {

		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;

		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		BeginMode2D(*state->camera);

		// PADDLES
		struct PaddleData *paddles[2] = { p1, p2 };
		for (int p_idx=0; p_idx<2; p_idx++) {
			struct PaddleData *p = paddles[p_idx];
			DrawRectangle(p->pos.x, p->pos.y, p->paddle_width, p->paddle_thickness, p->color);
			if (p->items[ITEM_CEREMONIAL_SWORD] > 0) {
				sword_draw(p1, false);
			}
		}
	
		// BALL
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
			ball_draw(ball, false);
		}

		// Items //
		for (int i=0; i<state->pong_state->num_rockets; i++) {
			rocket_draw(&state->pong_state->rockets[i], true);
		}

		for (int i=0; i<state->pong_state->num_explosions; i++) {
			explosion_draw(&state->pong_state->explosions[i], true);
		}

		EndMode2D();

		// UI (health, items) //
		display_items(p1, 0, 0, 1, tex_small_items);
		display_health(player1, SCREEN_WIDTH/2, 10);

		display_items(p2, SCREEN_WIDTH-50, SCREEN_HEIGHT, -1, tex_small_items);
		display_health(player2, SCREEN_WIDTH/2, SCREEN_HEIGHT-10-10);

}
