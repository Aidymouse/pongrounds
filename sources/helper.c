#include <stdlib.h>
#include "helper.h"
#include "raylib.h"

int randInt(int lower, int upper) {
	return rand() % (upper - lower + 1) + lower;
}

void DrawTextCentered(const char *t, int x, int y, int fontSize, Color color) {
	int width = MeasureText(t, fontSize);
	DrawText(t, x-width/2, y, fontSize, color);
}
