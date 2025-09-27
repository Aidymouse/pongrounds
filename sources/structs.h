#ifndef _STRUCTSH_
#define _STRUCTSH_

#include "raylib.h"

enum Items {
	ITEM_EXPIRED_PANADOL=0, // Higher than 0 and the ball will respawn on score, decrementing this number
	ITEM_CHERRY_BLOSSOM_CLOAK=1, // Let's the paddle dash
	ITEM_HYPERGONADISM=2, // 
	ITEM_CHASTITY_CAGE=3, // Make opponents paddle shorter
	ITEM_NERD_GLASSES=4,
	ITEM_CEREMONIAL_SWORD=5,
	ITEM_TIME_WIZARDS_CHRONOMETER=6,
	ITEM_BACHELOR_OF_PSYCHOLOGY_HONS=7,
	ITEM_RUSSIAN_SECRETS=8,
	ITEM_LEGALLY_DISTINCT_GOLDEN_SNOTCH=9,
	ITEM_NIEKRO_CARD=10, // Chance of knuckleball on hit
	ITEM_ANTIQUE_GAME_CONSOLE=11
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
	int id; // 1 is player 1, 2 is player 2. Player 2 owns the bottom side of the screen

	int paddle_width;
	int paddle_thickness;
	int hp;
	int max_hp;
	Color color;
	
	int items[16]; // Current items. Idx of arr is equal to item enum
	int items_total[16]; // Total held items, used to replenish above arr


};

struct PlayerData {
	int rounds_won;
	struct PaddleData *paddle;
};

enum BallState {
	BS_NORMAL, // Bounces and moves as normal
	BS_KNUCKLEBALL, // Moves funny
	BS_DESTROYED, // Doesn't update at all
};

struct BallData {
	Vector2 pos;
	int radius;
	// Actually the unit vector direction of velocity
	Vector2 vel; 
	int speed; 
	bool destroyed;
	// Pointer to paddle that last hit the ball. WARN: will be -1 if the ball has not been hit yet
	struct PaddleData *last_hit_by;
	// How much damage to inflict when a point is scored with this ball
	int score_damage; 

	enum BallState state;

	// KB state vars //
	// Angle direction when knuckleball
	Vector2 kb_dir; 
	// The dir we try to move towards
	float kb_desired_angle; 
	float kb_turn_speed;
	// When this timer hit's 0 we pick a new desired dir. Time varies from like .2s to .8s
	float kb_dir_timer; 

	

	
};

struct PongState {
	int current_round;
	float ball_respawn_timer; // Time that delays the ball respawn
	int num_balls;
	struct BallData balls[16]; // Up to 16 balls!
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
	struct PongState *pong_state;
	struct PickItemsState *pick_items_state;
	Camera2D *camera;
};

struct PaddleControls {
	int left;
	int right;
	int dash;
};

#endif
