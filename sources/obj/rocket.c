#include "Vec2.h"
#include "obj/obj.h"
#include "raylib.h"
#include "anim.h"
#include "helper.h"
#include <stdlib.h>
#include <stdio.h>
#include "textures.h"
#include "structs.h"
#include "defines.h"

FrameAnimation missile_init = {
	.tex_width = MISSILE_TEX_DIMS_PX,
	.tex_height = MISSILE_TEX_DIMS_PX,
	.frames_width = 19,
	.frames_height = 1,
	.frame_start_x = 0,
	.frame_start_y = 0,
	.frame_duration = 0.016*2,
};

FrameAnimation missile_loop = {
	.tex_width = MISSILE_TEX_DIMS_PX,
	.tex_height = MISSILE_TEX_DIMS_PX,
	.frames_width = 4,
	.frames_height = 1,
	.frame_start_x = 0,
	.frame_start_y = 1,
	.frame_duration = 0.016,
};

Circle get_missile_head_hitbox(RocketData *r) {
	float HB_DIST=70;
	Vector2 circle_pos = Vec2Add(r->pos, Vec2MultScalar(r->dir, HB_DIST));
	Circle c = {
		.x = circle_pos.x,
		.y = circle_pos.y,
		.radius = 7,
	};
	return c;
}

void rocket_init(RocketData *r, PaddleData *spawner) {
		Vector2 rocket_dir = spawner->facing;

		Vector2 rocket_pos = Vec2Add(paddle_center(spawner), Vec2MultScalar(spawner->facing, spawner->facing.y));
		if (spawner->facing.y == 1) { 
			rocket_dir.y = 1; 
			rocket_pos.y += spawner->paddle_thickness;
		}

		r->pos = rocket_pos;
		r->dir = rocket_dir;
		r->speed = ROCKET_INIT_SPEED;
		r->fall_timer = HANG_TIME; 
		r->fall_vel = ROCKET_INIT_SPEED;
		r->rot_force = randInt(-20, 20);
		r->anim_frame = 1;
		r->animation = &missile_init;
		r->anim_timer = r->animation->frame_duration;
		r->delete_me = false;
		r->speed_multiplier = 1;
		r->detonation_timer = 0;
		r->ym_turn_speed = 0;
}

void rocket_fly(float dt, RocketData *r, struct GameState *state) {

	struct PongState *pong_state = state->pong_state;

	// Detonation timer
	if (r->detonation_timer > 0) {
		r->detonation_timer -= dt;
		if (r->detonation_timer <= 0) {
			Circle c_head = get_missile_head_hitbox(r);
			Vector2 c_head_pos = { .x = c_head.x, .y = c_head.y };
			r->delete_me = true;
			explosion_spawn(c_head_pos, pong_state);
			state->screenshake_timer = MISSILE_SHAKE_DUR;
			state->screenshake_amplitude = MISSILE_SHAKE_AMPL;
			return;
		}
	}
	
	// Move
	if (r->fall_timer > 0) {
		int m = 1;
		if (r->dir.y < 0) { m = -1; }
		r->pos = Vec2Add(r->pos, Vec2MultScalar((Vector2){0, r->fall_vel*r->speed_multiplier*m}, dt));
		r->fall_vel -= GRAVITY;
		r->speed = r->fall_vel;
		r->dir = Vec2Rotate(r->dir, r->rot_force*r->speed_multiplier*dt);
		r->fall_timer -= dt*r->speed_multiplier;
		if (r->fall_timer < 0) {
			r->speed = ROCKET_FLY_INIT_SPEED;
		}
	} else {
		r->pos = Vec2Add(r->pos, Vec2MultScalar(r->dir, r->speed*r->speed_multiplier*dt));
		
		float angle = Vec2GetAngle(r->dir);
		float ang_to_top = get_angle_distance(angle, -90);
		float ang_to_bottom = get_angle_distance(angle, 90);

		if (r->ym_turn_speed > 0) {
			gravitate_towards(dt, r->pos, &r->dir, r->ym_target, r->ym_turn_speed);
		} else {
			if (fabsf(ang_to_top) < fabsf(ang_to_bottom)) {
				float ang_dir = 1;
				if (ang_to_top < 0) { ang_dir = -1; }
				r->dir = Vec2Rotate(r->dir, ROCKET_TURN_SPEED*ang_dir*r->speed_multiplier*dt);
			} else {
				float ang_dir = 1;
				if (ang_to_bottom < 0) { ang_dir = -1; }
				r->dir = Vec2Rotate(r->dir, ROCKET_TURN_SPEED*ang_dir*r->speed_multiplier*dt);
			}
		}

		r->speed += ROCKET_ACCELERATION;
	}
	//r->dir = Vec2Rotate(r->dir, 0.1);

	update_animation(dt*r->speed_multiplier, *r->animation, &r->anim_frame, &r->anim_timer);
}

