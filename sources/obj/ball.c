#include "structs.h"
#include "obj.h"
#include "Vec2.h"
#include "helper.h"
#include "defines.h"

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

void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	//if (b->vel.y < 0) { b->vel.y = -BALL_INIT_SPEED; } else { b->vel.y = BALL_INIT_SPEED; }
	b->speed = BALL_INIT_SPEED;
	b->destroyed = false;
	b->radius = BALL_RADIUS;
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

	if (b->last_hit_by != 0 && b->last_hit_by->id != p->id) {
		b->speed += 20;
	}

	b->last_hit_by = p;

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


void ball_sword_hit(struct BallData *ball, struct PongState *pong_state) {
	// Split into two balls!
	// Make this ball half, duplicate self
	ball->radius = ball->radius/2;
	ball->score_damage /= 2;

	struct BallData copy = *ball;
	Vec2Rotate(copy.vel, randInt(-20, 20));
	
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
