/** Object fns */

#include "structs.h"

void paddle_refresh(struct PaddleData *p, struct PaddleData *opponent, struct GameState *state);
void paddle_move(float dt, struct PaddleData *p, struct PaddleControls controls, struct GameState *state);

void ball_move(float dt, struct BallData *ball);
void ball_respawn(struct BallData *b);
void ball_reflect_wall(struct BallData *b);
void ball_paddle_hit(struct BallData *b, struct PaddleData *p);
void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent);
void ball_draw(struct BallData *ball, bool debug);
