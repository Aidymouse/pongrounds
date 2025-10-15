#include "obj/obj.h"
#include "structs.h"
#include "raylib.h"
#include "anim.h"
#include "textures.h"

FrameAnimation anim_explosion = {
	.tex_width = 128,
	.tex_height = 128,
	.frames_width = 13,
	.frames_height = 1,
	.frame_start_x = 0,
	.frame_start_y = 0,
	.frame_duration = 0.016,
};

void explosion_init(Explosion *e) {
	e->anim_timer = anim_explosion.frame_duration;
	e->anim_frame = 0;
	e->radius = EXPLOSION_RADIUS;
	e->delete_me = false;
}

void explosion_spawn(Vector2 pos, struct PongState *pong_state) {
	Explosion e;
	e.pos = pos;
	explosion_init(&e);
	pong_state->explosions[pong_state->num_explosions] = e;
	pong_state->num_explosions += 1;
}

void explosion_update(float dt, Explosion *e) {
	if (e->delete_me) { return; }

	update_animation(dt, anim_explosion, &e->anim_frame, &e->anim_timer);

	if (e->anim_frame >= EXPLOSION_FRAMES-1) {
		e->delete_me = true;
	}
}

void explosion_draw(Explosion *e, bool debug) {
	
	DrawTexturePro(
		tex_explosion,
		get_rect_from_animation(anim_explosion, e->anim_frame),
		(Rectangle){e->pos.x, e->pos.y, EXPLOSION_TEX_DIMS_PX, EXPLOSION_TEX_DIMS_PX}, 
		(Vector2){EXPLOSION_TEX_DIMS_PX/2, EXPLOSION_TEX_DIMS_PX/2},
		0,
		WHITE
	);

	if (debug) {
		DrawCircleLines(e->pos.x, e->pos.y, e->radius, RED);
	}
}

void explosion_cleanup(struct PongState *pong_state) {
	for (int i=0; i<pong_state->num_explosions; i++) {
		Explosion *e = &pong_state->explosions[i];	
		if (e->delete_me) {
			pong_state->explosions[i] = pong_state->explosions[pong_state->num_explosions-1];
			pong_state->num_explosions -= 1;
			i -= 1;
		}
	}
}
