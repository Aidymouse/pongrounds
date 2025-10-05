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
	.frame_duration = 0.016,
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

void rocket_check_collisions(RocketData *r, WorldBorders borders, struct PaddleData **paddles) {
	// if rocket is off screen, delete
	if (r->pos.y > borders.bottom + 20 || r->pos.y < borders.top - 20) {
		printf("Rocket Delete\n");
		r->delete_me = true;
		return;
	}
	// if rocket collides with ball, destroy it and spawn explosion
	
	// if rocket collides with paddle, damage it (destroy it for momentary respawn?)
}


void rocket_draw(RocketData *r) {
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

	DrawCircle(
		r->pos.x,
		r->pos.y,
		3,
		RED
	);

	//printf("Angle: %f\n; Dir: (%.2f, %.2f)", angle, r->dir.x, r->dir.y);
}

