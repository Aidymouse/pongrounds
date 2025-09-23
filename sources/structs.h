#ifndef _STRUCTSH_
#define _STRUCTSH_

#include "raylib.h"

enum Items {
	ITEM_EXPIRED_PANADOL=0, // Higher than 0 and the ball will respawn on score, decrementing this number
	ITEM_CHERRY_BLOSSOM_CLOAK=1, // Let's the paddle dash
	ITEM_HYPERGONADISM=2, // 
	ITEM_CHASTITY_CAGE=3, // Make opponents paddle shorter
};
 
enum GAME_STATES {
	PONG=0,
	PICK_ITEM=1,
	PAUSE=2,
	MENU=3
};

struct PaddleData {
	Vector2 pos;
	Vector2 vel;

	int paddle_width;
	int paddle_thickness;
	int hp;
	int max_hp;
	Color color;
	
	int items[16]; // Current items. Idx of arr is equal to item enum
	int items_total[16]; // Total held items, used to replenish above arr


};

struct PlayerData {
	int score;
	int rounds_won;
	struct PaddleData *paddle;
};


struct BallData {
	Vector2 pos;
	Vector2 vel;
	int radius;
};

struct PongState {
	int current_round;
};

struct PickItemsState {
	int item_choices[8]; // 4 by default but can do up
	int num_item_choices; // 4 by default
	int hovered_item; // Idx of the hovered item
	struct PaddleData *choosing_paddle;
};

struct GameState {
	int state; // GAME_STATE

	struct PlayerData *player1;
	struct PlayerData *player2;
	struct BallData *ball;
	struct PongState *pong_state;
	struct PickItemsState *pick_items_state;
};

#endif
