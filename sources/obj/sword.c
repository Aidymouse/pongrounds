#include "obj/obj.h"
#include "Vec2.h"
#include <math.h>
#include "defines.h"
#include "helper.h"
#include "raylib.h"


void sword_swing(float dt, struct PaddleData *p) {
	if (p->sword_timer > 0) { 
		p->sword_timer -= dt; 
		if (p->sword_timer <= 0) {
 			p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] = SWORD_COOLDOWN - 0.2 * p->items[ITEM_CEREMONIAL_SWORD];
		}
	}

}

Rectangle sword_get_hitbox(struct PaddleData *p) {
	Rectangle r = (Rectangle){p->pos.x, 0, p->paddle_width, 50};
	if (p->id == 1) {
		r.y = p->pos.y + p->paddle_thickness;
	} else {
		r.y = p->pos.y - r.height;
	}
	return r;
}

void sword_draw(struct PaddleData *p, bool debug) {
	
	Rectangle r = sword_get_hitbox(p);
	if (debug) {
		if (p->sword_timer > 0) {
			DrawRectangleLines(r.x, r.y, r.width, r.height, RED);
		} else if (p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] > 0) {
			DrawRectangleLines(r.x, r.y, r.width, r.height, BLUE);
		} else {
			DrawRectangleLines(r.x, r.y, r.width, r.height, GRAY);
		}
		
	}
}

