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
	p->dir = (Vector2){0, 0};
	p->speed = (Vector2){ 0, 0 };
	p->max_speed = (Vector2){PADDLE_SPEED, PADDLE_SPEED*0.6};
	p->invincibility_timer = 0;
	p->hit_timer = 0;

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
	p->invincibility_timer = 0;

	if (p->bmcm_flipped) {
		paddle_flip_across_axis(p);
	}

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

void paddle_flip_across_axis(PaddleData *paddle) {
	Vector2 pc = paddle_center(paddle);
	float dist_to_center = SCREEN_HEIGHT/2 - pc.y;
	paddle->pos.y = SCREEN_HEIGHT/2 + dist_to_center;
	paddle->facing = Vec2Rotate(paddle->facing, 180);
	paddle->bmcm_flipped = !paddle->bmcm_flipped;
}

/** */
void paddle_activate_items(float dt, PaddleData *p, struct PongState *pong_state) {

	if (p->destroyed_timer > 0) { return; }

	// Ceremonial Sword
	if (p->items[ITEM_CEREMONIAL_SWORD] > 0 && p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] <= 0 && p->sword_timer <= 0) {
		p->sword_timer = SWORD_DURATION;
		p->sword_anim_timer = SWORD_FRAME_TIME;
		p->item_use_timers[ITEM_CEREMONIAL_SWORD] = SWORD_DURATION;
	}

	// Nuclear Launch Codes
	if (p->items[ITEM_NUCLEAR_LAUNCH_CODES] > 0 && p->item_cooldown_timers[ITEM_NUCLEAR_LAUNCH_CODES] <= 0 && pong_state->num_rockets < MAX_ROCKETS) {
		RocketData r;
		rocket_init(&r, p);

		pong_state->rockets[pong_state->num_rockets] = r;
		pong_state->num_rockets += 1;
		p->item_cooldown_timers[ITEM_NUCLEAR_LAUNCH_CODES] = MISSILE_COOLDOWN;

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


	// Broken Mind Control Machine
	if (p->items[ITEM_BROKEN_MIND_CONTROL_MACHINE] > 0) {
		p->item_use_timers[ITEM_BROKEN_MIND_CONTROL_MACHINE] = ITEM_USE_BUMP_TIME;
		p->items[ITEM_BROKEN_MIND_CONTROL_MACHINE] -= 1;

		for (int pI = 0; pI < pong_state->num_paddles; pI++) {
			struct PaddleData *paddle = &pong_state->paddles[pI];
			paddle_flip_across_axis(paddle);
		}
	}

}


/** 
 * a.k.a paddle_brain_player()
 * */
void paddle_player_control(float dt, struct PaddleData *p, struct PaddleControls controls, struct GameState *state) {
	// Higher than max speed and we're dashing - no control allowed. 
	// Dash speed reduction handled in update
	Vector2 moving_impulse = { .x = 0, .y = 0 };
	if (p->speed.x <= p->max_speed.x && p->speed.y <= p->max_speed.y) { 
		if (IsKeyDown(controls.left)) {
			moving_impulse.x = -1;
		} else if (IsKeyDown(controls.right)) {
			moving_impulse.x = 1;
		} 

		if (p->items[ITEM_BACHELOR_OF_PSYCHOLOGY_HONS] > 0) {
			if (IsKeyDown(controls.up)) {
				moving_impulse.y = -1;
			} else if (IsKeyDown(controls.down)) {
				moving_impulse.y = 1;
			} 	
		} 

		if (moving_impulse.x == 0) {
			if (p->speed.x > 0) {
				p->speed.x -= PADDLE_DECELLERATION; 
				if (p->speed.x < 0) { p->speed.x = 0; }
			}
		}
		if (moving_impulse.y == 0) {
			if (p->speed.y > 0) {
				p->speed.y -= PADDLE_DECELLERATION;
				if (p->speed.y < 0) { p->speed.y = 0; }
			}
		} 


		//p->dir = Vec2Normalize(p->dir);
		//Vector2 desired_vel = Vec2Normalize(moving_impulse);

	}
		
	// Turn move impulse into a direction and speed
	if (moving_impulse.x != 0) {
		p->dir.x = moving_impulse.x;
		p->speed.x = PADDLE_SPEED;
	}
	if (moving_impulse.y != 0) {
		p->dir.y = moving_impulse.y;
		p->speed.y = PADDLE_SPEED * 0.6;
	}

	if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyPressed(controls.dash)) {
		if (moving_impulse.x != 0) {
			p->speed.x = CLOAK_DASH_SPEED + CLOAK_DASH_SPEED_BONUS*p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1;
		}
		if (moving_impulse.y != 0) {
			p->speed.y = CLOAK_DASH_SPEED + CLOAK_DASH_SPEED_BONUS*p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1;
		}
		//p->speed.y = 
		p->item_use_timers[ITEM_CHERRY_BLOSSOM_CLOAK] = ITEM_USE_BUMP_TIME;
	}

	if (state->pong_state->end_round_timer <= 0) {
		if (IsKeyPressed(controls.item)) {
			paddle_activate_items(dt, p, state->pong_state);
		}
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
			//paddle->vel.x = -1;
		} else {
			//paddle->vel.x = 1;
		}
	} else {
		//paddle->vel.x = 0;
	}
}

