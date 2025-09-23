#include "pong.h"

#include <stdio.h>
#include "defines.h"
#include "structs.h"

#include "Vec2.h"
#include "helper.h"

// Called on point score
void refresh_paddle(struct PaddleData *p) {

	for (int i=0; i<16; i++) {
		p->items[i] = p->items_total[i];
	}

}

void display_items(struct PaddleData *p, int x, int y) {
	for (int i=0; i<16; i++) {
		char s[16];
		//sprintf(p1score, "%d", player1.score);
		sprintf(s, "%d", p->items[i]);
		DrawText(s, x, y+i*10, 10, WHITE);
		
	}
}

// BALL //
void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	if (b->vel.y < 0) { b->vel.y = -100; } else { b->vel.y = 100; }
}

void ball_paddle_hit(struct BallData *b, struct PaddleData *p) {
	b->vel.y = -b->vel.y;
	if (b->vel.y < 0) {
		b->vel.y -= 20;
	} else {
		b->vel.y += 20;
	}

	b->vel.x = randInt(-300, 300);
}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer) {

	struct PaddleData *paddle = scorer->paddle;
	
	if (paddle->items[ITEM_EXPIRED_PANADOL] > 0) {
		paddle->items[ITEM_EXPIRED_PANADOL] -= 1;
		ball_paddle_hit(b, paddle);
		return;
	}

	scorer->score +=1;
	ball_respawn(b);
}


// PADDLE //
void paddle_move(float dt, struct PaddleData *p, int left_control, int right_control) {
		if (IsKeyDown(left_control)) {
			p->vel.x = -200;
		} else if (IsKeyDown(right_control)) {
			p->vel.x = 200;
		} else {
			p->vel.x = 0;
		}
		p->pos = Vec2Add(p->pos, Vec2MultScalar(p->vel, dt));
}

void state_pong(float dt, struct BallData *ball, struct PlayerData *player1, struct PlayerData *player2, struct GameState *state) {

		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		// Update //
		ball->pos = Vec2Add(ball->pos, Vec2MultScalar(ball->vel, dt));

		/*
		int pressed = GetKeyPressed();
		while (pressed != 0) {
			printf("Key pressed: %d\n", pressed);
			pressed = GetKeyPressed();
		}
		*/

		// Collisions
		if (ball->pos.x - ball->radius <= 0) {
			ball->pos.x = ball->radius;
			ball->vel.x = -ball->vel.x;
		} else if (ball->pos.x + ball->radius >= SCREEN_WIDTH) {
			ball->pos.x = SCREEN_WIDTH - ball->radius;
			ball->vel.x = -ball->vel.x;
		}

		struct Rectangle p1Rect = {p1->pos.x, p1->pos.y, p1->paddle_width, p1->paddle_thickness};
		if (CheckCollisionCircleRec(ball->pos, ball->radius, p1Rect)) {
			ball_paddle_hit(ball, p1);
		}

		struct Rectangle p2Rect = {p2->pos.x, p2->pos.y, p2->paddle_width, p2->paddle_thickness};
		if (CheckCollisionCircleRec(ball->pos, ball->radius, p2Rect)) {
			ball_paddle_hit(ball, p2);
		}

		// Scoring
		if (ball->pos.y > SCREEN_HEIGHT) {
			ball_score_hit(ball, player1);
		} else if (ball->pos.y < 0) {
			ball_score_hit(ball, player2);
		}

		// Player control
		paddle_move(dt, p1, P1_LEFT_KEY, P1_RIGHT_KEY);
		paddle_move(dt, p2, P2_LEFT_KEY, P2_RIGHT_KEY);

}

void draw_pong(struct BallData *ball, struct PlayerData *player1, struct PlayerData *player2, struct GameState *state) {

		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		// Draw Paddle One
		DrawRectangle(p1->pos.x, p1->pos.y, p1->paddle_width, p1->paddle_thickness, p2->color);

		// Draw Paddle Two
		DrawRectangle(p2->pos.x, p2->pos.y, p2->paddle_width, p2->paddle_thickness, p2->color);
	
		// BALL
		DrawCircle(ball->pos.x, ball->pos.y, ball->radius, WHITE);

		// Score
		char p1score[9];
		sprintf(p1score, "%d", player1->score);
		DrawText(p1score, 10, 10, 20, WHITE);
		display_items(p1, 20, 20);

		char p2score[9];
		sprintf(p2score, "%d", player2->score);
		DrawText(p2score, SCREEN_WIDTH-10, 10, 20, WHITE);
		display_items(p2, SCREEN_WIDTH-20-10, 20);


}
