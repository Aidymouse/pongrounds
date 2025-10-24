#include <stdio.h>
#include <time.h>

#include "string.h"
#include "states/states.h"
#include "states/pong.h"
#include "states/pick_item.h"

#include "anim.h"
#include "textures.h"
#include "audio.h"

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
	gs->state = STATE_MENU;
	gs->quit_please = false;
	gs->screenshake_timer = 0;
	gs->screenshake_amplitude = 2;
	gs->screenshake_decay = DEFAULT_SCREENSHAKE_DECAY;
	gs->hitstop_timer = 0;
}





Vector2 center = (Vector2){ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };

void init_camera(Camera2D *camera) {
	camera->target = center;
	camera->offset = center;
	camera->zoom = 1.0f;
	camera->rotation = 0.0;
}

// Misc
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

// TODO: load controls from file
struct PaddleControls p1_controls = { P1_LEFT_KEY, P1_RIGHT_KEY, P1_DASH_KEY, P1_UP_KEY, P1_DOWN_KEY, P1_ITEM_KEY };
struct PaddleControls p2_controls = { P2_LEFT_KEY, P2_RIGHT_KEY, P2_DASH_KEY, P2_UP_KEY, P2_DOWN_KEY, P2_ITEM_KEY };

// MAIN //
int main(void)
{

	srand(time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	InitAudioDevice();
    SetTargetFPS(60);


	// Loading Screen
	BeginDrawing();
	DrawText("Loading...", SCREEN_HEIGHT/2, SCREEN_WIDTH/2, 50, HACKER_GREEN);
	EndDrawing();


	// Load Textures //
	// TODO: make this a program state
	load_textures();
	load_audio();

	// Init State //
	enum GAME_STATES game_state = STATE_MENU;

	Camera2D camera;
	init_camera(&camera);

	MusicMind music_mind;
	init_music_mind(&music_mind);
	
	struct GameState state;
	init_gamestate(&state);

	struct PongState pong_state;
	init_state_pong(&pong_state);

	MenuState menu_state;
	init_state_menu(&menu_state);

	VictoryState victory_state;
	init_state_victory(&victory_state);

	paddle_init( &pong_state.paddles[0] );
	struct PaddleData *p1 = &pong_state.paddles[0];
	p1->id = 1;
	p1->pos.y = 40;
	p1->color = BLUE;
	p1->facing = (Vector2){ 0, 1 };

	paddle_init( &pong_state.paddles[1] );
	struct PaddleData *p2 = &pong_state.paddles[1];
	p2->id = 2;
	p2->pos.y = SCREEN_HEIGHT - (p2->paddle_thickness - 40);
	p2->color = ORANGE;
	p2->facing = (Vector2){ 0, -1 };

	struct PickItemsState pick_items_state;
	init_state_pick_items(&pick_items_state);

 	
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

	/* DEBUG: hard code in some items
	*/
	int debug_item = ITEM_CEREMONIAL_SWORD;
	p2->items[debug_item] += 1;
	p2->items_total[debug_item] += 1;
	debug_item = ITEM_CLONING_VAT;
	p2->items[debug_item] += 1;
	p2->items_total[debug_item] += 1;
	//debug_item = ITEM_BROKEN_MIND_CONTROL_MACHINE;
	p1->items[debug_item] += 1;
	p1->items_total[debug_item] += 1;
	// /DEBUG

	struct BallData ball;
	ball_init(&ball);

	state.player1 = &player1;
	state.player2 = &player2;
	state.pong_state = &pong_state;
	state.pick_items_state = &pick_items_state;
	state.menu_state = &menu_state;
	state.victory_state = &victory_state;
	state.music_mind = &music_mind;
	state.camera = &camera;

	float dt = 0;

	unsigned int b = BG_COLOR;
	Color bg = GetColor(b);

	// Start theme plaing
	queue_track(&music_mind, &msc_theme, 0);
	
	// Game loop //
    while (!WindowShouldClose() && !state.quit_please)
    {
		// TODO how to reset this on the first frame cos it takes ages to load the textures
		dt = GetFrameTime();

		// Update
		music_mind_update(dt, &music_mind);

		if (state.hitstop_timer > 0) {
			state.hitstop_timer -= dt;
		} else {
			if (state.state == STATE_PONG) {
				update_screenshake(dt, &state);
				state_pong(dt, &state);
			} else if (state.state == STATE_PICK_ITEM) {
				state_pick_items(dt, state.pick_items_state, &state);
			} else if (state.state == STATE_MENU) {
				state_menu(dt, &state);
			} else if (state.state == STATE_VICTORY) {
				state_victory(dt, &state);
			}
		}
	


		// Draw
        BeginDrawing();
        ClearBackground(bg);


		if (state.state == STATE_PONG) {
			draw_pong(&state);
		} else if (state.state == STATE_PICK_ITEM) {
			draw_pong(&state); // Still in background
			draw_pick_items(state.pick_items_state, tex_item_cards);
		} else if (state.state == STATE_MENU) {
			draw_menu(state.menu_state);
	 	} else if (state.state == STATE_VICTORY) {
			draw_pong(&state); // Still in background
			draw_victory(state.victory_state);
		}

		// Not sure bout this
		//DrawTexture(scanlines, 0, 0, ColorAlpha(WHITE, 0.2));

        EndDrawing();
    }

	// Unload textures //
	unload_textures();
	unload_audio();

	CloseAudioDevice();
    CloseWindow();

    return 0;
}
