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

void load_textures() {

	tex_sword = LoadTexture(ASSETS_PATH"sword swing.png");
	tex_missile = LoadTexture(ASSETS_PATH"missile shoot.png");
	tex_explosion = LoadTexture(ASSETS_PATH"explosion.png");

	for (int i=0; i<NUM_ITEMS; i++) {
		if (strcmp(item_filenames[i], "") != 0) {
			char b[100];
			sprintf(b,"%s%s", ASSETS_PATH, item_filenames[i]);
			tex_item_cards[i] = LoadTexture(b);
			printf("Would load %s (%d)\n", b, i);
		}

		if (strcmp(small_item_filenames[i], "") != 0) {
			char b[100];
			sprintf(b,"%s%s", ASSETS_PATH, small_item_filenames[i]);
			tex_small_items[i] = LoadTexture(b);
			printf("Would load small %s (%d)\n", b, i);
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
