#include "structs.h"
#include "obj.h"
#include "Vec2.h"
#include "helper.h"
#include "defines.h"
#include <stdio.h>

void ball_init(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->radius = 6;
	b->vel.y = 1;
	b->vel.x = 0;
	b->speed = BALL_INIT_SPEED;
	b->state = BS_NORMAL;
	b->score_damage = BALL_SCORE_DAMAGE;
	b->delete_me = false;
	b->last_hit_by = 0; // equiv to NULL

	b->kb_turn_speed = NIEKRO_KB_TURN_SPEED;

	b->rs_spiked_speed_mult = 1;

	b->hp_timer = 0;

}

struct BallData *ball_spawn(struct PongState *pong_state) {

	if (pong_state->num_balls >= MAX_BALLS) { return NULL; }

	struct BallData ball;
	pong_state->balls[pong_state->num_balls] = ball;
	pong_state->num_balls += 1;

	return &pong_state->balls[pong_state->num_balls-1];
}

struct BallData *ball_clone(struct BallData *ball, struct PongState *pong_state) {

	if (pong_state->num_balls >= MAX_BALLS) { return NULL; }

	struct BallData ball_clone = *ball;
	pong_state->balls[pong_state->num_balls] = ball_clone;
	pong_state->num_balls += 1;

	return &pong_state->balls[pong_state->num_balls-1];
}

void ball_move(float dt, struct BallData *ball, struct GameState *state) {

	if (ball->state == BS_NORMAL) {

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


		ball->kb_dir_timer -= dt;

		ball->vel = ball->kb_dir;
		// IDEA: If we're far enough toward one side just commit to a loop
	}
	
	// Hydraulic Press
	if (ball->hp_timer > 0) {
		ball->hp_timer -= dt;
		if (ball->hp_timer <= 0) {
			ball->radius = ball->radius * 0.3;
			if ( ball->radius < 1 ) { ball->radius = 1; }
			ball->hp_timer = 0;

			// Split into a million pieces!!!
			for (int i=0; i<HYDRAULIC_PRESS_NUM_BALLS; i++) {
				struct BallData *new_ball = ball_clone(ball, state->pong_state);
				if (new_ball == NULL) { continue; }
				new_ball->radius = new_ball->radius * 0.3;
				if ( new_ball->radius < 1 ) { new_ball->radius = 1; }
				new_ball->vel = Vec2Rotate(new_ball->vel, randInt(-20, 20));
				new_ball->hp_timer = 0;
			}
		}
	}

	// Find speed multiplier
	float speed_multiplier = 1;
	if (ball->state == BS_KNUCKLEBALL) {
		speed_multiplier *= 0.8;
	}

	PaddleData *paddles[2] = { state->player1->paddle, state->player2->paddle };
	for (int pi=0; pi<2; pi++) {
		PaddleData *paddle = paddles[pi];
		if (paddle->items[ITEM_TIME_WIZARDS_CHRONOMETER] > 0) {
			Rectangle time_influence = paddle_get_time_influence_area(paddle);
			if (CheckCollisionCircleRec(ball->pos, ball->radius, time_influence)) {
				speed_multiplier *= paddle_get_time_power(paddle);
			}	
		}
	}

	speed_multiplier *= ball->rs_spiked_speed_mult;

	if (ball->rs_spiked_speed_mult > 1) {
		ball->rs_spiked_speed_mult -= RS_SPIKE_DECELLERATION;
		if (ball->rs_spiked_speed_mult < 1) {
			ball->rs_spiked_speed_mult = 1;
		}
	}

	ball->pos = Vec2Add(ball->pos, Vec2MultScalar(ball->vel, ball->speed*speed_multiplier*dt));
}

void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	//if (b->vel.y < 0) { b->vel.y = -BALL_INIT_SPEED; } else { b->vel.y = BALL_INIT_SPEED; }
	b->speed = BALL_INIT_SPEED;
	b->delete_me = false;

	b->radius = BALL_RADIUS;
	b->score_damage = BALL_SCORE_DAMAGE;
	b->rs_spiked_speed_mult = 1;
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

