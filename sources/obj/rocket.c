#include "Vec2.h"
#include "obj.h"
#include "raylib.h"
#include "anim.h"
#include "helper.h"
#include <stdlib.h>
#include <stdio.h>


void rocket_fly(float dt, RocketData *r) {
	if (r->fall_timer > 0) {
		r->fall_timer -= dt;
		int m = 1;
		if (r->dir.y < 0) { m = -1; }
		r->pos = Vec2Add(r->pos, Vec2MultScalar((Vector2){0, r->fall_vel*m}, dt));
		r->fall_vel -= GRAVITY;
		r->speed = r->fall_vel;
		r->dir = Vec2Rotate(r->dir, r->rot_force*dt);
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

		r->speed += 20;
	}
	//r->dir = Vec2Rotate(r->dir, 0.1);
}

void rocket_check_collisions(RocketData *r, WorldBorders borders) {
	// if rocket is off screen, delete
	if (r->pos.y > borders.bottom + 20 || r->pos.y < borders.top - 20) {
		printf("Rocket Delete\n");
		r->delete_me = true;
		return;
	}
	// if rocket collides with ball, destroy it and spawn explosion
	// if rocket collides with paddle, damage it (destroy it for momentary respawn?)
}

void rocket_draw(RocketData *r, Texture2D *missile) {
	float angle = Vec2GetAngle(r->dir);

	DrawTexturePro(
		*missile,
 		missile_0.rect, 
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

