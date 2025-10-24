#include "helper.h"
#include "raylib.h"
#include <stdio.h>
#include "Vec2.h"

int main() {
	Vector2 v1 = { 1, 0 };	
	Vector2 v2 = { 0, 1 };	

	printf("V1: %f\n", Vec2GetAngle(v1));
	printf("V2: %f\n", Vec2GetAngle(v2));
	printf("%f\n", Vec2GetAngle(Vec2Sub(v1, v2)));
}