void ball_paddle_hit(struct BallData *b, PaddleData *p) {

	// Get away from the paddle!
	Vector2 paddle_center;
	paddle_center.x = p->pos.x + p->paddle_width/2;
	paddle_center.y = p->pos.y + p->paddle_thickness/2;

	Vector2 away = Vec2Sub(b->pos, paddle_center);	
	Vector2 new_dir; //= Vec2Rotate(away, randInt(-20, 20));

	if (b->pos.x > p->pos.x && b->pos.x < p->pos.x + p->paddle_width) {
		float dir_angle = Vec2GetAngle(new_dir);
		if (b->pos.y > paddle_center.y) {
			new_dir = GetVec2FromAngle(randInt(10, 170));
		} else {
			new_dir = GetVec2FromAngle(randInt(190, 350));
		}
	}

	b->vel = Vec2Normalize(new_dir);

	if (b->last_hit_by != 0 && b->last_hit_by->id != p->id) {
		b->speed += 20;
	}

	b->last_hit_by = p;

	//TODO: push the ball OUT of the paddle back the way it came first. We could probably be lazy and make it a linear push to the closest side but thats risky if the ball is going very fast

	// Knuckle ball
	if (b->state == BS_KNUCKLEBALL) {
		b->state = BS_NORMAL;
	}

	if (p->items[ITEM_NIEKRO_CARD] > 0) {
		int knuckleball_chance = 20 + 5 * p->items[ITEM_NIEKRO_CARD]-1;
		if (randInt(1, 100) < knuckleball_chance) {
			//printf("Knuckleball!\n");
			p->item_use_timers[ITEM_NIEKRO_CARD] = ITEM_USE_BUMP_TIME;
			b->state = BS_KNUCKLEBALL;
			b->kb_dir = Vec2Normalize(b->vel);
			b->kb_desired_angle = Vec2GetAngle(b->kb_dir);
			b->kb_dir_timer = 0.3;
		}
	}

	// Russian secrets spiking
	b->rs_spiked_speed_mult = 1;
	if (p->items[ITEM_RUSSIAN_SECRETS] > 0) {
		Rectangle rs_area = paddle_get_russian_secrets_rect(p);
		if (CheckCollisionCircleRec(b->pos, b->radius, rs_area)) {
			b->rs_spiked_speed_mult = RUSSIAN_SECRETS_SPEED_MULT + p->items[ITEM_RUSSIAN_SECRETS]-1 * RUSSIAN_SECRETS_SPEED_ADDITIONAL;
			printf("Spiked! %f\n", b->rs_spiked_speed_mult);
			b->vel = p->facing;
			p->item_use_timers[ITEM_RUSSIAN_SECRETS] = ITEM_USE_BUMP_TIME;
		}
	}

	// Hydraulic Press
	if (p->items[ITEM_HYDRAULIC_PRESS] > 0 && b->hp_timer == 0) {
		int press_chance = p->items[ITEM_HYDRAULIC_PRESS] * HYDRAULIC_PRESS_CHANCE + p->items[ITEM_HYDRAULIC_PRESS]-1 * HYDRAULIC_PRESS_CHANCE_ADDITIONAL;
		if (randInt(1, 100) < press_chance) {
			b->hp_timer = HYDRAULIC_PRESS_TIMER;
			p->item_use_timers[ITEM_HYDRAULIC_PRESS] = ITEM_USE_BUMP_TIME;
		}
	}
	

}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent) {

	PaddleData *paddle = scorer->paddle;

	//scorer->score +=1;
	opponent->paddle->hp -= b->score_damage;
	//ball_respawn(b);
	b->delete_me = true;
}


void ball_sword_hit(struct BallData *ball, struct PongState *pong_state) {
	// Split into two balls!
	// Make this ball half, duplicate self
	ball->radius = ball->radius/2;
	if (ball->radius < 1) { ball->radius = 1; }
	ball->score_damage /= 2;
	if (ball->score_damage < 5) { ball->score_damage = 5; }

	struct BallData copy = *ball;
	copy.vel = Vec2Rotate(copy.vel, randInt(-20, 20));
	
	pong_state->balls[pong_state->num_balls] = copy;
	pong_state->num_balls += 1;
}


