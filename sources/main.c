#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "structs.h"
#include "Vec2.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "Pong but its rounds"

// TODO: these should be user defined somewhere
#define P1_UP_KEY 87 // W Key
#define P1_DOWN_KEY 83 // S Key
#define P2_UP_KEY 265 // Up Arrow
#define P2_DOWN_KEY 264 // Down Arrow

int randInt(int lower, int upper) {
	return rand() % (upper - lower + 1) + lower;
}


// INIT FNS //
void init_paddle(struct PaddleData *p) {
	p->pos.x = 0;
	p->pos.y = 0;
	p->paddle_width = 100;
	p->paddle_thickness = 20;
	p->color = WHITE;
}

void init_ball(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->radius = 10;
	b->vel.x = 5;
	b->vel.y = 0;

}

void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.x = -b->vel.x;
	if (b->vel.x < 0) { b->vel.x = -5; } else { b->vel.x = 5; }
	// Vel y ?
}

void ball_paddle_hit(struct BallData *b) {
	b->vel.x = -b->vel.x;
	if (b->vel.x < 0) {
		b->vel.x -= 0.2;
	} else {
		b->vel.x += 0.2;
	}

	//b->vel.y = randInt(-40, 40) / 10;
}


// MAIN //
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    Texture2D texture = LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works

	struct PlayerData player1;
	player1.score = 0;
	struct PlayerData player2;
	player2.score = 0;

	struct PaddleData p1;
	init_paddle(&p1);
	p1.pos.x = 20;
	p1.pos.y = SCREEN_HEIGHT/2 - p1.paddle_width/2;

	struct PaddleData p2;
	init_paddle(&p2);
	p2.pos.x = SCREEN_WIDTH - p2.paddle_thickness - 20;
	p2.pos.y = SCREEN_HEIGHT/2 - p2.paddle_width/2;

	struct BallData ball;
	init_ball(&ball);
	

    while (!WindowShouldClose())
    {
		// Update //
		ball.pos = Vec2Add(ball.pos, ball.vel);

		/*
		int pressed = GetKeyPressed();
		while (pressed != 0) {
			printf("Key pressed: %d\n", pressed);
			pressed = GetKeyPressed();
		}
		*/

		// Collisions
		if (ball.pos.y - ball.radius <= 0) {
			ball.pos.y = ball.radius;
			ball.vel.y = -ball.vel.y;
		} else if (ball.pos.y + ball.radius >= SCREEN_HEIGHT) {
			ball.pos.y = SCREEN_HEIGHT - ball.radius;
			ball.vel.y = -ball.vel.y;
		}

		struct Rectangle p1Rect = {p1.pos.x, p1.pos.y, p1.paddle_thickness, p1.paddle_width};
		if (CheckCollisionCircleRec(ball.pos, ball.radius, p1Rect)) {
			ball_paddle_hit(&ball);
		}

		struct Rectangle p2Rect = {p2.pos.x, p2.pos.y, p2.paddle_thickness, p2.paddle_width};
		if (CheckCollisionCircleRec(ball.pos, ball.radius, p2Rect)) {
			ball_paddle_hit(&ball);
		}

		// Scoring
		if (ball.pos.x > SCREEN_WIDTH) {
			player1.score += 1;
			ball_respawn(&ball);
		} else if (ball.pos.x < 0) {
			player2.score += 1;
			ball_respawn(&ball);
		}

		// Player control
		if (IsKeyDown(P1_UP_KEY)) {
			p1.pos.y -= 5;
		} else if (IsKeyDown(P1_DOWN_KEY)) {
			p1.pos.y += 5;
		}

		if (IsKeyDown(P2_UP_KEY)) {
			p2.pos.y -= 5;
		} else if (IsKeyDown(P2_DOWN_KEY)) {
			p2.pos.y += 5;
		}

		// Draw //
        BeginDrawing();

        ClearBackground(BLACK);

		// Draw Paddle One
		DrawRectangle(p1.pos.x, p1.pos.y, p1.paddle_thickness, p1.paddle_width, p2.color);

		// Draw Paddle Two
		DrawRectangle(p2.pos.x, p2.pos.y, p2.paddle_thickness, p2.paddle_width, p2.color);
	
		// BALL
		DrawCircle(ball.pos.x, ball.pos.y, ball.radius, WHITE);

		char p1score[9];
		sprintf(p1score, "%d", player1.score);
		DrawText(p1score, 10, 10, 20, WHITE);

		char p2score[9];
		sprintf(p2score, "%d", player2.score);
		DrawText(p2score, SCREEN_WIDTH-10, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
