#pragma once

#include <SFML/Graphics.hpp>
#include "CONSTANTS.h"

using namespace sf;

class Slot : public CircleShape
{
public:
	Slot(float radius) : CircleShape{ radius }, upperLeft{ 0,0 }, bottomLeft{ 0,0 } {}

	
	// used for winning check

	// the upper left point of the 'X' shape of the slot
	Vector2i upperLeft;
	// the bottom left point of the 'X' shape of the slot
	Vector2i bottomLeft;
};

