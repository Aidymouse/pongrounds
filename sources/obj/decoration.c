#include "obj/obj.h"
#include "anim.h"
#include "structs.h"
#include "textures.h"
#include <stdio.h>

FrameAnimation fa_reflect = {
	.tex_width = 32, .tex_height = 32,
	.frames_width = 256/32, .frames_height = 1,
	.frame_start_x = 0, .frame_start_y = 0, 
	.frame_duration = 1/60,
};

FrameAnimation fa_slice = {
	.tex_width = 128, .tex_height = 128,
	.frames_width = 1028/128, .frames_height = 1,
	.frame_start_x = 0, .frame_start_y = 0, 
	.frame_duration = 1/60,
};

FrameAnimation fa_clink = {
	.tex_width = 128, .tex_height = 128,
	.frames_width = 768/128, .frames_height = 1,
	.frame_start_x = 0, .frame_start_y = 0, 
	.frame_duration = 1/60,
};

void decoration_init(Decoration *d) {
	d->pos.x = 0;
	d->pos.y = 0;
	d->anim_timer = 0;
	d->anim_frame = 0;
	d->delete_me = false;
}

void decoration_update(float dt, Decoration *d) {
	bool finished = update_animation(dt, d->animation, &d->anim_frame, &d->anim_timer);
	if (finished == true) { d->delete_me = true; }
}

// Returns NULL if we can't spawn a decoration
Decoration* decoration_spawn(enum Decorations dec_type, PongState *pong_state) {

	if (pong_state->num_decorations >= MAX_DECORATIONS) {
		return NULL;
	}

	Decoration *dec = &pong_state->decorations[pong_state->num_decorations];
	decoration_init(dec);

	if (dec_type == DE_REFLECT) {
		// Set texture and animation
		dec->animation = fa_reflect;
		dec->tex = &tex_dec_reflect;
	} else if (dec_type == DE_SLICE) {
		dec->animation = fa_slice;
		dec->tex = &tex_dec_slice;
	} else if (dec_type == DE_CLINK) {
		dec->animation = fa_clink;
		dec->tex = &tex_dec_clink;
	}

	pong_state->num_decorations += 1;

	return dec;
}

void decoration_draw(Decoration *d) {
	DrawTexturePro(
		*d->tex,
 		get_rect_from_animation(d->animation, d->anim_frame),
		(Rectangle){d->pos.x, d->pos.y, d->animation.tex_width, d->animation.tex_height}, 
		(Vector2){d->animation.tex_width/2, d->animation.tex_height/2},
		0,
		WHITE
	);
}

void decoration_cleanup(PongState *pong_state) {
	for (int i=0; i<pong_state->num_decorations; i++) {
		Decoration *d = &pong_state->decorations[i];	
		if (d->delete_me) {
			pong_state->decorations[i] = pong_state->decorations[pong_state->num_decorations-1];
			pong_state->num_decorations -= 1;
			i -= 1;
		}
	}
}
