#include "structs.h"

void change_state(enum GAME_STATES new_state, struct GameState *state);
void change_state_to_pick_items(struct GameState *state, struct PaddleData *choosing);
void change_state_to_pong(struct GameState *state);
