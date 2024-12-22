#pragma once

// window resolution
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

// number of circles
const unsigned char HORIZONTAL_CIRCLES = 7;
const unsigned char VERTICAL_CIRCLES = 6;

// space between cricles
const float HORIZONTAL_SPACE_BETWEEN_CIRCLES = 7;
const float VERTICAL_SPACE_BETWEEN_CIRCLES = 5;

//initial y
const float INITIAL_Y = WINDOW_HEIGHT * 15 / 100;

// circle radius
const float CIRCLE_RADIUS = (WINDOW_HEIGHT - (VERTICAL_SPACE_BETWEEN_CIRCLES * (VERTICAL_CIRCLES - 1)) - (INITIAL_Y)) / (VERTICAL_CIRCLES * 2);

// grid size
const float GRID_WIDTH = HORIZONTAL_CIRCLES * CIRCLE_RADIUS * 2 + (HORIZONTAL_CIRCLES - 1) * HORIZONTAL_SPACE_BETWEEN_CIRCLES;
const float GRID_HEIGHT = VERTICAL_CIRCLES * CIRCLE_RADIUS * 2 + (VERTICAL_CIRCLES - 1) * VERTICAL_SPACE_BETWEEN_CIRCLES;

const float COLUMN_WIDTH = (2 * CIRCLE_RADIUS) + (HORIZONTAL_SPACE_BETWEEN_CIRCLES);

// initial x
const float INITIAL_X = (WINDOW_WIDTH - GRID_WIDTH) / 2;

// Height of the falling balls
const float HOFB = INITIAL_Y - 5 - (2 * CIRCLE_RADIUS);

// Flag to use as a function return
const int RED_FLAG = -1;