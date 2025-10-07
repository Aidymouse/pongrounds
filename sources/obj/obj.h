/** Object fns */

#include "structs.h"
#include "stdlib.h"

/** Paddle **/
void paddle_refresh(PaddleData *p, PaddleData *opponent, struct GameState *state);
void paddle_move(float dt, PaddleData *p, struct PaddleControls controls, struct GameState *state);
void paddle_activate_items(float dt, PaddleData *p, struct PongState *pong_state);
void paddle_update(float dt, PaddleData *p);
Rectangle paddle_get_rect(PaddleData *p);
Rectangle paddle_get_time_influence_area(PaddleData *paddle);
float paddle_get_time_power(PaddleData *paddle);
Rectangle paddle_get_russian_secrets_rect(PaddleData *paddle);

/** Ball **/
void ball_move(float dt, struct BallData *ball, struct GameState *state);
void ball_respawn(struct BallData *b);
void ball_reflect_wall(struct BallData *b);
void ball_paddle_hit(struct BallData *b, PaddleData *p);
void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent);
void ball_draw(struct BallData *ball, bool debug);
void ball_sword_hit(struct BallData *b, struct PongState *pong_state);
void ball_refresh(struct BallData *ball);
void ball_check_collisions(struct BallData *ball, struct GameState *state, WorldBorders world_borders);

/** Sword **/
void sword_swing(float dt, PaddleData *p);
void sword_draw(PaddleData *p, bool debug);
Rectangle sword_get_hitbox(PaddleData *p);

/** Rocket **/
void rocket_init(RocketData *r, PaddleData *spawner);
void rocket_fly(float df, RocketData *r);
void rocket_check_collisions(RocketData *r, WorldBorders borders, struct GameState *state);
void rocket_draw(RocketData *r, bool debug);
void rocket_cleanup(struct PongState *pong_state);

/** Explosion **/
void explosion_init(Explosion *e);
void explosion_update(float dt, Explosion *e);
void explosion_spawn(Vector2 pos, struct PongState *pong_state);
void explosion_draw(Explosion *e, bool debug);
void explosion_cleanup(struct PongState *pong_state);
