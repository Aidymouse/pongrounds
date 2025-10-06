
#ifndef _STRUCTSH_
#define _STRUCTSH_

#include "raylib.h"
#include "defines.h"

enum Items {
	ITEM_EXPIRED_PANADOL=0, // Higher than 0 and the ball will respawn on score, decrementing this number
	ITEM_CHERRY_BLOSSOM_CLOAK=1, // Let's the paddle dash
	ITEM_HYPERGONADISM=2, // 
	ITEM_CHASTITY_CAGE=3, // Make opponents paddle shorter
	ITEM_NERD_GLASSES=4,
	ITEM_CEREMONIAL_SWORD=5,
	ITEM_TIME_WIZARDS_CHRONOMETER=6,
	ITEM_BACHELOR_OF_PSYCHOLOGY_HONS=7,
	ITEM_NIEKRO_CARD=8, // Chance of knuckleball on hit
	ITEM_NUCLEAR_LAUNCH_CODES=9
};

 
typedef struct Animation {
	Rectangle rect;
	float duration;
	struct Animation *next_frame;
} Animation;

typedef struct FrameAnimation {
	float tex_width;
	float tex_height;
	int frames_width;
	int frames_height;
	int frame_start_x; // offset
	int frame_start_y; // offset from top left in frames
	float frame_duration;
} FrameAnimation;

enum GAME_STATES {
	PONG=0,
	PICK_ITEM=1,
	PAUSE=2,
	MENU=3
};

/** Objects **/
struct PaddleData {
	Vector2 pos;
	Vector2 vel; // Unit vector velocity direction
	float speed;
	// Max normal speed while not dashing
	float max_speed; 
	int id; // 1 is player 1, 2 is player 2. Player 2 owns the bottom side of the screen

	int paddle_width;
	int paddle_thickness;
	int hp;
	int max_hp;
	Color color;
	
	// Current items. Idx of arr is equal to item enum
	int items[NUM_ITEMS]; 
	// Total held items, used to replenish above arr
	int items_total[NUM_ITEMS]; 
	// If these timers are greater than 0, the item is lit up on the item display
	float item_use_timers[NUM_ITEMS];
	// Not every item has a timer but i'm gonna keep this around anyway
	float item_cooldown_timers[NUM_ITEMS]; 

	// How long the sword hitbox lasts
	float sword_timer;
	float sword_anim_timer;
	int sword_frame;
	int sword_anim_dir; // -1 if back, 1 if forward


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

typedef struct RocketData {
	Vector2 pos;
	Vector2 dir;	
	float speed;

	float fall_vel; // Speed applied to missiles Y when its falling initially
	float fall_timer; // the timer to track how long the rocket falls after the initial burst
	float rot_force; // Rotational force on initial hang time
	float speed_multiplier;

	int anim_frame;
	float anim_timer;
	FrameAnimation *animation;

	bool delete_me; // If true, this rocket will be deleted this frame
} RocketData;

typedef struct Explosion {
	Vector2 pos;
	float radius;
	float life_timer; // the clock is always ticking
	bool delete_me;
} Explosion;

struct PlayerData {
	int rounds_won;
	struct PaddleData *paddle;
};

/** Util **/
typedef struct Circle {
	float x;
	float y;
	float radius;
} Circle;

/** States **/
struct PongState {
	int current_round;
	float ball_respawn_timer; // Time that delays the ball respawn
	float end_round_timer;
	int num_balls;
	struct BallData balls[16]; // Up to 16 balls!
	RocketData rockets[8]; // Swapback array of rockets
	int num_rockets;
	Explosion explosions[MAX_EXPLOSIONS];
	int num_explosions;
};

struct PickItemsState {
	float item_anim_timers[8];
	int item_anim_frames[8];
	//struct Animation item_animations[8];
	int item_choices[8]; // 3 by default but can do up
	int num_item_choices; // 3 by default
	int hovered_item; // Idx of the hovered item
	struct PaddleData *choosing_paddle;
};

typedef struct WorldBorders {
	float top;
	float bottom;
	float left;
	float right;
} WorldBorders;

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
	int up;
	int down;
};



#endif