/** */
void paddle_update(float dt, PaddleData *p, struct GameState *state, WorldBorders borders) {

	if (p->destroyed_timer > 0) { 
		p->destroyed_timer -= dt;
		if (p->destroyed_timer <= 0) {
			p->invincibility_timer = DAMAGE_INVINCIBILITY_TIME;
		}
		return;
	}

	if (p->invincibility_timer > 0) { p->invincibility_timer -= dt; }

	if (p->brain == PB_PLAYER) {
		struct PaddleControls controls = state->player1->controls;
		if (p->id == 2) { controls = state->player2->controls; }

		paddle_player_control(dt, p, controls, state);

	} else if (p->brain == PB_CLONE) {
		if (p->destroyed_timer > 0) { p->delete_me = true; }

		paddle_brain_clone(p, state);
	}


	// Decelerate dash
	if (p->speed.x > p->max_speed.x) {
		p->speed.x -= CLOAK_DECELLERATION;
		if (p->speed.x < p->max_speed.x) {
			p->speed.x = p->max_speed.x;
		}
	}

	if (p->speed.y > p->max_speed.y) {
		p->speed.y -= CLOAK_DECELLERATION;
		if (p->speed.y < p->max_speed.y) {
			p->speed.y = p->max_speed.y;
		}
	}

	// External speed multipliers
	// TODO: time wizards chronometer slowing down non-allied paddles
	float speed_mult = 1;
	if (p->brain == PB_CLONE) {
		speed_mult *= CV_CLONE_SPEED_MULT;
	}

	// Turn speed and dir into an actual velocity and then move that velocity
	if (p->speed.x != 0 || p->speed.y != 0) {
		Vector2 normal_speed = p->speed;
		Vector2 signed_speed = {
			.x = normal_speed.x * p->dir.x,
			.y = normal_speed.y * p->dir.y,
		};
		//Vector2 vel = Vec2Normalize(signed_speed);
		p->pos = Vec2Add(p->pos, Vec2MultScalar(signed_speed, speed_mult*dt));
	}

	// Cap position by number of bachelors of psychology
	int y_cap = SCREEN_HEIGHT/2 + BACHELOR_DIST_FROM_CENTER * p->facing.y;
	if (p->items[ITEM_BACHELOR_OF_PSYCHOLOGY_HONS] > 0) {
		y_cap -= BACHELOR_ADDITIONAL_DISTANCE * (p->items[ITEM_BACHELOR_OF_PSYCHOLOGY_HONS]-1) * p->facing.y;
	}

	if (p->facing.y == -1 && p->pos.y < y_cap) {
		p->pos.y = y_cap;
	} else if (p->facing.y == 1 && p->pos.y + p->paddle_thickness > y_cap) {
		p->pos.y = y_cap - p->paddle_thickness;
	}
	
	// Cap position by world border
	if (p->pos.x + p->paddle_width > borders.right) {
		p->pos.x = borders.right - p->paddle_width;
	} else if (p->pos.x < borders.left) {
		p->pos.x = borders.left;
	}

	if (p->pos.y + p->paddle_thickness > borders.bottom) {
		p->pos.y = borders.bottom - p->paddle_thickness;
	} else if (p->pos.y < borders.top) {
		p->pos.y = borders.top;
	}

	for (int i=0; i<NUM_ITEMS; i++) {
		p->item_cooldown_timers[i] -= dt;
		p->item_use_timers[i] -= dt;
	}

}


/** */
Circle paddle_get_gravity_circle(PaddleData *p) {
	Vector2 center = paddle_center(p);
	Circle c = {
		.x = center.x,
		.y = center.y,
		.radius = YM_RADIUS + 10 * ((p->paddle_width - PADDLE_DEFAULT_WIDTH) / HYPERGONADISM_WIDTH_BONUS) // Pretty hacky but should mean you have +10 for every hypergonadism and -10 for every chastity cage
	};
	return c;
}

void paddle_check_collisions(PaddleData *p, struct GameState *state) {
	struct PongState *pong_state = state->pong_state;
	if (p->invincibility_timer <= 0) {
		// Swords
		for (int pI=0; pI<pong_state->num_paddles; pI++) {
			PaddleData *paddle = &pong_state->paddles[pI];
			if (paddle == p) { continue; }
			Rectangle sword = sword_get_hitbox(paddle);
			Rectangle me = paddle_get_rect(p);
			if ( CheckCollisionRecs(sword, me) && paddle->sword_timer > 0 ) {
				if (p->brain == PB_CLONE) {
					p->delete_me = true;
				} else {
					p->hp -= SWORD_DAMAGE;
					p->invincibility_timer = DAMAGE_INVINCIBILITY_TIME;
				}
			}
		}
	}
}

void paddle_draw(PaddleData *p, bool debug) {
			if (p->destroyed_timer > 0) { return; }
			DrawRectangle(p->pos.x, p->pos.y, p->paddle_width, p->paddle_thickness, p->color);

			if (p->items[ITEM_CEREMONIAL_SWORD] > 0) {
				sword_draw(p, debug);
			}

			if (debug) {

				if (p->items[ITEM_RUSSIAN_SECRETS] > 0) {
					Rectangle rsa = paddle_get_russian_secrets_rect(p);
					DrawRectangleLinesEx(rsa, 1, RED);
				}

				if (p->items[ITEM_YO_MOMMA] > 0) {
					Circle gravity = paddle_get_gravity_circle(p);
					DrawCircleLines(gravity.x, gravity.y, gravity.radius, RED);
				}

				char s[32];
				sprintf(s, "Speed: %f, %f\n", p->speed.x, p->speed.y);
				DrawText(s, p->pos.x, p->pos.y + p->paddle_thickness, 10, WHITE);
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

