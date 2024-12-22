#pragma once

#include "CONSTANTS.h"

class Column
{
public:
	Column();

	void update_row();

	// x initial
	float xi;
	// x final
	float xf;

	bool isFilled;

	// empty row
	int topRow;
};

