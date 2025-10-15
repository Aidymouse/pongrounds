#include "structs.h"

void change_state(enum GAME_STATES new_state, struct GameState *state);
void change_state_to_pick_items(struct GameState *state, PaddleData *choosing);
void change_state_to_pong(struct GameState *state);
void change_state_to_victory(struct GameState *state, struct PlayerData *victor);
void change_state_to_menu(struct GameState *state);
