#ifndef _ANIM_H_
#define _ANIM_H_

#include "raylib.h"
#include "structs.h"

/*
extern struct Animation card_item_0;
extern struct Animation card_item_1;
extern struct Animation card_item_2;
*/

extern Animation missile_0;
extern Animation missile_1;

void init_animations(Texture2D *textures, int texture_count);
void draw_from_animation(Texture2D texture, Animation anim, Rectangle target);

#endif
