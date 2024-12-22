#pragma once

#include <SFML/Graphics.hpp>
#include "Column.h"

using namespace sf;

class Ball : public CircleShape
{
public:
	enum BallColor { Red, Yellow};
	Ball(BallColor color);
};
