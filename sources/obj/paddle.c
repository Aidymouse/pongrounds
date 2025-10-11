#include "structs.h"
#include "obj.h"
#include "defines.h"
#include "Vec2.h"
#include "helper.h"
#include <stdio.h>

/**
 * WARN: returns NULL if no paddle is made
 */
struct PaddleData *paddle_spawn(struct PongState *pong_state) {
	if (pong_state->num_paddles >= MAX_PADDLES) {
		return NULL;
	}
	paddle_init(&pong_state->paddles[pong_state->num_paddles]);
	pong_state->num_paddles += 1;
	return &pong_state->paddles[pong_state->num_paddles-1];
}

void paddle_init(PaddleData *p) {
	p->pos.x = SCREEN_WIDTH/2 - PADDLE_DEFAULT_WIDTH/2;
	p->paddle_width = PADDLE_DEFAULT_WIDTH;
	p->delete_me = false;
	p->paddle_thickness = 10;
	p->color = WHITE;
	p->hp = PADDLE_DEFAULT_HP;
	p->max_hp = PADDLE_DEFAULT_HP;
	p->vel.x = 0;
	p->vel.y = 0;
	p->speed = PADDLE_SPEED;
	p->max_speed = PADDLE_SPEED;

	p->sword_frame = 0;
	p->sword_anim_timer = 0;
	p->sword_anim_dir = 1;
	p->sword_timer = 0;
	p->brain = PB_PLAYER;

	for (int i=0; i<NUM_ITEMS; i++) {
		p->items[i] = 0;
		p->items_total[i] = 0;
		p->item_cooldown_timers[i] = 0;
		p->item_use_timers[i] = 0;
	} 

	p->cv_creator = NULL;
	p->cv_clone = false;
	p->cv_num_clones = 0;
	p->cv_lag_dist = 0;

}


Rectangle paddle_get_time_influence_area(PaddleData *paddle) {
	return (Rectangle){ 
		paddle->pos.x - CHRONOMETER_SIZE/2,
		paddle->pos.y - CHRONOMETER_SIZE/2,
		CHRONOMETER_SIZE+paddle->paddle_width,
		CHRONOMETER_SIZE+paddle->paddle_thickness 
	};
}

Vector2 paddle_center(PaddleData *p) { return (Vector2){ p->pos.x + p->paddle_width/2, p->pos.y + p->paddle_thickness/2}; }

/** */
float paddle_get_time_power(PaddleData *paddle) {
	float speed_multiplier = 1;
	if (paddle->items[ITEM_TIME_WIZARDS_CHRONOMETER] > 0) {
		speed_multiplier *= BASE_TIME_POWER;
		for (int t = paddle->items[ITEM_TIME_WIZARDS_CHRONOMETER]-1; t>0; t--) {
			speed_multiplier *= CUMULATIVE_TIME_POWER;
		}
	}
	return speed_multiplier;
}

/** */
Rectangle paddle_get_rect(PaddleData *p) {
	Rectangle r = {
		.x = p->pos.x,
		.y = p->pos.y,
		.width = p->paddle_width,
		.height = p->paddle_thickness
	};
	return r;
}

/** Gets the zone of influence for russian secrets, providing it exists. Otherwise returns a 0 width rectangle at 0, 0 (this might be a bug actually) */
Rectangle paddle_get_russian_secrets_rect(PaddleData *p) {
	if (p->items[ITEM_RUSSIAN_SECRETS] < 1) {
		Rectangle none = { .x=0, .y=0, .width=0, .height=0 };
		return none;
	}

	Vector2 p_center = paddle_center(p);
	
	float russian_secrets_width = RUSSIAN_SECRETS_AREA_WIDTH + (p->items[ITEM_RUSSIAN_SECRETS]-1) * RUSSIAN_SECRETS_AREA_ADDITIONAL;

	Rectangle russian_secrets_rect = {
		.x = p_center.x - russian_secrets_width/2,
		.y = p->pos.y,
		.width = russian_secrets_width,
		.height = p->paddle_thickness
	};
	return russian_secrets_rect;
}


