#include <stdio.h>

#include "states/pong.h"

#include "defines.h"

#include "raylib.h"
#include "structs.h"
#include "Vec2.h"



// INIT FNS //
void init_paddle(struct PaddleData *p) {
	p->pos.x = SCREEN_WIDTH/2 - 50;
	p->pos.y = 40;
	p->paddle_width = 100;
	p->paddle_thickness = 20;
	p->color = WHITE;

	for (int i=0; i<16; i++) {
		p->items[i] = 0;
		p->items_total[i] = 0;
	} 

}


void init_player(struct PlayerData *player, struct PaddleData *paddle) {
	player->score = 0;
	player->paddle = paddle;
	init_paddle(player->paddle);
}

void init_ball(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->radius = 6;
	b->vel.y = 5;
	b->vel.x = 0;
}

void init_state(struct GameState *g) {
	g->state = 1;
	g->current_round = 0;
}



// MAIN //
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works

	enum GAME_STATES game_state = PONG;

	struct GameState state;
	init_state(&state);

	struct PaddleData p1;
	struct PlayerData player1;
	struct PaddleData p2;
	struct PlayerData player2;
 	
	init_player(&player1, &p1);
	init_player(&player2, &p2);

	p2.items_total[0] += 2;

	refresh_paddle(&p2);

	p2.pos.y = SCREEN_HEIGHT - p2.paddle_thickness - 40;

	struct BallData ball;
	init_ball(&ball);

    while (!WindowShouldClose())
    {
		state_pong(&ball, &player1, &player2, &state);
    }

    CloseWindow();

    return 0;
}
