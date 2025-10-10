
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Prounds"

// Game Vars
#define POINTS_PER_ROUND (2)
#define NUM_ROUNDS (7)
#define DEFAULT_SCREENSHAKE_DECAY 1

#define NUM_ITEMS 13
#define MAX_BALLS 32

// Animations
#define CARD_ANIM_FRAMES 43
#define FRAME_TIME 0.016
#define SMALL_ITEM_DIMS_PX 85
#define ITEM_USE_BUMP_TIME (0.3) // Time an item is highlighted for if it is used once

// Ball
#define BALL_INIT_SPEED (400) 
#define BALL_SCORE_DAMAGE (50) // Damage against opponent when you score against them
#define BALL_RESPAWN_DELAY (1) // seconds after score that the ball respawns
#define BALL_RADIUS (6)

// Paddles
#define PADDLE_DEFAULT_WIDTH (100)
#define PADDLE_DEFAULT_HP (100)
#define PADDLE_SPEED (400)

// Sword
#define SWORD_DURATION (0.2)
#define SWORD_COOLDOWN (2)
#define SWORD_TEX_DIMS_PX (128)
#define NUM_SWORD_FRAMES (31)
#define SWORD_FRAME_TIME 0.016

// Niekro
#define NIEKRO_KB_TURN_SPEED 100

// Time Wizards Chronometer
#define CHRONOMETER_SIZE (200)
#define BASE_TIME_POWER 0.7
#define CUMULATIVE_TIME_POWER 0.95 // Additional multiplier on speed for each additional chronometer

// Rockets
#define HANG_TIME (0.5)
#define ROCKET_INIT_SPEED (200) // Speed rocket is thrust out of the paddle with
#define ROCKET_FLY_INIT_SPEED (400) // Speed the rocket becomes when its thrusters activate
#define ROCKET_ACCELERATION (20)
#define GRAVITY (9.8) // Missile falls due to gravity while it's hanging
#define MISSILE_TEX_DIMS_PX (128)
#define MAX_ROCKETS (8)
#define ROCKET_TURN_SPEED (5) // Speed at which rocket tries to correct to going straight up
#define MISSILE_DEATH_TIME (1)

// Explosions
#define MAX_EXPLOSIONS 8
#define EXPLOSION_RADIUS 30
#define EXPLOSION_FRAMES 13
#define EXPLOSION_TEX_DIMS_PX (128)

// Russian Secrets
#define RUSSIAN_SECRETS_AREA_WIDTH 5
#define RUSSIAN_SECRETS_AREA_ADDITIONAL 2
#define RUSSIAN_SECRETS_SPEED_MULT 3
#define RUSSIAN_SECRETS_SPEED_ADDITIONAL 0.5
#define RS_SPIKE_DECELLERATION 0.2

// Hydraulic Press
#define HYDRAULIC_PRESS_CHANCE 10
#define HYDRAULIC_PRESS_CHANCE_ADDITIONAL 5
#define HYDRAULIC_PRESS_TIMER 0.5
#define HYDRAULIC_PRESS_NUM_BALLS 5
