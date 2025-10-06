#include "Vec2.h"
#include "obj.h"
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

void rocket_init(RocketData *r, struct PaddleData *spawner) {
		Vector2 rocket_dir = {
			.x = 0,
			.y = -1,
		};
		Vector2 rocket_pos = Vec2Add(spawner->pos, (Vector2){spawner->paddle_width/2, 0});
		if (spawner->id == 1) { 
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
}

void rocket_fly(float dt, RocketData *r) {
	if (r->fall_timer > 0) {
		int m = 1;
		if (r->dir.y < 0) { m = -1; }
		r->pos = Vec2Add(r->pos, Vec2MultScalar((Vector2){0, r->fall_vel*m}, dt));
		r->fall_vel -= GRAVITY;
		r->speed = r->fall_vel;
		r->dir = Vec2Rotate(r->dir, r->rot_force*dt);
		r->fall_timer -= dt;
		if (r->fall_timer < 0) {
			r->speed = ROCKET_FLY_INIT_SPEED;
		}
	} else {
		r->pos = Vec2Add(r->pos, Vec2MultScalar(r->dir, r->speed*dt));
		//const angle = 
		float angle = Vec2GetAngle(r->dir);
		float ang_to_top = get_angle_distance(angle, -90);
		float ang_to_bottom = get_angle_distance(angle, 90);
		if (abs(ang_to_top) < abs(ang_to_bottom)) {
			float ang_dir = 1;
			if (ang_to_top < 0) { ang_dir = -1; }
			printf("Ang Dir %f\n", ang_dir);
			r->dir = Vec2Rotate(r->dir, ROCKET_TURN_SPEED*ang_dir*dt);
		} else {
			float ang_dir = 1;
			if (ang_to_bottom < 0) { ang_dir = -1; }
			r->dir = Vec2Rotate(r->dir, ROCKET_TURN_SPEED*ang_dir*dt);
		}

		r->speed += ROCKET_ACCELERATION;
	}
	//r->dir = Vec2Rotate(r->dir, 0.1);

	update_animation(dt, *r->animation, &r->anim_frame, &r->anim_timer);
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

void rocket_check_collisions(RocketData *r, WorldBorders borders, struct GameState *state) {
	// if rocket is off screen, delete
	if (r->pos.y > borders.bottom + 20 || r->pos.y < borders.top - 20) {
		printf("Rocket Delete\n");
		r->delete_me = true;
		return;
	}
	
	Vector2 hitbox[4];
	get_rocket_hitbox(r, hitbox);

	Circle c = get_missile_head_hitbox(r);
	Vector2 c_pos = {
		.x = c.x,
		.y = c.y
	};

	struct PongState *pong_state = state->pong_state;

	// if rocket collides with ball, destroy it and spawn explosion
	for (int i=0; i<pong_state->num_balls; i++) {
		struct BallData *ball = &pong_state->balls[i];
		if (CheckCollisionCircles(ball->pos, ball->radius, c_pos, c.radius)) {
			r->delete_me = true;
			explosion_spawn(c_pos, pong_state);
			return;
		}
	}
	
	// if rocket collides with paddle, damage it (destroy it for momentary respawn?)
	// if the rocket is in a time wizards zone, slow it down
}


void rocket_draw(RocketData *r, bool debug) {
	float angle = Vec2GetAngle(r->dir);
	
	printf("%d\n", r->anim_frame);
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
		/*
		Vector2 hb[5];
		get_rocket_hitbox(r, hb);
		hb[4] = hb[0];
		DrawLineStrip(hb, 5, RED);
		*/
	}


	//printf("Angle: %f\n; Dir: (%.2f, %.2f)", angle, r->dir.x, r->dir.y);
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
