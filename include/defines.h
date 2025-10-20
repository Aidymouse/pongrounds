
#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (600)

#define WINDOW_TITLE "Prounds"
#define HACKER_GREEN (Color){95, 247, 158, 255}
#define BG_COLOR 0x12110fff

// Music
#define NUM_TRACKS 3

// State Vars
#define NUM_ROUNDS (10) // a.k.a points required to win
#define DEFAULT_SCREENSHAKE_DECAY 1
#define FUCK_YOU_DURATION 0.5

#define NUM_ITEMS 19
#define MAX_BALLS 32
#define MAX_PADDLES 32

// Menu State
#define MENU_SPEED 10
#define MENU_LINE_DIST 10

// Victory State
#define VICTORY_FLASH_DURATION 5

// Pong State
#define SCORE_TIMER 1.5

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
#define DAMAGE_INVINCIBILITY_TIME 2
#define PADDLE_DECELLERATION 50 //50
#define PADDLE_TURN_SPEED 270

// Hypergonadism
#define HYPERGONADISM_WIDTH_BONUS 20

// Chastity Cage
#define CHASTITY_CAGE_WIDTH_PENALTY 20

// Bachelor of Psychology (Hons)
#define BACHELOR_DIST_FROM_CENTER 80 // Distance from center of screen you can be 
#define BACHELOR_ADDITIONAL_DISTANCE 40 // Ammount of extra distance you can walk with additional bachelors

// Cherry blossom cloak
#define CLOAK_DASH_SPEED (2200)
#define CLOAK_DASH_SPEED_BONUS (200) // Additional
#define CLOAK_DECELLERATION (250)

// Sword
#define SWORD_DURATION (0.2)
#define SWORD_COOLDOWN (2)
#define SWORD_TEX_DIMS_PX (128)
#define NUM_SWORD_FRAMES (31)
#define SWORD_FRAME_TIME 0.016
#define SWORD_DAMAGE 40

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
#define MISSILE_SHAKE_DUR (0.5) // Missile screenshake duration
#define MISSILE_SHAKE_AMPL (2) // missile screenshake amplitude
#define MISSILE_COOLDOWN (2)

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
#define HYDRAULIC_PRESS_CHANCE 20
#define HYDRAULIC_PRESS_CHANCE_ADDITIONAL 5
#define HYDRAULIC_PRESS_TIMER 0.5
#define HYDRAULIC_PRESS_NUM_BALLS 5
#define HYDRAULIC_DIVISOR 3

// Cloning vat
#define CV_DEFAULT_LAG_DISTANCE 100
#define CV_CLONE_SPEED_MULT 0.8

// Snotch
#define SNOTCH_CHANCE 20
#define SNOTCH_CHANCE_ADDITIONAL 20

// Yo momma
#define YM_RADIUS 120 // gravity strength is always 100% if the ball is in the gravity zone
#define YM_BASE_STRENGTH 100 // turn speed (deg/sec) toward gravitational center?

// Underwear (used)

// Mutant Mouse
#define MUTANT_MOUSE_ACCEL 5
