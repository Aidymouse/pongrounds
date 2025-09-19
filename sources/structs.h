#include "raylib.h"

struct PlayerData {
	int score;
};

struct PaddleData {
	Vector2 pos;
	int paddle_width;
	int paddle_thickness;
	Color color;
};

struct BallData {
	Vector2 pos;
	Vector2 vel;
	int radius;
};
