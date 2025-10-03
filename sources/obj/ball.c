#include "structs.h"
#include "obj.h"
#include "Vec2.h"
#include "helper.h"
#include "defines.h"
#include <stdio.h>

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
	
	float speed_multiplier = 1;
	if (ball->state == BS_KNUCKLEBALL) {
		speed_multiplier *= 0.8;
	}

	struct PaddleData *paddles[2] = { state->player1->paddle, state->player2->paddle };
	for (int pi=0; pi<2; pi++) {
		struct PaddleData *paddle = paddles[pi];
		if (paddle->items[ITEM_TIME_WIZARDS_CHRONOMETER] > 0) {
			Rectangle time_influence = (Rectangle){ paddle->pos.x - CHRONOMETER_SIZE/2, paddle->pos.y - CHRONOMETER_SIZE/2, CHRONOMETER_SIZE+paddle->paddle_width, CHRONOMETER_SIZE+paddle->paddle_thickness };
			if (CheckCollisionCircleRec(ball->pos, ball->radius, time_influence)) {
				speed_multiplier *= 0.7;
				for (int t = state->player1->paddle->items[ITEM_TIME_WIZARDS_CHRONOMETER]-1; t>0; t--) {
					speed_multiplier *= 0.95;
				}
			}	
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
	b->destroyed = false;

	b->radius = BALL_RADIUS;
	b->score_damage = BALL_SCORE_DAMAGE;
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

}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent) {

	struct PaddleData *paddle = scorer->paddle;

	//scorer->score +=1;
	opponent->paddle->hp -= b->score_damage;
	//ball_respawn(b);
	b->destroyed = true;
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

void ball_draw(struct BallData *ball, bool debug) {
	DrawCircle(ball->pos.x, ball->pos.y, ball->radius, WHITE);
	if (debug) {
		// Draw line of balls knuckleball facing
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(ball->kb_dir, 50)), 3, RED); 
		Vector2 kb_desired = GetVec2FromAngle(ball->kb_desired_angle);
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(kb_desired, 50)), 5, BLUE); 
	}
}
