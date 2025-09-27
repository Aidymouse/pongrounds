/** Object fns */

#include "structs.h"

/** Paddle **/
void paddle_refresh(struct PaddleData *p, struct PaddleData *opponent, struct GameState *state);
void paddle_move(float dt, struct PaddleData *p, struct PaddleControls controls, struct GameState *state);
void paddle_swing_sword(struct PaddleData *p);

/** Ball **/
void ball_move(float dt, struct BallData *ball);
void ball_respawn(struct BallData *b);
void ball_reflect_wall(struct BallData *b);
void ball_paddle_hit(struct BallData *b, struct PaddleData *p);
void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent);
void ball_draw(struct BallData *ball, bool debug);
void ball_sword_hit(struct BallData *b, struct PongState *pong_state);
void ball_refresh(struct BallData *ball);

/** Sword **/
void sword_swing(float dt, struct PaddleData *p);
void sword_draw(struct PaddleData *p, bool debug);
