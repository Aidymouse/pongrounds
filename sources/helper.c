#include <stdlib.h>
#include <stdio.h>
#include "helper.h"
#include "raylib.h"
#include <math.h>

int randInt(int lower, int upper) {
	return rand() % (upper - lower + 1) + lower;
}

void DrawTextCentered(const char *t, int x, int y, int fontSize, Color color) {
	int width = MeasureText(t, fontSize);
	DrawText(t, x-width/2, y, fontSize, color);
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
	if (abs(dist1) < abs(dist2)) { return dist1; } else { return dist2; }
}

