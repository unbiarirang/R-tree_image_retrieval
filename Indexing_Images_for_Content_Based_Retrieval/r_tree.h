#pragma once

#include "point.h"
#include "rectangle.h"

class r_tree
{
public:
	r_tree();
	~r_tree();

	rectangle* root;
};

