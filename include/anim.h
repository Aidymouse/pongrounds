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

extern Animation sword_0;

void init_animations(Texture2D *textures, int texture_count);
void draw_from_animation(Texture2D texture, Animation anim, Rectangle target);

//void update_animation(float dt, FrameAnimation anim, int *anim_timer, int *anim_frame);
Rectangle get_rect_from_animation(FrameAnimation anim, int frame);
bool update_animation(float dt, FrameAnimation anim, int *frame, float *anim_timer);

#endif
