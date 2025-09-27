
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Prounds"

// Game Vars
#define POINTS_PER_ROUND (2)
#define NUM_ROUNDS (7)

#define NUM_ITEMS 5

#define BALL_INIT_SPEED (200) // usually 200
#define DEFAULT_PADDLE_WIDTH (100)
#define DEFAULT_PADDLE_HP (100)

// Pong
#define BALL_SCORE_DAMAGE (50) // Damage against opponent when you score against them
#define BALL_RESPAWN_DELAY (1) // seconds after score that the ball respawns

// TODO: these should be user defined somewhere
#define P1_LEFT_KEY 65 // A Key
#define P1_RIGHT_KEY 68 // D Key
#define P1_DASH_KEY KEY_SPACE
#define P2_LEFT_KEY 263 // Left Arrow
#define P2_RIGHT_KEY 262 // Right Arrow
#define P2_DASH_KEY KEY_LEFT_SHIFT
