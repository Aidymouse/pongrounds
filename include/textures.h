#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include "defines.h"
#include "raylib.h"

extern Texture2D tex_item_cards[NUM_ITEMS];
extern Texture2D tex_small_items[NUM_ITEMS];
extern Texture2D tex_sword;
extern Texture2D tex_missile;
extern Texture2D tex_explosion;
extern Texture2D tex_dec_reflect;
extern Texture2D tex_dec_slice;
extern Texture2D tex_dec_clink;

void load_textures();
void unload_textures();


#endif
