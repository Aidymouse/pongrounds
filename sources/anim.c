#include "anim.h"
#include "raylib.h"
#include "structs.h"
#include <stddef.h>

// Rocket //
Animation missile_0 = {
	.duration = 1/60,
	.rect = (Rectangle) {0, 0, MISSILE_TEX_DIMS_PX, MISSILE_TEX_DIMS_PX},
	.next_frame = &missile_1,
};

Animation missile_1 = {
	.duration = 1/60,
	.rect = (Rectangle) {MISSILE_TEX_DIMS_PX*1, 0, MISSILE_TEX_DIMS_PX, MISSILE_TEX_DIMS_PX},
	.next_frame = NULL, // TODO
};

// Sword //
Animation sword_0 = {
	.duration = 1/60,
	.rect = (Rectangle) {SWORD_TEX_DIMS_PX*0, 0, SWORD_TEX_DIMS_PX, SWORD_TEX_DIMS_PX},
	.next_frame = NULL, // TODO
};

FrameAnimation missile = {
	.tex_width = MISSILE_TEX_DIMS_PX,
	.tex_height = MISSILE_TEX_DIMS_PX,
	.frames_width = 3,
	.frames_height = 1,
};

 
void init_animations(Texture2D *textures, int texture_count) { }

void draw_from_animation(Texture2D texture, Animation anim, Rectangle target) {
	DrawTexturePro(texture, anim.rect, target, (Vector2){0, 0}, 0, WHITE);
}

Rectangle get_rect_from_animation(FrameAnimation anim, int frame) {
	int frames_down = (frame - (frame % anim.frames_width)) / anim.frames_width;
	int frames_across = frame % anim.frames_width;

	return (Rectangle){
		frames_across*anim.tex_width,
		frames_down*anim.tex_height,
		anim.tex_width,
		anim.tex_height
	};
}
