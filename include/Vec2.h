#include "raylib.h"
#include <math.h>

double to_degrees(double radians);
double to_radians(double degrees);
Vector2 Vec2Add(Vector2 v1, Vector2 v2);
Vector2 Vec2Sub(Vector2 v1, Vector2 v2);
Vector2 Vec2AddScalar(Vector2 v1, float s);
Vector2 Vec2MultScalar(Vector2 v, float s);
float Vec2GetAngle(Vector2 v);
float Vec2GetMagnitude(Vector2 v);
Vector2 Vec2Normalize(Vector2 v);
Vector2 Vec2Rotate(Vector2 v, float degrees);
Vector2 GetVec2FromAngle(float degrees);
