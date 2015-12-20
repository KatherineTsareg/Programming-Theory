#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
const Vector2f RECTANGLES_SIZE = { 50, 30 };
const Vector2f WINDOW_SIZE = { 700, 500 };
const int NUMBER_BLOCKS = 6;
const int MOVE_DOWN = 1;
const int MOVE_RIGHT_AND_UP = 2;
const int MOVE_RIGHT_AND_TURN = 3;
const int MOVE_DOWN_AND_CHANGE_SCALE= 4;
const int MOVE_LEFT= 5;