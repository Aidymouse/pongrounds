#include "Vec2.h"
#include "obj.h"

void rocket_fly(float dt, RocketData *r) {
	if (r->fall_timer > 0) {
		//r->pos = Vec2Add(r->pos, Vec2MultScalar(r->dir, r->speed*dt));
		// TODO: fall
	} else {
		r->speed += 1;
	}
	r->pos = Vec2Add(r->pos, Vec2MultScalar(r->dir, r->speed*dt));
}

void rocket_check_collisions(RocketData *r) {
	// if rocket is off screen, delete
	// if rocket collides with ball, destroy it and spawn explosion
	// if rocket collides with paddle, damage it (destroy it for momentary respawn?)
}

void rocket_draw(RocketData *r) {
}

