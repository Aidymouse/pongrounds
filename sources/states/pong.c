#include "pong.h"

#include <stdio.h>
#include "defines.h"
#include "structs.h"
#include "state_change.h"

#include "Vec2.h"
#include "helper.h"

// Called on point score
void refresh_paddle(struct PaddleData *p) {

	for (int i=0; i<16; i++) {
		p->items[i] = p->items_total[i];
	}

	p->paddle_width = DEFAULT_PADDLE_WIDTH + 10 * (p->items[ITEM_HYPERGONADISM]);
	// TODO: reduce width by enemy
}

void display_items(struct PaddleData *p, int x, int y) {
	for (int i=0; i<16; i++) {
		char s[16];
		//sprintf(p1score, "%d", player1.score);
		sprintf(s, "%d", p->items[i]);
		DrawText(s, x, y+i*10, 10, WHITE);
		
	}
}

void display_round_score(struct PlayerData *p, int x, int y) {
	for (int i=0; i<POINTS_PER_ROUND; i++) {
		if (p->score >= (i+1)) {
			DrawCircle(x + i*15, y, 5, WHITE);	
		} else {
			DrawCircleLines(x+i*15, y, 5, WHITE);	
		}
	}
}

// BALL //
void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	if (b->vel.y < 0) { b->vel.y = -BALL_INIT_SPEED; } else { b->vel.y = BALL_INIT_SPEED; }
}

void ball_reflect(struct BallData *b) {
	b->vel.y = -b->vel.y;
	if (b->vel.y < 0) {
		b->vel.y -= 20;
	} else {
		b->vel.y += 20;
	}
}

void ball_paddle_hit(struct BallData *b, struct PaddleData *p) {
	ball_reflect(b);

	b->vel.x = randInt(-300, 300);
}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer) {

	struct PaddleData *paddle = scorer->paddle;

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

void state_pong(float dt, struct GameState *state) {

		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;
		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		struct BallData *ball = state->ball;

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

		if (ball->pos.y - ball->radius < 0 && p1->items[ITEM_EXPIRED_PANADOL] > 0) {
			p1->items[ITEM_EXPIRED_PANADOL] -= 1;
			ball_reflect(ball);
		} else if (ball->pos.y + ball->radius > SCREEN_HEIGHT && p2->items[ITEM_EXPIRED_PANADOL] > 0) {
			p2->items[ITEM_EXPIRED_PANADOL] -= 1;
			ball_reflect(ball);
		}

		// Scoring
		if (ball->pos.y - ball->radius > SCREEN_HEIGHT+10) {
			ball_score_hit(ball, player1);
		} else if (ball->pos.y + ball->radius + 10 < 0) {
			ball_score_hit(ball, player2);
		}

		if (player1->score >= POINTS_PER_ROUND) {
			change_state_to_pick_items(state, player2->paddle);
		} else if (player2->score >= POINTS_PER_ROUND) {
			change_state_to_pick_items(state, player1->paddle);
		}

		// Player control
		paddle_move(dt, p1, P1_LEFT_KEY, P1_RIGHT_KEY);
		paddle_move(dt, p2, P2_LEFT_KEY, P2_RIGHT_KEY);

}

void draw_pong(struct GameState *state) {

		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;

		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		struct  BallData *ball = state->ball;

		// Draw Paddle One
		DrawRectangle(p1->pos.x, p1->pos.y, p1->paddle_width, p1->paddle_thickness, p2->color);

		// Draw Paddle Two
		DrawRectangle(p2->pos.x, p2->pos.y, p2->paddle_width, p2->paddle_thickness, p2->color);
	
		// BALL
		DrawCircle(ball->pos.x, ball->pos.y, ball->radius, WHITE);

		// Score
		display_items(p1, 20, 20);
		display_round_score(player1, SCREEN_WIDTH/2, 10);

		display_items(p2, SCREEN_WIDTH-20-10, 20);
		display_round_score(player2, SCREEN_WIDTH/2, SCREEN_HEIGHT-15);


}