// returns hitbox points by reference
void get_rocket_hitbox(RocketData *r, Vector2 *hitbox_points) {
	float HB_WIDTH=16;
	float HB_HEIGHT=80;

	float rocket_angle = Vec2GetAngle(r->dir);
	hitbox_points[0] = Vec2Add(r->pos, Vec2MultScalar(Vec2Rotate(r->dir, 90), HB_WIDTH/2));
	hitbox_points[1] = Vec2Add(hitbox_points[0], Vec2MultScalar(r->dir, HB_HEIGHT));
	hitbox_points[2] = Vec2Add(hitbox_points[1], Vec2MultScalar(Vec2Rotate(r->dir, -90), HB_WIDTH));
	hitbox_points[3] = Vec2Add(hitbox_points[2], Vec2MultScalar(r->dir, -HB_HEIGHT));
}


Circle get_missile_base_hitbox(RocketData *r) {
	float HB_DIST=0;
	Vector2 circle_pos = Vec2Add(r->pos, Vec2MultScalar(r->dir, HB_DIST));
	Circle c = {
		.x = circle_pos.x,
		.y = circle_pos.y,
		.radius = 7,
	};
	return c;
}

void rocket_check_collisions(RocketData *r, WorldBorders borders, struct GameState *state) {
	if (r->delete_me) { return; }
	// if rocket is off screen, delete
	if (r->pos.y > borders.bottom + 20 || r->pos.y < borders.top - 20) {
		r->delete_me = true;
		return;
	}

	Circle c_head = get_missile_head_hitbox(r);
	Vector2 c_head_pos = { .x = c_head.x, .y = c_head.y };

	// Used for some effects where we basically want to target the whole missile. Only the head is explosive
	Circle c_base = get_missile_base_hitbox(r);
	Vector2 c_base_pos = { .x = c_base.x, .y = c_base.y };

	struct PongState *pong_state = state->pong_state;

	// if rocket collides with ball, destroy it and spawn explosion
	for (int i=0; i<pong_state->num_balls; i++) {
		struct BallData *ball = &pong_state->balls[i];
		if (CheckCollisionCircles(ball->pos, ball->radius, c_head_pos, c_head.radius)) {
			r->delete_me = true;
			explosion_spawn(c_head_pos, pong_state);
			state->screenshake_timer = 0.5;
			state->screenshake_amplitude = 2;
			return;
		}
	}

	// if a rocket collides with another rocket, both should blow up!
	for (int i=0; i<pong_state->num_rockets; i++) {
		RocketData *rocket = &pong_state->rockets[i];
		if (rocket == r) { continue; } // I think this works?

		Circle r_c_head = get_missile_head_hitbox(rocket);
		Vector2 r_c_head_pos = { .x = r_c_head.x, .y = r_c_head.y };
		Circle r_c_base = get_missile_base_hitbox(rocket);
		Vector2 r_c_base_pos = { .x = r_c_base.x, .y = r_c_base.y };
		
		if (CheckCollisionCircles(c_head_pos, c_head.radius, r_c_head_pos, r_c_head.radius) || 
			CheckCollisionCircles(c_head_pos, c_head.radius, r_c_base_pos, r_c_head.radius)) {
			r->delete_me = true;
			explosion_spawn(c_head_pos, pong_state);
			rocket->delete_me = true;
			explosion_spawn(r_c_head_pos, pong_state);
			state->screenshake_timer = 0.5;
			state->screenshake_amplitude = 2;
			return;
			
		}
	}
	
	// if rocket collides with paddle, damage it (destroy it for momentary respawn?)
	
	r->speed_multiplier = 1; // Also handle time wizard here

	r->ym_turn_speed = 0;
	for (int p=0; p<pong_state->num_paddles; p++) {
		PaddleData *paddle = &pong_state->paddles[p];
		if (paddle->destroyed_timer > 0) { continue; }
		Rectangle paddle_rect = paddle_get_rect(paddle);
		if (CheckCollisionCircleRec(c_head_pos, c_head.radius, paddle_rect)) {
			r->delete_me = true;
			if (paddle->invincibility_timer <= 0) {
				paddle->destroyed_timer = MISSILE_DEATH_TIME;
			}
			explosion_spawn(c_head_pos, pong_state);
			state->screenshake_timer = MISSILE_SHAKE_DUR;
			state->screenshake_amplitude = MISSILE_SHAKE_AMPL;
			return;
		} 

		// if the rocket is in a time wizards zone, slow it down
		Rectangle time_influence = paddle_get_time_influence_area(paddle);
		if (CheckCollisionCircleRec(c_head_pos, c_head.radius, time_influence) || 
		CheckCollisionCircleRec(c_base_pos, c_base.radius, time_influence)) {
			r->speed_multiplier *= paddle_get_time_power(paddle);
		}

		// Paddle: Yo Momma
		if (paddle->items[ITEM_YO_MOMMA] > 0) {
			Circle ym_circ = paddle_get_gravity_circle(paddle);
			if (is_heading_towards(r->pos, r->dir, paddle_center(paddle)) &&
				CheckCollisionCircles(c_head_pos, c_head.radius, paddle_center(paddle), ym_circ.radius)
			) {
				r->ym_target = paddle_center(paddle);
				r->ym_turn_speed = YM_BASE_STRENGTH;
			}
		}
	
		// If we hit a sword --- DAMN!
		if (paddle->sword_timer > 0) {
			printf("Checking paddle %d for missile collision\n", paddle->id);
			Rectangle sword_box = sword_get_hitbox(paddle);
			if (CheckCollisionCircleRec(c_head_pos, c_head.radius, sword_box)) {
				r->dir = Vec2Rotate(r->dir, 180);
				r->detonation_timer = 1;

				state->hitstop_timer = SWORD_MISSILE_HITSTOP;
				state->screenshake_timer = SWORD_MISSILE_HITSTOP*5;
				state->screenshake_amplitude = 10;
				
				Decoration *dec = decoration_spawn(DE_CLINK, state->pong_state);
				if (dec != NULL) {
					dec->pos = c_head_pos;
				}
			}
		}
	}
}


