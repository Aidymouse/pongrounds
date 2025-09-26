#include "pong.h"

#include <stdio.h>
#include "defines.h"
#include "structs.h"
#include "state_change.h"

#include "Vec2.h"
#include "helper.h"

// Called on point score
void refresh_paddle(struct PaddleData *p, struct PaddleData *opponent) {

	p->hp = p->max_hp;

	for (int i=0; i<16; i++) {
		p->items[i] = p->items_total[i];
	}

	p->paddle_width = DEFAULT_PADDLE_WIDTH + 20 * (p->items[ITEM_HYPERGONADISM]) - 20 * (opponent->items[ITEM_CHASTITY_CAGE]);
}

void display_items(struct PaddleData *p, int x, int y) {
	for (int i=0; i<16; i++) {
		char s[16];
		//sprintf(p1score, "%d", player1.score);
		sprintf(s, "%d", p->items[i]);
		DrawText(s, x, y+i*10, 10, WHITE);
		
	}
}

/*
void display_round_score(struct PlayerData *p, int x, int y) {
	for (int i=0; i<POINTS_PER_ROUND; i++) {
		if (p->score >= (i+1)) {
			DrawCircle(x + i*15, y, 5, WHITE);	
		} else {
			DrawCircleLines(x+i*15, y, 5, WHITE);	
		}
	}
}
*/

void display_health(struct PlayerData *p, int x, int y) {
	struct PaddleData *paddle = p->paddle;

	int width = paddle->max_hp;

	DrawRectangleLines(x - width/2, y, width, 10, WHITE);
	DrawRectangle(x - width/2, y, paddle->hp, 10, WHITE);
}

// BALL //
void ball_respawn(struct BallData *b) {
	b->pos.x = SCREEN_WIDTH/2;
	b->pos.y = SCREEN_HEIGHT/2;
	b->vel.y = -b->vel.y;
	if (b->vel.y < 0) { b->vel.y = -BALL_INIT_SPEED; } else { b->vel.y = BALL_INIT_SPEED; }
}

void ball_reflect(struct BallData *b) {
	b->speed += 20;
	if (b->vel.y > 0) {
		b->vel.y = -b->speed;
	} else {
		b->vel.y = b->speed;
	}

	b->kb_dir.y = -b->kb_dir.y;

}

void ball_paddle_hit(struct BallData *b, struct PaddleData *p) {
	ball_reflect(b);

	b->vel.x = randInt(-300, 300);

	// TODO: start ball knuckleball
	b->state = BS_KNUCKLEBALL;
	b->kb_dir = Vec2Normalize(b->vel);
	
	printf("vel %f, %f\n", b->vel.x, b->vel.y);
	printf("kb dir %f, %f\n", b->kb_dir.x, b->kb_dir.y);

	if (p->items[ITEM_NIEKRO_CARD] > 0) {
		int knuckleball_chance = 20 + p->items[ITEM_NIEKRO_CARD]-1 * 5;
		if (randInt(1, 100) < knuckleball_chance) {
		}
	}

}

void ball_score_hit(struct BallData *b, struct PlayerData *scorer, struct PlayerData *opponent) {

	struct PaddleData *paddle = scorer->paddle;

	//scorer->score +=1;
	opponent->paddle->hp -= SCORE_DAMAGE;
	ball_respawn(b);
}

void ball_move(float dt, struct BallData *ball) {


	if (ball->state == BS_NORMAL) {
		// If this occurs alongside knuckleball, the ball sways around all drunk its really fun
		ball->pos = Vec2Add(ball->pos, Vec2MultScalar(ball->vel, dt));
	} else if (ball->state == BS_KNUCKLEBALL) {
		// Weight toward direction side depending on how far i am.

		/*
		Vector2 dir_vec; 
		dir_vec.x = 0;
		dir_vec.y = ball->kb_desired_dir; // Pointing straight up
		float desired_angle = Vec2GetAngle(dir_vec);
		*/

		// This code is equiv to the above commented out block
		float desired_angle;
		if (ball->kb_desired_dir == -1) {
			desired_angle = -90;
		} else {
			desired_angle = 90;
		}

		float dist_to_desired = get_angle_distance(Vec2GetAngle(ball->kb_dir), desired_angle);

		ball->kb_dir = Vec2Rotate(ball->kb_dir, 2);

		ball->pos = Vec2Add(ball->pos, Vec2MultScalar(Vec2MultScalar(ball->kb_dir, ball->speed), dt));



		// IDEA: If we're far enough toward one side just commit to a loop
	}
}


// PADDLE //
void paddle_move(float dt, struct PaddleData *p, struct PaddleControls controls) {

		if (p->vel.x > 300) {
			p->vel.x -= 500;
		} else if (p->vel.x < -300) {
			p->vel.x += 500;
		} else if (IsKeyDown(controls.left)) {
				p->vel.x = -300;

				if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyDown(controls.dash)) {
					p->vel.x = -3000 + (p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1)*-100;
				}
		} else if (IsKeyDown(controls.right)) {
				p->vel.x = 300;

				if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyDown(controls.dash)) {
					p->vel.x = 3000 + (p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1)*100;
				}
		} else {
			p->vel.x = 0;
		}

		p->pos = Vec2Add(p->pos, Vec2MultScalar(p->vel, dt));

		if (p->pos.x + p->paddle_width > SCREEN_WIDTH) {
			p->pos.x = SCREEN_WIDTH - p->paddle_width;
		} else if (p->pos.x < 0) {
			p->pos.x = 0;
		}
}

void state_pong(float dt, struct GameState *state) {

		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;
		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		struct BallData *ball = state->ball;

		// Update //
		ball_move(dt, ball);

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
			ball_score_hit(ball, player1, player2);
		} else if (ball->pos.y + ball->radius + 10 < 0) {
			ball_score_hit(ball, player2, player1);
		}

		if (player1->paddle->hp <= 0) {
			change_state_to_pick_items(state, player1->paddle);
		} else if (player2->paddle->hp <= 0) {
			change_state_to_pick_items(state, player2->paddle);
		}

		// Player control
		struct PaddleControls p1_controls = { P1_LEFT_KEY, P1_RIGHT_KEY, P1_DASH_KEY };
		paddle_move(dt, p1, p1_controls);
		struct PaddleControls p2_controls = { P2_LEFT_KEY, P2_RIGHT_KEY, P2_DASH_KEY };
		paddle_move(dt, p2, p2_controls);

}

void draw_pong(struct GameState *state) {

		struct PlayerData *player1 = state->player1;
		struct PlayerData *player2 = state->player2;

		struct PaddleData *p1 = player1->paddle;
		struct PaddleData *p2 = player2->paddle;

		struct  BallData *ball = state->ball;

		// Draw Paddle One
		DrawRectangle(p1->pos.x, p1->pos.y, p1->paddle_width, p1->paddle_thickness, p1->color);

		// Draw Paddle Two
		DrawRectangle(p2->pos.x, p2->pos.y, p2->paddle_width, p2->paddle_thickness, p2->color);
	
		// BALL
		DrawCircle(ball->pos.x, ball->pos.y, ball->radius, WHITE);
		// Draw line of balls knuckleball facing
		DrawLine(ball->pos.x, ball->pos.y, ball->pos.x + ball->kb_dir.x*10, ball->pos.y + ball->kb_dir.y*10, RED);

		// Score
		display_items(p1, 20, 20);
		display_health(player1, SCREEN_WIDTH/2, 10);

		display_items(p2, SCREEN_WIDTH-20-10, 20);
		display_health(player2, SCREEN_WIDTH/2, SCREEN_HEIGHT-10-10);

}
