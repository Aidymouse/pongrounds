#include "textures.h"
#include "raylib.h"
#include "defines.h"
#include "items.h"
#include <stdio.h>
#include <string.h>

Texture2D tex_item_cards[NUM_ITEMS];
Texture2D tex_small_items[NUM_ITEMS];
Texture2D tex_sword;
Texture2D tex_missile;
Texture2D tex_explosion;

Texture2D tex_dec_reflect;
Texture2D tex_dec_slice;
Texture2D tex_dec_clink;

void load_textures() {

	tex_sword = LoadTexture(ASSETS_PATH"sword swing.png");
	tex_missile = LoadTexture(ASSETS_PATH"missile shoot.png");
	tex_explosion = LoadTexture(ASSETS_PATH"explosion.png");

	tex_dec_reflect = LoadTexture(ASSETS_PATH"reflect.png");
	tex_dec_slice = LoadTexture(ASSETS_PATH"slice.png");
	tex_dec_clink = LoadTexture(ASSETS_PATH"clink.png");

	for (int i=0; i<NUM_ITEMS; i++) {
		if (strcmp(item_filenames[i], "") != 0) {
			char b[100];
			sprintf(b,"%s%s", ASSETS_PATH, item_filenames[i]);
			tex_item_cards[i] = LoadTexture(b);
			//printf("Would load %s (%d, %s)\n", b, i, item_labels[i]);
		}

		if (strcmp(small_item_filenames[i], "") != 0) {
			char b[100];
			sprintf(b,"%s%s", ASSETS_PATH, small_item_filenames[i]);
			tex_small_items[i] = LoadTexture(b);
			//printf("Would load small %s (%d, %s)\n", b, i, item_labels[i]);
		}
	}
}

void unload_textures() {
	UnloadTexture(tex_sword);
	UnloadTexture(tex_missile);
	for (int i=0; i<NUM_ITEMS; i++) {
		UnloadTexture(tex_item_cards[i]);
		UnloadTexture(tex_small_items[i]);
	}
}
