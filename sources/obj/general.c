#include "Vec2.h"
#include "obj/obj.h"
#include "helper.h"

/** Manipulates an objects direction vector toward the direction of a gravitational center */
void gravitate_towards(float dt, Vector2 pos, Vector2 *dir, Vector2 center, float strength) {
	float dir_angle = Vec2GetAngle(*dir);
	Vector2 to_center = Vec2Sub(center, pos);
	float dir_center = Vec2GetAngle(to_center);
	if (get_angle_distance(dir_angle, dir_center) < 0) {
		*dir = Vec2Rotate(*dir, -strength*dt);
	} else {
		*dir = Vec2Rotate(*dir, strength*dt);
	}
}
