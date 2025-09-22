#ifndef _STRUCTSH_
#define _STRUCTSH_

#include "raylib.h"

#define NUM_ITEMS 2;

enum Items {
	ITEM_EXPIRED_PANADOL=0, // Higher than 0 and the ball will respawn on score, decrementing this number
	ITEM_CHERRY_BLOSSOM_CLOAK=1, // Let's the paddle dash
};
 
enum GAME_STATES {
	PONG=0,
	PICK_ITEM=1,
	PAUSE=2,
	MENU=3
};

struct PaddleData {
	Vector2 pos;
	int paddle_width;
	int paddle_thickness;
	Color color;
	
	int items[16]; // Current items
	int items_total[16]; // Total held items


};

struct PlayerData {
	int score;
	struct PaddleData *paddle;
};


struct BallData {
	Vector2 pos;
	Vector2 vel;
	int radius;
};

struct GameState {
	int current_round;
	int state; // GAME_STATE
};

#endif
