#include "raylib.h"

const int NUM_ITEMS=2;

enum Items {
	EXPIRED_PANADOL=0, // Adds one loss protection
	CHERRY_BLOSSOM_CLOAK=1, // Let's the paddle dash
};

struct PlayerData {
	int score;
};

struct PaddleData {
	Vector2 pos;
	int paddle_width;
	int paddle_thickness;
	Color color;
	
	int items[16];
	int items_count[16];

	int loss_protection; // Higher than 0 and the ball will respawn on score, decrementing this number

};

struct BallData {
	Vector2 pos;
	Vector2 vel;
	int radius;
};
