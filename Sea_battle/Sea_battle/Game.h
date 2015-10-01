#pragma once

using namespace sf;

typedef enum
{
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
} Direction;

struct Field
{
	unsigned width;
	unsigned height;
	char *fieldPlayer;
	char *fieldComputer;
	RectangleShape *rects;
};

struct Pacman
{
	CircleShape *shape;
	Direction direction;
};

struct Game
{
	Pacman pacman;
	Field field;
};
