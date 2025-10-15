#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "raylib.h"
#include <math.h>
#include "Vec2.h"

int randInt(int lower, int upper) {
	return rand() % (upper - lower + 1) + lower;
}

void DrawTextCentered(const char *t, int x, int y, int fontSize, Color color) {
	int width = MeasureText(t, fontSize);
	DrawText(t, x-width/2, y-fontSize/2, fontSize, color);
}

void DrawPolygon(const Vector2 *points, int num_points, float thickness, Color color) {
	for (int p=0; p<num_points; p++) {
	}
}

/** Gets closest distance in degrees between two angles, assuming negative is rotating anti-clockwise and positive rotating clockwise */
float get_angle_distance(float angle1, float angle2) {


	// Normalize all angles so they're from 0 to 360
	angle1 = fmod(angle1, 360);
	angle2 = fmod(angle2, 360);
	if (angle1 < 0) { angle1 = 360 + angle1; }
	if (angle2 < 0) { angle2 = 360 + angle2; }

	float dist1 = angle2 - angle1;
	float dist2 = (angle2+360) - angle1;
	if (fabsf(dist1) < fabsf(dist2)) { return dist1; } else { return dist2; }
}


/** */
bool is_heading_towards(Vector2 obj_pos, Vector2 obj_dir, Vector2 target_pos) {
	Vector2 to_target = Vec2Sub(target_pos, obj_pos);
	float ang_target = Vec2GetAngle(to_target);
	float ang_dir = Vec2GetAngle(obj_dir);
	return ( fabsf(get_angle_distance(ang_target, ang_dir)) < 90);
}
