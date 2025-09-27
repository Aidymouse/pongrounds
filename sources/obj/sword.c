#include "obj/obj.h"
#include "Vec2.h"
#include <math.h>
#include "defines.h"
#include "helper.h"
#include "raylib.h"


void sword_swing(float dt, struct PaddleData *p) {
	if (p->sword_cooldown_timer > 0) { p->sword_cooldown_timer -= dt; }
	if (p->sword_timer > 0) { 
		p->sword_timer -= dt; 
		if (p->sword_timer <= 0) {
 			p->sword_cooldown_timer = SWORD_COOLDOWN - 0.2 * p->items[ITEM_CEREMONIAL_SWORD];
		}
	}

}


void sword_draw(struct PaddleData *p, bool debug) {
	
	if (debug) {
		if (p->sword_timer > 0) {
			DrawRectangleLines(p->pos.x, p->pos.y-50, p->paddle_width, 50, RED);
		} else if (p->sword_cooldown_timer > 0) {
			DrawRectangleLines(p->pos.x, p->pos.y-50, p->paddle_width, 50, BLUE);
		} else {
			DrawRectangleLines(p->pos.x, p->pos.y-50, p->paddle_width, 50, GRAY);
		}
		
	}
}