void ball_check_collisions(struct BallData *ball, struct GameState *state, WorldBorders world_borders) {
	if (ball->delete_me) { return; }

	// Explosions
	for (int p=0; p<state->pong_state->num_explosions; p++) {
		Explosion e = state->pong_state->explosions[p];
		if (CheckCollisionCircles(ball->pos, ball->radius, e.pos, e.radius)) {
			ball->delete_me = true;
			return;
		}
		
	}

	if (ball->pos.x - ball->radius <= world_borders.left) {
		ball->pos.x = world_borders.left + ball->radius;
		ball_reflect_wall(ball);
	} else if (ball->pos.x + ball->radius >= world_borders.right) {
		ball->pos.x = world_borders.right - ball->radius;
		ball_reflect_wall(ball);
	}

	// Collisions per paddle
	PaddleData *paddles[2] = { state->player1->paddle, state->player2->paddle };
	for (int p=0; p<2; p++) {
		PaddleData *paddle = paddles[p];
		// Paddle Ball Collisions
		struct Rectangle pRect = {paddle->pos.x, paddle->pos.y, paddle->paddle_width, paddle->paddle_thickness};
		if (CheckCollisionCircleRec(ball->pos, ball->radius, pRect)) {
			ball_paddle_hit(ball, paddle);
		}

		// Sword
		if (paddle->items[ITEM_CEREMONIAL_SWORD] > 0 && paddle->sword_timer > 0) {
			if (CheckCollisionCircleRec(ball->pos, ball->radius, sword_get_hitbox(paddle))) {
				ball_sword_hit(ball, state->pong_state);
				paddle->sword_timer = 0;
			}
		}
	} // /paddle collisions



	// Expired panadol edge of screen collision
	PaddleData *p1 = state->player1->paddle;
	PaddleData *p2 = state->player2->paddle;
	// TODO: make it get the item from the paddle currently on that side
	if (ball->pos.y - ball->radius < world_borders.top && p1->items[ITEM_EXPIRED_PANADOL] > 0) {
		p1->items[ITEM_EXPIRED_PANADOL] -= 1;
		p1->item_use_timers[ITEM_EXPIRED_PANADOL] = ITEM_USE_BUMP_TIME;
		ball->vel.y = -ball->vel.y; // TODO: better reflection fn
	} else if (ball->pos.y + ball->radius > world_borders.bottom && p2->items[ITEM_EXPIRED_PANADOL] > 0) {
		p2->items[ITEM_EXPIRED_PANADOL] -= 1;
		p2->item_use_timers[ITEM_EXPIRED_PANADOL] = ITEM_USE_BUMP_TIME;
		ball->vel.y = -ball->vel.y; // TODO: better reflection fn
	}

	// Scoring
	if (ball->pos.y - ball->radius > world_borders.bottom+10) {
		ball_score_hit(ball, state->player1, state->player2);
	} else if (ball->pos.y + ball->radius + 10 < world_borders.top) {
		ball_score_hit(ball, state->player2, state->player1);
	}
}

void ball_draw(struct BallData *ball, bool debug) {
	DrawCircle(ball->pos.x, ball->pos.y, ball->radius, WHITE);
	if (debug) {
		// Draw line of balls knuckleball facing
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(ball->kb_dir, 50)), 3, RED); 
		Vector2 kb_desired = GetVec2FromAngle(ball->kb_desired_angle);
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(kb_desired, 50)), 5, BLUE); 
	}
}

void ball_cleanup(struct PongState *state) {
	for (int b=0; b<state->num_balls; b++) {
		if (state->num_balls == 1) { return; }

		if (state->balls[b].delete_me) {
			state->balls[b] = state->balls[state->num_balls-1];
			state->num_balls -= 1;
			b -= 1;
		}
	}
}
