#include <stdlib.h>
#include "helper.h"

int randInt(int lower, int upper) {
	return rand() % (upper - lower + 1) + lower;
}
