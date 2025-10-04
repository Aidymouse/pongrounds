#include "obj/obj.h"
#include "Vec2.h"
#include <math.h>
#include "defines.h"
#include "helper.h"
#include "raylib.h"
#include "textures.h"
#include "anim.h"



void sword_swing(float dt, struct PaddleData *p) {
	if (p->sword_timer > 0) { 
		p->sword_timer -= dt; 
		if (p->sword_timer <= 0) {
 			p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] = SWORD_COOLDOWN - 0.2 * p->items[ITEM_CEREMONIAL_SWORD];
		}
	}
	
	if (p->sword_anim_timer > 0) {
			p->sword_anim_timer -= dt;	

			if (p->sword_anim_timer < 0) {
				p->sword_frame += 1;
				p->sword_anim_timer = SWORD_FRAME_TIME;
	
				if (p->sword_frame == NUM_SWORD_FRAMES-1) {
						p->sword_anim_dir = -p->sword_anim_dir;
						p->sword_anim_timer = 0;
						p->sword_frame = 0;
				}

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

	Rectangle sword_rect = { 
		.x = SWORD_TEX_DIMS_PX*p->sword_frame,
		.y = 0,
		.width = SWORD_TEX_DIMS_PX*p->sword_anim_dir,
		.height = SWORD_TEX_DIMS_PX 
	};
	Vector2 sword_origin = {
		.x = SWORD_TEX_DIMS_PX/2, 
		.y = SWORD_TEX_DIMS_PX-16,
	};
	Vector2 sword_pos = {
		.x = p->pos.x + p->paddle_width/2,
		.y = p->pos.y
	};

	if (p->id == 1) {
		sword_rect.height = SWORD_TEX_DIMS_PX*-1;
		sword_origin.y = 16;
		sword_pos.y += p->paddle_thickness;
	}

	DrawTexturePro(
		tex_sword,
		sword_rect,
		(Rectangle){sword_pos.x, sword_pos.y, SWORD_TEX_DIMS_PX, SWORD_TEX_DIMS_PX},
		sword_origin,
		0,
		WHITE
	);

	if (debug) {
		DrawCircle(sword_pos.x, sword_pos.y, 3, RED);
		Rectangle r = sword_get_hitbox(p);
		if (p->sword_timer > 0) {
			DrawRectangleLines(r.x, r.y, r.width, r.height, RED);
		} else if (p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] > 0) {
			DrawRectangleLines(r.x, r.y, r.width, r.height, BLUE);
		} else {
			DrawRectangleLines(r.x, r.y, r.width, r.height, GRAY);
		}
		
	}
}

