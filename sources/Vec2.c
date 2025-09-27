#include "Vec2.h"
#include <math.h>

double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}
double to_radians(double degrees) {
	return degrees / (180.0 / M_PI);
}

Vector2 Vec2Add(Vector2 v1, Vector2 v2) {
	Vector2 added;
	added.x = v1.x + v2.x;
	added.y = v1.y + v2.y;
	return added;
}

Vector2 Vec2AddScalar(Vector2 v1, float s) {
	Vector2 added;
	added.x = v1.x + s;
	added.y = v1.y + s;
	return added;
}

Vector2 Vec2MultScalar(Vector2 v, float s) {
	Vector2 mult;
	mult.x = v.x * s;
	mult.y = v.y * s;
	return mult;
}

float Vec2GetAngle(Vector2 v) {
	// Get's angle in degrees assuming ? is 0
	float a = atan2(v.y, v.x);
	return to_degrees(a);
}

float Vec2GetMagnitude(Vector2 v) {
	return sqrt(v.x*v.x + v.y*v.y);
}

Vector2 Vec2Normalize(Vector2 v) {
	float mag = Vec2GetMagnitude(v);
	Vector2 norm;
	norm.x = v.x / mag;
	norm.y = v.y / mag;
	return norm;
}

Vector2 Vec2Rotate(Vector2 v, float degrees) {
	Vector2 rotated;
	float rad = to_radians(degrees);
	rotated.x = v.x * cos(rad) - v.y * sin(rad);
	rotated.y = v.x * sin(rad) + v.y * cos(rad);
	return rotated;
}

/** Assumes ?? is direclty right. Gets a unit vector */
Vector2 GetVec2FromAngle(float degrees) {
	// Get adjacent
	float rads = to_radians(degrees);

	float x = cos(rads);
	float y = sin(rads);

	Vector2 from_angle;
	from_angle.x = x;
	from_angle.y = y;
	return from_angle;

}

Vector2 Vec2Sub(Vector2 v1, Vector2 v2) {
	Vector2 subbed;
	subbed.x = v1.x - v2.x;
	subbed.y = v1.y - v2.y;
	return subbed;
}
