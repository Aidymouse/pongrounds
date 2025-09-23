#include "structs.h"

void state_pong(float dt, struct BallData *ball, struct PlayerData *player1, struct PlayerData *player2, struct GameState *state); 
void draw_pong(struct BallData *ball, struct PlayerData *player1, struct PlayerData *player2, struct GameState *state); 

void refresh_paddle(struct PaddleData *p); // Won't need this here forever I think

