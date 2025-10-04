#include <stdio.h>

#include "string.h"
#include "states/pong.h"
#include "states/pick_item.h"

#include "anim.h"
#include "textures.h"

#include "defines.h"
#include "items.h"

#include "raylib.h"
#include "structs.h"
#include "Vec2.h"
#include "helper.h"



// INIT FNS //
void init_gamestate(struct GameState *gs) {
	gs->state = PONG;
}

void init_paddle(struct PaddleData *p) {
	p->pos.x = SCREEN_WIDTH/2 - PADDLE_DEFAULT_WIDTH/2;
	p->paddle_width = PADDLE_DEFAULT_WIDTH;
	p->paddle_thickness = 10;
	p->color = WHITE;
	p->hp = PADDLE_DEFAULT_HP;
	p->max_hp = PADDLE_DEFAULT_HP;
	p->vel.x = 0;
	p->vel.y = 0;
	p->speed = PADDLE_SPEED;
	p->max_speed = PADDLE_SPEED;

	for (int i=0; i<NUM_ITEMS; i++) {
		p->items[i] = 0;
		p->items_total[i] = 0;
		p->item_cooldown_timers[i] = 0;
		p->item_use_timers[i] = 0;
	} 

}


void init_player(struct PlayerData *player, struct PaddleData *paddle) {
	player->paddle = paddle;
	init_paddle(player->paddle);
}

void init_ball(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->radius = 6;
	b->vel.y = 1;
	b->vel.x = 0;
	b->speed = BALL_INIT_SPEED;
	b->state = BS_NORMAL;
	b->score_damage = BALL_SCORE_DAMAGE;
	b->destroyed = false;

	b->kb_turn_speed = 100;
	b->last_hit_by = 0; // equiv to NULL

}

void init_pong_state(struct PongState *g) {
	g->current_round = 0;
	g->num_balls = 1;
	init_ball(&(g->balls[0]));
	//init_ball(&(g->balls[1]));
	g->ball_respawn_timer = 0;
	g->num_rockets = 0;
	
}

void init_pick_items_state(struct PickItemsState *s) {
	s->num_item_choices = 3;
}

void init_camera(Camera2D *camera) {
	camera->target = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
	camera->offset = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };
	camera->zoom = 1.0f;
	camera->rotation = 0.0;
}

// MAIN //
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);


	// Load Textures //
	load_textures();

	// Init State //
	enum GAME_STATES game_state = PONG;

	Camera2D camera;
	init_camera(&camera);

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

	p1.pos.y = 40;
	p1.color = BLUE;
	p1.id = 1;

	p2.pos.y = SCREEN_HEIGHT - p2.paddle_thickness - 40;
	p2.color = ORANGE;
	p2.id = 2;

	// DEBUG: hard code in some items
	int debug = ITEM_CEREMONIAL_SWORD;
	p2.items[debug] += 1;
	p2.items_total[debug] += 1;
	p1.items[debug] += 1;
	p1.items_total[debug] += 1;

	struct BallData ball;
	init_ball(&ball);

	state.player1 = &player1;
	state.player2 = &player2;
	state.pong_state = &pong_state;
	state.pick_items_state = &pick_items_state;
	state.camera = &camera;

	float dt = 0;

	// Game loop //
    while (!WindowShouldClose())
    {
		dt = GetFrameTime();

		// Update
		if (state.state == PONG) {
			state_pong(dt, &state);
		} else if (state.state == PICK_ITEM) {
			state_pick_items(dt, state.pick_items_state, &state);
		}

		// Draw
        BeginDrawing();
        ClearBackground(BLACK);

		if (state.state == PONG) {
			draw_pong(&state);
		} else if (state.state == PICK_ITEM) {
			draw_pong(&state); // Still in background
			draw_pick_items(state.pick_items_state, tex_item_cards);
		}

		// Not sure bout this
		//DrawTexture(scanlines, 0, 0, ColorAlpha(WHITE, 0.2));

        EndDrawing();
    }

	// Unload textures //
	unload_textures();

    CloseWindow();

    return 0;
}
