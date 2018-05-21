#include "util.h"

int min_index_of_three(float x, float y, float z) {
	float min = x;
	int index = 0;

	if (y < min) {
		min = y;
		index = 1;
	}

	if (z < min) {
		min = z;
		index = 2;
	}

	return index;
}