#include "pong.h"

#include <stdio.h>
#include "defines.h"
#include "structs.h"
#include "state_change.h"

#include "Vec2.h"
#include "helper.h"

// Called on point score
void refresh_paddle(struct PaddleData *p, struct PaddleData *opponent) {

	p->hp = p->max_hp;

	for (int i=0; i<16; i++) {
		p->items[i] = p->items_total[i];
	}

	p->paddle_width = DEFAULT_PADDLE_WIDTH + 20 * (p->items[ITEM_HYPERGONADISM]) - 20 * (opponent->items[ITEM_CHASTITY_CAGE]);
}

void display_items(struct PaddleData *p, int x, int y) {
	for (int i=0; i<16; i++) {
		char s[16];
		//sprintf(p1score, "%d", player1.score);
		sprintf(s, "%d", p->items[i]);
		DrawText(s, x, y+i*10, 10, WHITE);
		
	}
}

/*
void display_round_score(struct PlayerData *p, int x, int y) {
	for (int i=0; i<POINTS_PER_ROUND; i++) {
		if (p->score >= (i+1)) {
			DrawCircle(x + i*15, y, 5, WHITE);	
		} else {
			DrawCircleLines(x+i*15, y, 5, WHITE);	
		}
	}
}
*/

void display_health(struct PlayerData *p, int x, int y) {
	struct PaddleData *paddle = p->paddle;

	int width = paddle->max_hp;

	DrawRectangleLines(x - width/2, y, width, 10, WHITE);
	DrawRectangle(x - width/2, y, paddle->hp, 10, WHITE);
}

// BALL //
void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	//if (b->vel.y < 0) { b->vel.y = -BALL_INIT_SPEED; } else { b->vel.y = BALL_INIT_SPEED; }
	b->speed = BALL_INIT_SPEED;
	b->destroyed = false;
}


void ball_reflect_wall(struct BallData *b) {
	b->vel.x = -b->vel.x;

	if (b->state == BS_KNUCKLEBALL) {
		b->kb_dir.x = -b->kb_dir.x;

		Vector2 d = GetVec2FromAngle(b->kb_desired_angle);
		d.x = -d.x;
		b->kb_desired_angle = Vec2GetAngle(d);
		
	}
}

void ball_paddle_hit(struct BallData *b, struct PaddleData *p) {


	// Get away from the paddle!
	Vector2 paddle_center;
	paddle_center.x = p->pos.x + p->paddle_width/2;
	paddle_center.y = p->pos.y + p->paddle_thickness/2;

	Vector2 away = Vec2Sub(b->pos, paddle_center);	
	Vector2 new_dir = Vec2Rotate(away, randInt(-20, 20));
	b->vel = Vec2Normalize(new_dir);

	b->speed += 20;

	//TODO: push the ball OUT of the paddle back the way it came first. We could probably be lazy and make it a linear push to the closest side but thats risky if the ball is going very fast

	if (b->state == BS_KNUCKLEBALL) {
		b->state = BS_NORMAL;
	}

	if (p->items[ITEM_NIEKRO_CARD] > 0) {
		int knuckleball_chance = 20 + p->items[ITEM_NIEKRO_CARD]-1 * 5; if (randInt(1, 100) < knuckleball_chance) {
			b->state = BS_KNUCKLEBALL;
			b->kb_dir = Vec2Normalize(b->vel);
			b->kb_desired_angle = Vec2GetAngle(b->kb_dir);
			b->kb_dir_timer = 0.3;
		}
	}

}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent) {

	struct PaddleData *paddle = scorer->paddle;

	//scorer->score +=1;
	opponent->paddle->hp -= b->score_damage;
	//ball_respawn(b);
	b->destroyed = true;
}

void ball_move(float dt, struct BallData *ball) {


	if (ball->state == BS_NORMAL) {
		ball->pos = Vec2Add(ball->pos, Vec2MultScalar(ball->vel, ball->speed*dt));
	} else if (ball->state == BS_KNUCKLEBALL) {


		if (ball->kb_dir_timer <= 0) {
			// Pick new dir (weighted)
			// TODO: weight based on desired side
			int delta = randInt(-60, 60);
			ball->kb_desired_angle += delta;

			ball->kb_dir_timer = (float)randInt(2, 8) / 10;
		}

		float dist_to_desired = get_angle_distance(Vec2GetAngle(ball->kb_dir), ball->kb_desired_angle);

		// Rotate toward desired angle
		if (dist_to_desired <= 0) {
			ball->kb_dir = Vec2Rotate(ball->kb_dir, -ball->kb_turn_speed * dt);
		} else {
			ball->kb_dir = Vec2Rotate(ball->kb_dir, ball->kb_turn_speed * dt);
		}

		ball->pos = Vec2Add(ball->pos, Vec2MultScalar(ball->vel, ball->speed*0.8*dt));

		ball->kb_dir_timer -= dt;

		// IDEA: If we're far enough toward one side just commit to a loop
	}
}

