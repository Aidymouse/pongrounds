#include "structs.h"
#include "obj/obj.h"
#include "Vec2.h"
#include "helper.h"
#include "defines.h"
#include <stdio.h>
#include "audio.h"

/**
 */
void ball_init(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->radius = 6;
	b->vel.y = 1;
	if (randInt(1, 2) == 2) { b->vel.y = -1; }
	b->vel.x = 0;
	b->speed = BALL_INIT_SPEED;
	b->state = BS_NORMAL;
	b->score_damage = BALL_SCORE_DAMAGE;
	b->delete_me = false;
	b->last_hit_by = 0; // equiv to NULL
	b->color = HACKER_GREEN;

	b->kb_turn_speed = NIEKRO_KB_TURN_SPEED;

	b->rs_spiked_speed_mult = 1;

	b->hp_timer = 0;
	b->ym_gravity_turn_speed = 0;

	b->mm_speed_bonus = 0;
	b->mutant_moused = false;

}

/** */
struct BallData *ball_spawn(struct PongState *pong_state) {

	if (pong_state->num_balls >= MAX_BALLS) { return NULL; }

	struct BallData ball;
	pong_state->balls[pong_state->num_balls] = ball;
	pong_state->num_balls += 1;

	return &pong_state->balls[pong_state->num_balls-1];
}

/** */
struct BallData *ball_clone(struct BallData *ball, struct PongState *pong_state) {

	if (pong_state->num_balls >= MAX_BALLS) { return NULL; }

	struct BallData ball_clone = *ball;
	pong_state->balls[pong_state->num_balls] = ball_clone;
	pong_state->num_balls += 1;

	return &pong_state->balls[pong_state->num_balls-1];
}


// Game is sloooow if the ball ends up at a shallow angle. We limit it to no more than 
// Note: Exceptions are hydraulic press balls or sword cut balls. I think if there's more than one it's okay for it to take ages
void limit_ball_angle(BallData *ball) {
	const float MAX_FROM_CENTER = 60;

	float allowed_upper_l = -90 - MAX_FROM_CENTER;
	float allowed_upper_r = -90 + MAX_FROM_CENTER;

	float allowed_lower_l = 90 + MAX_FROM_CENTER;
	float allowed_lower_r = 90 - MAX_FROM_CENTER;

	float ball_angle = Vec2GetAngle(ball->vel);

	if (angle_is_between_two_others(ball_angle, allowed_upper_r, allowed_lower_r)) {
		float dist_up = get_angle_distance(ball_angle, allowed_upper_r);
		float dist_down = get_angle_distance(ball_angle, allowed_lower_r);
		if (dist_up < dist_down) {
			ball->vel = Vec2Rotate(ball->vel, dist_up);
		} else {
			ball->vel = Vec2Rotate(ball->vel, dist_up);
		}
	} else if (angle_is_between_two_others(ball_angle, allowed_upper_l, allowed_lower_l)) {
		float dist_up = get_angle_distance(ball_angle, allowed_upper_l);
		float dist_down = get_angle_distance(ball_angle, allowed_lower_l);
		if (dist_up < dist_down) {
			ball->vel = Vec2Rotate(ball->vel, dist_up);
		} else {
			ball->vel = Vec2Rotate(ball->vel, dist_up);
		}
	}
}

