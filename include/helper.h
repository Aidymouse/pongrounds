#include "raylib.h"

int randInt(int lower, int higher);
void DrawTextCentered(const char* t, int x, int y, int fontSize, Color color);
float get_angle_distance(float angle1, float angle2);
const char *strconcat(const char *s1, const char *s2);
void DrawPolygon(const Vector2 *points, int num_points, float thickness, Color color);
bool is_heading_towards(Vector2 obj_pos, Vector2 obj_dir, Vector2 target_pos);
bool angle_is_between_two_others(float angle, float other1, float other2);
