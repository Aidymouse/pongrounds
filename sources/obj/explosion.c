#include "obj.h"
#include "structs.h"

void explosion_init(Explosion *e) {
}

void explosion_spawn(Vector2 pos, struct PongState *pong_state) {
	Explosion e;
	explosion_init(&e);
	pong_state->explosions[pong_state->num_explosions] = e;
	pong_state->num_explosions += 1;
}

void explosion_update(float dt, Explosion *e) {
	if (e->delete_me) { return; }

	e->life_timer -= dt;
	if (e->life_timer <= 0) {
		e->delete_me = true;
	}
}

void explosion_draw(Explosion *e) {
}

void explosion_cleanup(struct PongState *pong_state) {
	for (int i=0; i<pong_state->num_explosions; i++) {
		Explosion *e = &pong_state->explosions[i];	
		if (e->delete_me) {
			pong_state->explosions[i] = pong_state->explosions[pong_state->num_explosions-1];
			pong_state->num_explosions -= 1;
			i -= 1;
		}
	}
}
