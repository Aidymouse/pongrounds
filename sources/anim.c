#include "anim.h"
#include "raylib.h"
#include "structs.h"
#include <stddef.h>

struct Animation card_item_0 = {
	.duration = 0.16,
	.rect = (Rectangle){ 144*0, 0, 144, 144 },
	.next_frame = &card_item_1,
};

struct Animation card_item_1 = {
	.duration = 0.16,
	.rect = (Rectangle){ 144*1, 0, 144, 144 },
	.next_frame = &card_item_2,
};


struct Animation card_item_2 = {
	.duration = 0.16,
	.rect = (Rectangle){ 144*2, 0, 144, 144 },
	.next_frame = NULL
};

 
void init_animations(Texture2D *textures, int texture_count) {

	/*
	card_item_0.duration = 0.16;
	card_item_0.texture = &textures[ITEM_TIME_WIZARDS_CHRONOMETER];
	card_item_0.rect = (Rectangle){ 0, 0, 144, 144 };
	card_item_0.next_frame = &card_item_1;

	card_item_1.duration = 0.16;
	card_item_1.texture = &textures[ITEM_TIME_WIZARDS_CHRONOMETER];
	card_item_1.next_frame = &card_item_2;

	card_item_2.duration = 0.16;
	card_item_2.texture = &textures[ITEM_TIME_WIZARDS_CHRONOMETER];
	card_item_2.rect = (Rectangle){ 144*2, 0, 144, 144 };
	card_item_2.next_frame = NULL;
	*/


}

void draw_from_animation(Texture2D texture, struct Animation anim, Rectangle target) {
	DrawTexturePro(texture, anim.rect, target, (Vector2){0, 0}, 0, WHITE);
}
