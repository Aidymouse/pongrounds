#include "structs.h"

// TODO: these should be user defined somewhere
#define P2_LEFT_KEY KEY_A // A Key
#define P2_RIGHT_KEY KEY_D // D Key
#define P2_DOWN_KEY KEY_S
#define P2_DASH_KEY KEY_SPACE
#define P2_UP_KEY KEY_W
#define P2_ITEM_KEY KEY_LEFT_SHIFT

#define P1_LEFT_KEY KEY_LEFT 
#define P1_RIGHT_KEY KEY_RIGHT 
#define P1_DASH_KEY KEY_O
#define P1_ITEM_KEY KEY_P
#define P1_UP_KEY KEY_UP 
#define P1_DOWN_KEY KEY_DOWN

void load_controls(PlayerData *p1, PlayerData *p2);

void remap_control();
