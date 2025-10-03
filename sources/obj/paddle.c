#include "structs.h"
#include "obj.h"
#include "defines.h"
#include "Vec2.h"
#include <stdio.h>

void paddle_refresh(struct PaddleData *p, struct PaddleData *opponent, struct GameState *state) {

	p->hp = p->max_hp;

	for (int i=0; i<NUM_ITEMS; i++) {
		p->items[i] = p->items_total[i];
		p->item_cooldown_timers[i] = 0;
		p->item_use_timers[i] = 0;
	}

	// Width changes have a flat bonus of 30, then a 5% width increase for every hypergonadism. It's 5% of the prior width so there's diminishing returns
	float width_bonus = 0;
	if (p->items[ITEM_HYPERGONADISM] > 0) {

		float new_width = PADDLE_DEFAULT_WIDTH + 20;

		for (int b=p->items[ITEM_HYPERGONADISM]-1; b>0; b--) {
			new_width *= 1.05;
		}

		width_bonus = new_width - PADDLE_DEFAULT_WIDTH;
	}

	float width_penalty = 0;
	if (opponent->items[ITEM_CHASTITY_CAGE] > 0) {

		float new_width = PADDLE_DEFAULT_WIDTH + 20;

		for (int b=opponent->items[ITEM_CHASTITY_CAGE]-1; b>0; b--) {
			new_width *= 1.05;
		}

		width_penalty = new_width - PADDLE_DEFAULT_WIDTH;
	}

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
}

void paddle_update(float dt, struct PaddleData *p) {
	for (int i=0; i<NUM_ITEMS; i++) {
		p->item_cooldown_timers[i] -= dt;
		p->item_use_timers[i] -= dt;
	}
}

void paddle_move(float dt, struct PaddleData *p, struct PaddleControls controls, struct GameState *state) {


		// Higher than max speed and we're dashing - no control allowed
		if (p->speed <= p->max_speed) { 
			if (IsKeyDown(controls.left)) {
				p->vel.x = -1;
			} else if (IsKeyDown(controls.right)) {
				p->vel.x = 1;
			} else {
				p->vel.x = 0;
			}

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

		if (p->items[ITEM_CHERRY_BLOSSOM_CLOAK] > 0 && IsKeyPressed(controls.dash)) {
			p->speed = 2200 + 200*p->items[ITEM_CHERRY_BLOSSOM_CLOAK]-1;
		}

		// Decelerate dash
		if (p->speed > p->max_speed) {
			p->speed -= 250;
			if (p->speed < p->max_speed) {
				p->speed = p->max_speed;
			}
		}

		p->pos = Vec2Add(p->pos, Vec2MultScalar(p->vel, p->speed*dt));

		float world_left = GetScreenToWorld2D((Vector2){0, 0}, *state->camera).x;
		float world_right = GetScreenToWorld2D((Vector2){SCREEN_WIDTH, 0}, *state->camera).x;

		if (p->pos.x + p->paddle_width > world_right) {
			p->pos.x = world_right - p->paddle_width;
		} else if (p->pos.x < world_left) {
			p->pos.x = world_left;
		}
}

void paddle_activate_items(float dt, struct PaddleData *p) {

	if (p->items[ITEM_CEREMONIAL_SWORD] > 0 && p->item_cooldown_timers[ITEM_CEREMONIAL_SWORD] <= 0 && p->sword_timer <= 0) {
		p->sword_timer = SWORD_DURATION;
	}

	if (p->items[ITEM_NUCLEAR_LAUNCH_CODES] > 0) {
		// TODO
	}

}

