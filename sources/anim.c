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

 
void init_animations(Texture2D *textures, int texture_count) { }

void draw_from_animation(Texture2D texture, Animation anim, Rectangle target) {
	DrawTexturePro(texture, anim.rect, target, (Vector2){0, 0}, 0, WHITE);
}