/** Refreshes a paddle, giving all it's items back and applying items that change properties of the paddle e.g. Hypergonadism */
void paddle_refresh(PaddleData *p, PaddleData *opponent, struct GameState *state) {

	p->hp = p->max_hp;
	p->destroyed_timer = 0;

	for (int i=0; i<NUM_ITEMS; i++) {
		p->items[i] = p->items_total[i];
		p->item_cooldown_timers[i] = 0;
		p->item_use_timers[i] = 0;
	}

	float width_bonus = HYPERGONADISM_WIDTH_BONUS * p->items[ITEM_HYPERGONADISM];
	float width_penalty = CHASTITY_CAGE_WIDTH_PENALTY * opponent->items[ITEM_CHASTITY_CAGE];

	p->paddle_width = PADDLE_DEFAULT_WIDTH + width_bonus - width_penalty;

	// Update Y Position (in case screen zoom has changed via nerd glasses). But only if you don't have omnidirectional movement
	if (p->items[ITEM_BACHELOR_OF_PSYCHOLOGY_HONS] == 0) {
		float world_top = GetScreenToWorld2D((Vector2){0, 0}, *state->camera).y;
		float world_bottom = GetScreenToWorld2D((Vector2){0, SCREEN_HEIGHT}, *state->camera).y;
		if (p->id == 1) {
			p->pos.y = world_top + 40;
		} else {
			p->pos.y = world_bottom - p->paddle_thickness - 40;
		}
	}

	// Handle cloning vat
	p->cv_num_clones = 0; // State change handles deleting extra paddles

}


/** */
void clone_paddle_init(struct PaddleData *c, struct PaddleData *creator) {
	//paddle_init(c);
	c->brain = PB_CLONE;
	c->cv_creator = creator;
	c->cv_clone = true;
	c->cv_lag_dist = CV_DEFAULT_LAG_DISTANCE * creator->cv_num_clones;
	c->color = GRAY;
	// Clones dont get items
	for (int i=0; i<NUM_ITEMS; i++) {
		c->items[i] = 0;
		c->items_total[i] = 0;
	}
}

/** */
void paddle_activate_items(float dt, PaddleData *p, struct PongState *pong_state) {

	if (p->destroyed_timer > 0) { return; }

	// Ceremonial Sword
	if (p->items[ITEM_CEREMONIAL_SWORD] > 0 && p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] <= 0 && p->sword_timer <= 0) {
		p->sword_timer = SWORD_DURATION;
		p->sword_anim_timer = SWORD_FRAME_TIME;
	}

	// Nuclear Launch Codes
	if (p->items[ITEM_NUCLEAR_LAUNCH_CODES] > 0 && p->item_cooldown_timers[ITEM_NUCLEAR_LAUNCH_CODES] <= 0 && pong_state->num_rockets < MAX_ROCKETS) {
		printf("Spawning rocket %d\n", pong_state->num_rockets);
		RocketData r;
		rocket_init(&r, p);

		pong_state->rockets[pong_state->num_rockets] = r;
		pong_state->num_rockets += 1;
		p->item_cooldown_timers[ITEM_NUCLEAR_LAUNCH_CODES] = 0.5;

	}

	// Antique Gaming Console
	if (p->items[ITEM_ANTIQUE_GAME_CONSOLE] > 0) {
		for (int i=0; i<pong_state->num_balls; i++) {
			pong_state->balls[i].vel.x *= -1;
			pong_state->balls[i].kb_dir.x *= -1;
		}

		p->items[ITEM_ANTIQUE_GAME_CONSOLE] -= 1;
		p->item_use_timers[ITEM_ANTIQUE_GAME_CONSOLE] = ITEM_USE_BUMP_TIME;
	}

	// Cloning Vat
	if (p->items[ITEM_CLONING_VAT] > 0) {
		p->items[ITEM_CLONING_VAT] -= 1;
		PaddleData *c = paddle_spawn(pong_state);
		if (c != NULL) { 
			PaddleData clone = *p;
			p->cv_num_clones += 1;
			clone_paddle_init(&clone,p);
			*c = clone;
			// Find pos
			c->pos = Vec2Add(p->pos, Vec2MultScalar(p->facing, (p->paddle_thickness + 10) * p->cv_num_clones));
			// TODO: how to stop paddles layering on top of one another
		}
	}

}


/** 
 * a.k.a paddle_brain_player()
 * */
