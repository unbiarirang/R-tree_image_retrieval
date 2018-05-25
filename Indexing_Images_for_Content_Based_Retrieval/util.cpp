#include "util.h"

int min_index_of_three(double x, double y, double z) {
	double min = x;
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