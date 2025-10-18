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

/** Gets closest distance angle1 needs to change (in degrees) to be pointing in the same direction as angle 2, assuming negative is rotating anti-clockwise and positive rotating clockwise.
 * NOTE: capped so it won't rotate more than once i.e. shortest rotation to match direction, not literal angle
 * */
float get_angle_distance(float angle1, float angle2) {

	// Make all angles so they're from 0 to 360 (360 = 0)
	// 0 = pointing right
	angle1 = fmod(angle1, 360);
	angle2 = fmod(angle2, 360);
	if (angle1 < 0) { angle1 = 360 + angle1; }
	if (angle2 < 0) { angle2 = 360 + angle2; }

	//printf("Angle1: %.2f, Angle2: %.2f\n", angle1, angle2);

	float dist1 = angle2 - angle1;
	float dist2 = -(360 - dist1);
	if (dist1 < 0) {
		dist2 = 360 + dist1;
	}

	//printf("Dist1: %.2f, Dist2: %.2f\n", dist1, dist2);

	if (fabsf(dist1) < fabsf(dist2)) { return dist1; } else { return dist2; }
}


/** */
bool is_heading_towards(Vector2 obj_pos, Vector2 obj_dir, Vector2 target_pos) {
	Vector2 to_target = Vec2Sub(target_pos, obj_pos);
	float ang_target = Vec2GetAngle(to_target);
	float ang_dir = Vec2GetAngle(obj_dir);
	return ( fabsf(get_angle_distance(ang_target, ang_dir)) < 90);
}

int signi(int i) {
	if (i<0) { return -1; }
	if (i == 0) { return 0; }
	return 1;
}

int singf(float f) {
	if (f<0) { return -1; }
	if (f == 0) { return 0; }
	return 1;
}
