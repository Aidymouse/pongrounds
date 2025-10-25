
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
	ITEM_NUCLEAR_LAUNCH_CODES=9,
	ITEM_RUSSIAN_SECRETS=10,
	ITEM_HYDRAULIC_PRESS=11,
	ITEM_ANTIQUE_GAME_CONSOLE=12,
	ITEM_CLONING_VAT=13,
	ITEM_SNOTCH=14,
	ITEM_YO_MOMMA=15,
	ITEM_BROKEN_MIND_CONTROL_MACHINE=16,
	//ITEM_UNDERWEAR_USED=17,
	ITEM_MUTANT_MOUSE=17,
	ITEM_SENTIENT_HAND=18,
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

enum Decorations {
	DE_REFLECT,
	DE_SLICE,
	DE_CLINK,
};

typedef struct Deocration {
	FrameAnimation animation;
	Texture *tex;
	Vector2 pos;
	float anim_timer;
	int anim_frame;
	bool delete_me;
} Decoration;

enum GAME_STATES {
	STATE_PONG=0,
	STATE_PICK_ITEM=1,
	STATE_PAUSE=2,
	STATE_MENU=3,
	STATE_VICTORY=4,
};

// How the paddle is controlled
//enum PaddleBrain {
//}

/** Objects **/
typedef struct PaddleControls {
	int left;
	int right;
	int dash;
	int up;
	int down;
	int item;
} PaddleControls;

// Paddle
typedef enum PaddleBrain {
	PB_PLAYER=0,
	PB_CLONE=1, // Clones created by cloning vat
	PB_COMPUTER=2, // Computer controlled player paddle
} PaddleBrain;

typedef struct PaddleData {
	Vector2 pos;
	Vector2 dir; // Actually just the signs for speed so speed calculations can be positive only
	//Vector2 desired_vel; // This is manipulated by paddle controls
	Vector2 facing; // What direction we're facing. Yeah I know it's a rectangle
	int id; // 2 owns the bottom of the screen
	PaddleBrain brain;
	bool delete_me;
	float invincibility_timer; // Paddle flashes intermittently when this timer is > 0?
	float hit_timer; // Paddle tinted red when hit timer is active

	//float speed;
	Vector2 speed;
	// Max normal speed while not dashing
	Vector2 max_speed; 

	float destroyed_timer;

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

	float mind_control_timer; // While above 0, the paddles are flipped

	// How long the sword hitbox lasts
	float sword_timer;
	float sword_anim_timer;
	int sword_frame;
	int sword_anim_dir; // -1 if back, 1 if forward

	//bool cv_clone; // Clone paddles are destroyed when the ball hits them or their destroyed timer goes up by one
	struct PaddleData *cv_creator;
	int cv_lag_dist;
	int cv_health;

	bool bmcm_flipped; // false if not flipped, true if flipped
	float bmcm_timer; // When this runs out, mind control stops

} PaddleData;


enum BallState {
	BS_NORMAL, // Bounces and moves as normal
	BS_KNUCKLEBALL, // Moves funny
	BS_DESTROYED, // Doesn't update at all
};

typedef struct BallData {
	Vector2 pos;
	int radius;
	// Actually the unit vector direction of velocity
	Vector2 vel; 
	int speed; 
	bool delete_me;
	Color color;
	// Pointer to paddle that last hit the ball. WARN: will be -1 if the ball has not been hit yet
	PaddleData *last_hit_by;
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

	float rs_spiked_speed_mult;

	float hp_timer;

	float ym_gravity_turn_speed;
	Vector2 ym_gravity_center;

	float mm_speed_bonus;
	bool mutant_moused; // If true, the ball will accrue speed
	
} BallData;

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
	float detonation_timer; // If something sets this to higher than 0, the missile will blow up when it hits 0

	bool delete_me; // If true, this rocket will be deleted this frame
	Vector2 ym_target;
	int ym_turn_speed;
} RocketData;

typedef struct Explosion {
	Vector2 pos;
	float radius;
	float active_timer;
	float anim_timer;
	int anim_frame;
	//float life_timer; // the clock is always ticking. Unless you comment it out. Lol
	bool delete_me;
} Explosion;

typedef struct PlayerData {
	int num_points;
	PaddleData *paddle;
	struct PaddleControls controls;
} PlayerData;





/** Util **/
typedef struct Circle {
	float x;
	float y;
	float radius;
} Circle;






/** States **/
typedef struct PongState {
	int current_round;
	float ball_respawn_timer; // Time that delays the ball respawn
	float end_round_timer;
	float impact_timer; // If > 0, the game will freeze. Don't set this very high
	struct PaddleData paddles[MAX_PADDLES]; // [0] is controlled by p1, [1] is controlled by p2, all others are free
	int num_paddles;
	int num_balls;
	struct BallData balls[MAX_BALLS]; // Up to many balls!
	RocketData rockets[8]; // Swapback array of rockets
	int num_rockets;
	Explosion explosions[MAX_EXPLOSIONS];
	int num_explosions;

	float score_timer;

	float fuck_you_timer;
	char fuck_you_text[128];
	int fuck_you_size;
	int points_to_win;

	Decoration decorations[MAX_DECORATIONS];
	int num_decorations;
} PongState;

struct PickItemsState {
	float item_anim_timers[8];
	int item_anim_frames[8];
	//struct Animation item_animations[8];
	int item_choices[8]; // 3 by default but can do up
	int num_item_choices; // 3 by default
	int hovered_item; // Idx of the hovered item
	//PaddleData *choosing_paddle;
	struct PlayerData *choosing_player;
};

// Menu
typedef struct MenuButton {
	Rectangle dims;
} MenuButton;

typedef enum {
	MENU_MAIN=0,
	MENU_CONTROLS=1,
} MENU;

typedef enum {
	BTN_PLAY,
	BTN_QUIT,
} BUTTON;

typedef struct Button {
	Rectangle dims;
	Color color;
	const char *text;
	BUTTON id;
} Button;

typedef struct MenuState {
	Button *current_menu;
	int num_buttons;
	Button *hovered; // Often NULL

	Button main_buttons[32];
	int num_main_buttons;
	
	int ui_idx;

	float bar_progress;
} MenuState;

// Victory
typedef struct VictoryState {
	struct PlayerData *victor;
	int hovered_option;
	bool deciding;
	float flash_timer;
} VictoryState;


typedef struct WorldBorders {
	float top;
	float bottom;
	float left;
	float right;
} WorldBorders;

enum MusicMode {
	MM_PLAYLIST,
	MM_REPEAT
};

typedef struct MusicMind {
	enum MusicMode mode;
	Music *playing;
	Music *fade;
	int track_idx;

	//float volume[2]; // 0 is playing, 1 is fade
	float fade_timer;
	// Used to figure out how far we are to 0 volume
	float fade_timer_start;
} MusicMind;

typedef struct GameState {
	int state; // GAME_STATE

	float hitstop_timer;

	bool quit_please; // Allows access to quitting the window from anywhere
	struct PlayerData *player1;
	struct PlayerData *player2;
	struct PongState *pong_state;
	struct PickItemsState *pick_items_state;
	MenuState *menu_state;
	VictoryState *victory_state;
	Camera2D *camera;
	MusicMind *music_mind;
	float screenshake_timer;
	float screenshake_freq_timer;
	float screenshake_decay; // Screenshake will shake at amplitude until the timer runs out, then amplitude will reduce by decay until it reaches 0
	float screenshake_amplitude; // How much to shake by
} GameState;



#endif


