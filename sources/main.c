#include <stdio.h>

#include "string.h"
#include "states/pong.h"
#include "states/pick_item.h"

#include "anim.h"
#include "textures.h"

#include "defines.h"
#include "controls.h"
#include "items.h"

#include "obj/obj.h"
#include "raylib.h"
#include "structs.h"
#include "Vec2.h"
#include "helper.h"

// INIT FNS //
void init_gamestate(struct GameState *gs) {
	gs->state = PONG;

	gs->screenshake_timer = 0;
	gs->screenshake_amplitude = 2;
	gs->screenshake_decay = DEFAULT_SCREENSHAKE_DECAY;
}


void init_player(struct PlayerData *player) {
	//player->paddle = paddle;
}


void init_pong_state(struct PongState *g) {
	g->current_round = 0;
	g->num_balls = 1;
	ball_init(&(g->balls[0]));
	//init_ball(&(g->balls[1]));
	g->ball_respawn_timer = 0;
	g->end_round_timer = 0;
	g->num_rockets = 0;
	g->num_explosions = 0;
	g->num_paddles = 2;
	g->fuck_you_timer = 0;
	g->fuck_you_idx = 0;
}

void init_pick_items_state(struct PickItemsState *s) {
	s->num_item_choices = 3;
}

Vector2 center = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };

void init_camera(Camera2D *camera) {
	camera->target = center;
	camera->offset = center;
	camera->zoom = 1.0f;
	camera->rotation = 0.0;
}

void update_screenshake (float dt, struct GameState *state) {
	// Screenshake
	if (state->screenshake_timer > 0 || state->screenshake_amplitude > 0) {
		
		state->camera->offset = Vec2Add(center, (Vector2){
			randInt(-state->screenshake_amplitude, state->screenshake_amplitude),
			randInt(-state->screenshake_amplitude, state->screenshake_amplitude),
		});

		state->screenshake_timer -= dt;

		if (state->screenshake_timer <= 0) {

			state->screenshake_amplitude -= state->screenshake_decay;
			if (state->screenshake_amplitude <= 0 || state->screenshake_decay <= 0) {
				state->camera->offset = center;
				state->screenshake_amplitude = 0;
			}
		}
	}
}

struct PaddleControls p1_controls = { P1_LEFT_KEY, P1_RIGHT_KEY, P1_DASH_KEY, P1_UP_KEY, P1_DOWN_KEY, P1_ITEM_KEY };
struct PaddleControls p2_controls = { P2_LEFT_KEY, P2_RIGHT_KEY, P2_DASH_KEY, P2_UP_KEY, P2_DOWN_KEY, P2_ITEM_KEY };

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

	paddle_init( &pong_state.paddles[0] );
	struct PaddleData *p1 = &pong_state.paddles[0];
	p1->id = 1;
	p1->pos.y = 40;
	p1->color = BLUE;
	p1->facing = (Vector2){ 0, 1 };

	paddle_init( &pong_state.paddles[1] );
	struct PaddleData *p2 = &pong_state.paddles[1];
	p2->id = 2;
	p2->pos.y = SCREEN_HEIGHT - p2->paddle_thickness - 40;
	p2->color = ORANGE;
	p2->facing = (Vector2){ 0, -1 };

	struct PickItemsState pick_items_state;
	init_pick_items_state(&pick_items_state);

 	
	// Player 1
	struct PlayerData player1;
	init_player(&player1);
	player1.paddle = &pong_state.paddles[0];
	player1.controls = p1_controls;

	// Player 2
	struct PlayerData player2;
	init_player(&player2);
	player2.paddle = &pong_state.paddles[1];
	player2.controls = p2_controls;

	// DEBUG: hard code in some items
	int debug_item = ITEM_NUCLEAR_LAUNCH_CODES;
	p2->items[debug_item] += 1;
	p2->items_total[debug_item] += 1;
	p1->items[debug_item] += 1;
	p1->items_total[debug_item] += 1;
	// /DEBUG

	struct BallData ball;
	ball_init(&ball);

	state.player1 = &player1;
	state.player2 = &player2;
	state.pong_state = &pong_state;
	state.pick_items_state = &pick_items_state;
	state.camera = &camera;

	float dt = 0;

	unsigned int b = 0x12110f;
	Color bg = GetColor(b);
	
	// Game loop //
    while (!WindowShouldClose())
    {
		// TODO how to reset this on the first frame cos it takes ages to load the textures
		dt = GetFrameTime();

		// Update
		if (state.state == PONG) {
			update_screenshake(dt, &state);
			state_pong(dt, &state);
		} else if (state.state == PICK_ITEM) {
			state_pick_items(dt, state.pick_items_state, &state);
		}
	


		// Draw
        BeginDrawing();
        ClearBackground(bg);

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