/**
 */
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
				new_ball->radius = new_ball->radius * 0.5;
				if ( new_ball->radius < 1 ) { new_ball->radius = 1; }
				new_ball->score_damage = new_ball->score_damage / HYDRAULIC_PRESS_NUM_BALLS + 1;
				if (new_ball->score_damage < 1) { new_ball->score_damage = 1; }

				new_ball->vel = Vec2Rotate(new_ball->vel, randInt(-20, 20));
				new_ball->hp_timer = 0;
			}
			
			// TODO: get a more distinct sound
			play_sfx(state->music_mind, SFX_HYDRAULIC_PRESS_BALL);
		}
	}

	
	// Mutant mouse speed accrual
	if (ball->mutant_moused) {
		for (int pI = 0; pI < 2; pI++) {
			PaddleData *paddle = &state->pong_state->paddles[pI];

			if (paddle != ball->last_hit_by) { continue; }

			if (paddle->items[ITEM_MUTANT_MOUSE] > 0) {
				if ((ball->vel.y > 0 && paddle->facing.y > 0) ||
					(ball->vel.y < 0 && paddle->facing.y < 0)
				) {
					ball->mm_speed_bonus += MUTANT_MOUSE_ACCEL;
				} 	
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

	// Yo Momma gravity angle change
	if (ball->ym_gravity_turn_speed > 0) {
		gravitate_towards(dt, ball->pos, &ball->vel, ball->ym_gravity_center, ball->ym_gravity_turn_speed * speed_multiplier);
	}

	ball->pos = Vec2Add(ball->pos, Vec2MultScalar(ball->vel, (ball->speed+ball->mm_speed_bonus)*speed_multiplier*dt));
}

void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	//if (b->vel.y < 0) { b->vel.y = -BALL_INIT_SPEED; } else { b->vel.y = BALL_INIT_SPEED; }
	b->speed = BALL_INIT_SPEED;
	b->delete_me = false;
	b->color = HACKER_GREEN;

	b->radius = BALL_RADIUS;
	b->score_damage = BALL_SCORE_DAMAGE;
	b->rs_spiked_speed_mult = 1;

	b->ym_gravity_turn_speed = 0;

	b->mm_speed_bonus = 0;
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

/** Takes the normal from the plane to reflect on (a.k.a the base dir of the balls new random angle) */
void ball_reflect(BallData *b, Vector2 norm) {
		

		// Find new dir
		Vector2 new_dir = Vec2Rotate(norm, randInt(-60, 60));

		b->vel = new_dir;
	
}

void ball_paddle_hit(struct BallData *b, PaddleData *p, GameState *game_state) {
	
	/** Push ball out of the paddle **/
	// Four vecs to the paddles corners determine what the ball is bouncing off of
	Vector2 center = paddle_center(p);

	Vector2 adjusted_pos = b->pos;

	// Determine which face of the 
	Vector2 facing_l = Vec2Rotate(p->facing, -90);
	Vector2 facing_r = Vec2Rotate(p->facing, 90);

	// The foci are at the point in the paddle you get if you follow the corners 45 degrees inwards
	// ---------------------
	// |\                 /|
	// | >---------------< |
	// |/                 \|
	// ---------------------
	Vector2 focus_l = Vec2Add(center, Vec2MultScalar(facing_l, (p->paddle_width/2) - p->paddle_thickness));
	Vector2 focus_r = Vec2Add(center, Vec2MultScalar(facing_r, (p->paddle_width/2) - p->paddle_thickness));

	float ang_l_front_left = Vec2GetAngle(facing_l) + 45;
	float ang_l_back_left = Vec2GetAngle(facing_l)  - 45;


	float ang_r_front_right = Vec2GetAngle(Vec2Rotate(facing_r, -45)); float ang_r_back_right = Vec2GetAngle(Vec2Rotate(facing_r, 45));

	float ang_ball_l = Vec2GetAngle(Vec2Sub(b->pos, focus_l));
	float ang_ball_r = Vec2GetAngle(Vec2Sub(b->pos, focus_r));

	float ang_ball_center = Vec2GetAngle(Vec2Sub(b->pos, center));
	float ang_facing = Vec2GetAngle(p->facing);
	float facing_dist = get_angle_distance(ang_ball_center, ang_facing);

	int hit_face = 0;

	if (angle_is_between_two_others(ang_ball_l, ang_l_front_left, ang_l_back_left)) {
		hit_face = 3;
	} else if (angle_is_between_two_others(ang_ball_r, ang_r_front_right, ang_r_back_right)) {
		hit_face = 4;
	} else if (fabsf(facing_dist) > 90) {
		hit_face = 2;
	} else {
		hit_face = 1;
	}
	
	


	if (hit_face == 1 || hit_face == 2) {

		// Adjust ball to by just barely touching the paddle
		Vector2 base_dir = p->facing;

		if (hit_face == 2) {
			base_dir = Vec2MultScalar(base_dir, -1);
		}

		adjusted_pos.y = center.y + Vec2MultScalar(base_dir, b->radius+p->paddle_thickness/2).y;

		ball_reflect(b, base_dir);

	} else if (hit_face == 3) { // LEFT
		adjusted_pos.x = center.x + Vec2MultScalar(facing_l, b->radius+p->paddle_width/2).x;

		Vector2 new_dir = Vec2Rotate(facing_l, randInt(-30, 0));
		b->vel = new_dir;

		if (b->speed < p->speed.x) { b->speed = p->speed.x; }

	} else if (hit_face == 4) { // RIGHT
		adjusted_pos.x = center.x + Vec2MultScalar(facing_r, b->radius+p->paddle_width/2).x;

		Vector2 new_dir = Vec2Rotate(facing_r, randInt(0, 30));
		b->vel = new_dir;

		if (b->speed < p->speed.x) { b->speed = p->speed.x; }
	}


	if (b->last_hit_by != 0 && b->last_hit_by->id != p->id) {
		b->speed += 20;
	}

	b->last_hit_by = p;


	// Knuckle ball
	if (b->state == BS_KNUCKLEBALL) {
		b->state = BS_NORMAL;
	}

	if (p->items[ITEM_NIEKRO_CARD] > 0) {
		int knuckleball_chance = p->items[ITEM_NIEKRO_CARD] * NIEKRO_CHANCE; 
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
	// This code is fine to stay here because we don't actually push the ball until the end
	b->rs_spiked_speed_mult = 1;
	if (p->items[ITEM_RUSSIAN_SECRETS] > 0) {
		Rectangle rs_area = paddle_get_russian_secrets_rect(p);
		if (CheckCollisionCircleRec(b->pos, b->radius, rs_area)) {
			b->rs_spiked_speed_mult = RUSSIAN_SECRETS_SPEED_MULT + p->items[ITEM_RUSSIAN_SECRETS]-1 * RUSSIAN_SECRETS_SPEED_ADDITIONAL;
			b->vel = p->facing;
			p->item_use_timers[ITEM_RUSSIAN_SECRETS] = ITEM_USE_BUMP_TIME;
			
			game_state->screenshake_amplitude += RUSSIAN_SECRETS_SHAKE_AMPL;
			game_state->screenshake_timer += RUSSIAN_SECRETS_SHAKE_TIME;
		}
	}

	// Hydraulic Press
	if (p->items[ITEM_HYDRAULIC_PRESS] > 0 && b->hp_timer == 0) {
		int press_chance = p->items[ITEM_HYDRAULIC_PRESS] * HYDRAULIC_PRESS_CHANCE;
		if (randInt(1, 100) <= press_chance) {
			b->hp_timer = HYDRAULIC_PRESS_TIMER;
			p->item_use_timers[ITEM_HYDRAULIC_PRESS] = HYDRAULIC_PRESS_TIMER;
		}
	}

	// Cloning vat - lose some clone health
	if (p->brain == PB_CLONE) {
		p->cv_health -= 1;
	}

	// Snotch
	if (p->items[ITEM_SNOTCH] > 0) {
		int snotch_chance = SNOTCH_CHANCE * p->items[ITEM_SNOTCH];
		if (randInt(1, 100) <= snotch_chance) {
			b->score_damage *= 2;
			play_sfx(game_state->music_mind, SFX_BALL_DAMAGE_DOUBLED);
			p->item_use_timers[ITEM_SNOTCH] = ITEM_USE_BUMP_TIME;
			if (b->color.b - 60 <= 0) { b->color.b = 0; } else { b->color.b -= 60; }
			if (b->color.g - 60 <= 0) { b->color.g = 0; } else { b->color.g -= 60; }
			play_sfx(game_state->music_mind, SFX_BALL_DAMAGE_DOUBLED);
		}
	}

	// Mutant Mouse
	b->mm_speed_bonus = 0;
	b->mutant_moused = false;
	if (p->items[ITEM_MUTANT_MOUSE] > 0) {
		int mm_chance = p->items[ITEM_MUTANT_MOUSE] * MUTANT_MOUSE_CHANCE;
		if (randInt(0, 100) < mm_chance) {
			b->mutant_moused = true;
			p->item_use_timers[ITEM_MUTANT_MOUSE] = ITEM_USE_BUMP_TIME;
		}
	}

	/** Apply position adjustment from earlier detection */
	b->pos = adjusted_pos;

	limit_ball_angle(b);

	// Play sound
	play_sfx(game_state->music_mind, SFX_PADDLE_HIT);

}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent) {

	PaddleData *paddle = scorer->paddle;

	//scorer->score +=1;
	opponent->paddle->hp -= b->score_damage;
	//ball_respawn(b);
	b->delete_me = true;
}


void ball_sword_hit(struct BallData *ball, GameState *game_state, PaddleData *wielder) {
	// Split into two balls!
	PongState *pong_state = game_state->pong_state;

	// Make this ball half, duplicate self
	ball_reflect(ball, wielder->facing);
	ball->radius = ball->radius*0.75;
	if (ball->radius < 1) { ball->radius = 1; }
	ball->score_damage /= 2;
	if (ball->score_damage < 5) { ball->score_damage = 5; }

	struct BallData copy = *ball;
	copy.vel = Vec2Rotate(copy.vel, randInt(-20, 20));
	
	pong_state->balls[pong_state->num_balls] = copy;
	pong_state->num_balls += 1;

	Decoration *dec = decoration_spawn(DE_SLICE, pong_state);
	if (dec != NULL) {
		dec->pos = ball->pos;
	}

	play_sfx(game_state->music_mind, SFX_SWORD_SLICE);
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

	// World walls
	if (ball->pos.x - ball->radius <= world_borders.left) {
		ball->pos.x = world_borders.left + ball->radius;
		ball_reflect_wall(ball);
		play_sfx(state->music_mind, SFX_BALL_HIT_WALL);
	} else if (ball->pos.x + ball->radius >= world_borders.right) {
		ball->pos.x = world_borders.right - ball->radius;
		ball_reflect_wall(ball);
		play_sfx(state->music_mind, SFX_BALL_HIT_WALL);
	}

	// Collisions per paddle
	ball->ym_gravity_turn_speed = 0;
	for (int p=0; p<state->pong_state->num_paddles; p++) {
		PaddleData *paddle = &state->pong_state->paddles[p];
		if (paddle->destroyed_timer > 0) { continue; }
		// Paddle Ball Collisions
		struct Rectangle pRect = {paddle->pos.x, paddle->pos.y, paddle->paddle_width, paddle->paddle_thickness};
		if (CheckCollisionCircleRec(ball->pos, ball->radius, pRect)) {
			ball_paddle_hit(ball, paddle, state);
		}

		// Sword
		if (paddle->items[ITEM_CEREMONIAL_SWORD] > 0 && paddle->sword_timer > 0) {
			if (CheckCollisionCircleRec(ball->pos, ball->radius, sword_get_hitbox(paddle))) {
				ball_sword_hit(ball, state, paddle);
				paddle->sword_timer = 0;
			}
		}

		// Yo Momma 
		if (paddle->items[ITEM_YO_MOMMA] > 0) {
			Circle paddle_gravity = paddle_get_gravity_circle(paddle);
			if (is_heading_towards(ball->pos, ball->vel, paddle_center(paddle)) &&
				CheckCollisionCircles((Vector2){paddle_gravity.x, paddle_gravity.y}, paddle_gravity.radius, ball->pos, ball->radius)
			) {
				ball->ym_gravity_turn_speed = YM_BASE_STRENGTH;
				ball->ym_gravity_center = paddle_center(paddle);
			}
		}
		
		

	} // /paddle collisions



	// Expired panadol edge of screen collision
	PaddleData *p1 = state->player1->paddle;
	PaddleData *p2 = state->player2->paddle;
	if (ball->pos.y - ball->radius < world_borders.top && p1->items[ITEM_EXPIRED_PANADOL] > 0) {
		p1->items[ITEM_EXPIRED_PANADOL] -= 1;
		p1->item_use_timers[ITEM_EXPIRED_PANADOL] = ITEM_USE_BUMP_TIME;
		ball->vel.y = -ball->vel.y; // TODO: better reflection fn ?
		play_sfx(state->music_mind, SFX_EXPIRED_PANADOL);
	} else if (ball->pos.y + ball->radius > world_borders.bottom && p2->items[ITEM_EXPIRED_PANADOL] > 0) {
		p2->items[ITEM_EXPIRED_PANADOL] -= 1;
		p2->item_use_timers[ITEM_EXPIRED_PANADOL] = ITEM_USE_BUMP_TIME;
		ball->vel.y = -ball->vel.y; // TODO: better reflection fn ?
		play_sfx(state->music_mind, SFX_EXPIRED_PANADOL);
	}

	// Scoring
	if (ball->pos.y - ball->radius > world_borders.bottom+10) {
		ball_score_hit(ball, state->player1, state->player2);
	} else if (ball->pos.y + ball->radius + 10 < world_borders.top) {
		ball_score_hit(ball, state->player2, state->player1);
	}
}

void ball_draw(struct BallData *ball, bool debug) {
	DrawCircle(ball->pos.x, ball->pos.y, ball->radius, ball->color);
	if (debug) {
		// Draw line of balls knuckleball facing
		DrawLineEx(ball->pos, Vec2Add(ball->pos, Vec2MultScalar(ball->vel, 50)), 3, RED); 
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
