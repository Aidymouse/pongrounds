#include <stdio.h>

#include "states/pong.h"
#include "states/pick_item.h"

#include "defines.h"

#include "raylib.h"
#include "structs.h"
#include "Vec2.h"



// INIT FNS //
void init_gamestate(struct GameState *gs) {
	gs->state = PONG;
}

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
	b->vel.y = BALL_INIT_SPEED;
	b->vel.x = 0;
}

void init_pong_state(struct PongState *g) {
	g->current_round = 0;
}

void init_pick_items_state(struct PickItemsState *s) {
	s->num_item_choices = 4;
}



// MAIN //
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works

	enum GAME_STATES game_state = PONG;

	struct GameState state;
	init_gamestate(&state);

	struct PongState pong_state;
	init_pong_state(&pong_state);

	struct PickItemsState pick_items_state;
	init_pick_items_state(&pick_items_state);

	struct PaddleData p1;
	struct PlayerData player1;
	struct PaddleData p2;
	struct PlayerData player2;
 	
	init_player(&player1, &p1);
	init_player(&player2, &p2);

	//p2.items_total[0] += 2;

	//refresh_paddle(&p2);

	p2.pos.y = SCREEN_HEIGHT - p2.paddle_thickness - 40;

	struct BallData ball;
	init_ball(&ball);

	state.player1 = &player1;
	state.player2 = &player2;
	state.ball = &ball;
	state.pong_state = &pong_state;
	state.pick_items_state = &pick_items_state;

	float dt = 0;

    while (!WindowShouldClose())
    {
		dt = GetFrameTime();

		// Update
		if (state.state == PONG) {
			state_pong(dt, &state);
		}

		// Draw
        BeginDrawing();
        ClearBackground(BLACK);

		if (state.state == PONG) {
			draw_pong(&state);
		} else if (state.state == PICK_ITEM) {
			draw_pong(&state); // Still in background
			draw_pick_items(state.pick_items_state);
		}

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
