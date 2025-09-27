#include "structs.h"

void state_pong(float dt, struct GameState *state); 
void draw_pong(struct GameState *state); 

void refresh_paddle(struct PaddleData *p, struct PaddleData *opponent, struct GameState *state); // This is here because we need to call it on state change

