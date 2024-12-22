#include "Ball.h"
#include "Textures.h"
#include "CONSTANTS.h"
#include "Column.h"

Ball::Ball(BallColor color)
: CircleShape(CIRCLE_RADIUS){
	if (color == Red)
	{
		setTexture(&Textures::redBallTexture);
	}
	else if(color == Yellow)
	{
		setTexture(&Textures::yellowBallTexture);
		setRadius(CIRCLE_RADIUS - 3);
	}

	setPosition({INITIAL_X, HOFB});

	// make transparent
	setFillColor(Color(255, 255, 255, 100));
}