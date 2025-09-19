#include "Vec2.h"

Vector2 Vec2Add(Vector2 v1, Vector2 v2) {
	Vector2 added;
	added.x = v1.x + v2.x;
	added.y = v1.y + v2.y;
	return added;
}
