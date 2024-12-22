#include "Column.h"

Column::Column()
	: xi{ 0 }, xf{ 0 }, isFilled{ false }, topRow{ VERTICAL_CIRCLES - 1 }
{}

void Column::update_row()
{
	topRow--;
	if (topRow == -1)
		isFilled = true;
}