void rocket_draw(RocketData *r, bool debug) {
	float angle = Vec2GetAngle(r->dir);
	
	if (r->anim_frame == 18) {
		r->animation = &missile_loop;
		r->anim_frame = 0;
	}

	DrawTexturePro(
		tex_missile,
 		get_rect_from_animation(*r->animation, r->anim_frame),
		(Rectangle){r->pos.x, r->pos.y, MISSILE_TEX_DIMS_PX, MISSILE_TEX_DIMS_PX}, 
		(Vector2){MISSILE_TEX_DIMS_PX/2, MISSILE_TEX_DIMS_PX-40},
		angle+90,
		WHITE
	);


	if (debug) {
		DrawCircle(
			r->pos.x,
			r->pos.y,
			3,
			RED
		);

		// draw hitbox
		Circle hit_circle = get_missile_head_hitbox(r);
		DrawCircleLines(hit_circle.x, hit_circle.y, hit_circle.radius, RED);
		Circle hit_circle_base = get_missile_base_hitbox(r);
		DrawCircleLines(hit_circle_base.x, hit_circle_base.y, hit_circle_base.radius, PINK);
	}


}

void rocket_cleanup(struct PongState *pong_state) {
	for (int i=0; i<pong_state->num_rockets; i++) {
		RocketData *r = &pong_state->rockets[i];	
		if (r->delete_me) {
			pong_state->rockets[i] = pong_state->rockets[pong_state->num_rockets-1];
			pong_state->num_rockets -= 1;
			i -= 1;
		}
	}
}