void paddle_player_control(float dt, struct PaddleData *p, struct PaddleControls controls, struct GameState *state) {
	// Higher than max speed and we're dashing - no control allowed
	if (p->speed <= p->max_speed) { 
		if (IsKeyDown(controls.left)) {
			p->vel.x = -1;
		} else if (IsKeyDown(controls.right)) {
			p->vel.x = 1;
		} else {
			p->vel.x = 0;
		}

		// TODO: limit
		if (p->items[ITEM_BACHELOR_OF_PSYCHOLOGY_HONS] > 0) {
			if (IsKeyDown(controls.up)) {
				p->vel.y = -1;
			} else if (IsKeyDown(controls.down)) {
				p->vel.y = 1;
			} else {
				p->vel.y = 0;
			}
		}
	}

	// TODO: timer???
	if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyPressed(controls.dash)) {
		p->speed = CLOAK_DASH_SPEED + CLOAK_DASH_SPEED_BONUS*p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1;
	}

	if (IsKeyPressed(controls.item)) {
		paddle_activate_items(dt, p, state->pong_state);
	}
}





/**  */
void paddle_brain_clone(struct PaddleData *paddle, struct GameState *state) {
	// If the master paddle is far away, move toward it. Otherwise sit still
	// very simply, if paddle is to the left of ball, go right. otherwise, go left
	
	struct PongState *pong_state = state->pong_state;

	struct BallData ball = pong_state->balls[0];

	struct PaddleData *following = paddle->cv_creator;

	float dist = following->pos.x - paddle->pos.x;

	float MAX_DIST = paddle->cv_lag_dist;
	if (dist * dist > MAX_DIST*MAX_DIST) {
		if (dist < 0) {
			paddle->vel.x = -1;
		} else {
			paddle->vel.x = 1;
		}
	} else {
		paddle->vel.x = 0;
	}
}

/** */
void paddle_update(float dt, PaddleData *p, struct GameState *state) {

	if (p->destroyed_timer > 0) { 
		p->destroyed_timer -= dt;
		return;
	 }

	if (p->brain == PB_PLAYER) {
		struct PaddleControls controls = state->player1->controls;
		if (p->id == 2) { controls = state->player2->controls; }

		paddle_player_control(dt, p, controls, state);

	} else if (p->brain == PB_CLONE) {
		if (p->destroyed_timer > 0) { p->delete_me = true; }

		paddle_brain_clone(p, state);
	}


	// Decelerate dash
	if (p->speed > p->max_speed) {
		p->speed -= 250;
		if (p->speed < p->max_speed) {
			p->speed = p->max_speed;
		}
	}

	// External speed multipliers
	// TODO: time wizards chronometer slowing down non-allied paddles
	float speed_mult = 1;
	if (p->brain == PB_CLONE) {
		speed_mult *= CV_CLONE_SPEED_MULT;
	}

	p->pos = Vec2Add(p->pos, Vec2MultScalar(p->vel, p->speed*speed_mult*dt));

	float world_left = GetScreenToWorld2D((Vector2){0, 0}, *state->camera).x;
	float world_right = GetScreenToWorld2D((Vector2){SCREEN_WIDTH, 0}, *state->camera).x;

	if (p->pos.x + p->paddle_width > world_right) {
		p->pos.x = world_right - p->paddle_width;
	} else if (p->pos.x < world_left) {
		p->pos.x = world_left;
	}

	for (int i=0; i<NUM_ITEMS; i++) {
		p->item_cooldown_timers[i] -= dt;
		p->item_use_timers[i] -= dt;
	}

}



void paddle_draw(PaddleData *p) {
			if (p->destroyed_timer > 0) { return; }
			DrawRectangle(p->pos.x, p->pos.y, p->paddle_width, p->paddle_thickness, p->color);
			if (p->items[ITEM_CEREMONIAL_SWORD] > 0) {
				sword_draw(p, false);
			}

			if (p->items[ITEM_RUSSIAN_SECRETS] > 0) {
				Rectangle rsa = paddle_get_russian_secrets_rect(p);
				DrawRectangleLinesEx(rsa, 1, RED);
			}
}

void paddle_cleanup(struct PongState *pong_state) {

	for (int p=2; p<pong_state->num_paddles; p++) {
		struct PaddleData *paddle = &pong_state->paddles[p];
		if (pong_state->num_paddles <= 2) { return; }

		if (paddle->delete_me) {
			pong_state->paddles[p] = pong_state->paddles[pong_state->num_paddles-1];
			pong_state->num_paddles -= 1;
			p -= 1;
		}
	}
}