void display_ball(struct BallData *ball, bool debug) {
	DrawCircle(ball->pos.x, ball->pos.y, ball->radius, WHITE);
	if (debug) {
		// Draw line of balls knuckleball facing
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(ball->kb_dir, 50)), 3, RED); 
		Vector2 kb_desired = GetVec2FromAngle(ball->kb_desired_angle);
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(kb_desired, 50)), 5, BLUE); 
	}
}

// PADDLE //
void paddle_move(float dt, struct PaddleData *p, struct PaddleControls controls) {

		if (p->vel.x > 300) {
			p->vel.x -= 500;

		} else if (p->vel.x < -300) {
			p->vel.x += 500;

		} else if (IsKeyDown(controls.left)) {
				p->vel.x = -300;

				if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyDown(controls.dash)) {
					p->vel.x = -3000 + (p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1)*-100;
				}
		} else if (IsKeyDown(controls.right)) {
				p->vel.x = 300;

				if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyDown(controls.dash)) {
					p->vel.x = 3000 + (p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1)*100;
				}
		} else {
			p->vel.x = 0;
		}

		p->pos = Vec2Add(p->pos, Vec2MultScalar(p->vel, dt));

		if (p->pos.x + p->paddle_width > SCREEN_WIDTH) {
			p->pos.x = SCREEN_WIDTH - p->paddle_width;
		} else if (p->pos.x < 0) {
			p->pos.x = 0;
		}
}

void state_pong(float dt, struct GameState *state) {

		struct PongState *pong_state = state->pong_state;


		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;
		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		// Update //
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
			if (ball->destroyed) continue;
			ball_move(dt, ball);
		}

		/** Player control **/
		struct PaddleControls p1_controls = { P1_LEFT_KEY, P1_RIGHT_KEY, P1_DASH_KEY };
		paddle_move(dt, p1, p1_controls);
		struct PaddleControls p2_controls = { P2_LEFT_KEY, P2_RIGHT_KEY, P2_DASH_KEY };
		paddle_move(dt, p2, p2_controls);

		/** Collisions **/
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			// Left and right of screen
			struct BallData *ball = &(state->pong_state->balls[b_idx]);

			if (ball->destroyed) { continue; }

			if (ball->pos.x - ball->radius <= 0) {
				ball->pos.x = ball->radius;
				ball_reflect_wall(ball);
			} else if (ball->pos.x + ball->radius >= SCREEN_WIDTH) {
				ball->pos.x = SCREEN_WIDTH - ball->radius;
				ball_reflect_wall(ball);
			}

			// Paddle Ball Collisions
			struct Rectangle p1Rect = {p1->pos.x, p1->pos.y, p1->paddle_width, p1->paddle_thickness};
			if (CheckCollisionCircleRec(ball->pos, ball->radius, p1Rect)) {
				ball_paddle_hit(ball, p1);
			}
			struct Rectangle p2Rect = {p2->pos.x, p2->pos.y, p2->paddle_width, p2->paddle_thickness};
			if (CheckCollisionCircleRec(ball->pos, ball->radius, p2Rect)) {
				ball_paddle_hit(ball, p2);
			}
		
			// Expired panadol edge of screen collision
			if (ball->pos.y - ball->radius < 0 && p1->items[ITEM_EXPIRED_PANADOL] > 0) {
				p1->items[ITEM_EXPIRED_PANADOL] -= 1;
				ball->vel.y = -ball->vel.y; // TODO: better reflection fn
			} else if (ball->pos.y + ball->radius > SCREEN_HEIGHT && p2->items[ITEM_EXPIRED_PANADOL] > 0) {
				p2->items[ITEM_EXPIRED_PANADOL] -= 1;
				ball->vel.y = -ball->vel.y; // TODO: better reflection fn
			}
		
			// Scoring
			if (ball->pos.y - ball->radius > SCREEN_HEIGHT+10) {
				ball_score_hit(ball, player1, player2);
			} else if (ball->pos.y + ball->radius + 10 < 0) {
				ball_score_hit(ball, player2, player1);
			}
		}


		// If all balls are destroyed, set the timer to respawn the ball
		bool all_destroyed = true;
		for (int b_idx = 0; b_idx < pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(pong_state->balls[b_idx]);
			if (ball->destroyed == false) {
				all_destroyed = false;
				break;
			}
		}

		if (all_destroyed && pong_state->ball_respawn_timer <= 0) {
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

		// Draw Paddle One
		DrawRectangle(p1->pos.x, p1->pos.y, p1->paddle_width, p1->paddle_thickness, p1->color);

		// Draw Paddle Two
		DrawRectangle(p2->pos.x, p2->pos.y, p2->paddle_width, p2->paddle_thickness, p2->color);
	
		// BALL
		for (int b_idx = 0; b_idx < state->pong_state->num_balls; b_idx++) {
			struct BallData *ball = &(state->pong_state->balls[b_idx]);
			display_ball(ball, false);
		}

		// Score
		display_items(p1, 20, 20);
		display_health(player1, SCREEN_WIDTH/2, 10);

		display_items(p2, SCREEN_WIDTH-20-10, 20);
		display_health(player2, SCREEN_WIDTH/2, SCREEN_HEIGHT-10-10);

}
