#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include "Var.h"

using namespace sf;


void step_one(RectangleShape &rectangle, float time, float &y, int &step)
{
	if (y > WINDOW_SIZE.y - RECTANGLES_SIZE.y / 2) {
		rectangle.setFillColor(Color::Magenta);
		step = MOVE_RIGHT_AND_UP;
	}
	else
		y += time * 0.1;
}

void step_two(RectangleShape &rectangle, float time, float &x, float &y, int &step, int number_block, float &rotation)
{
	if (x < WINDOW_SIZE.x - RECTANGLES_SIZE.x / 2) {
		x += time * 0.08;
		if (y > WINDOW_SIZE.y / 2)
			y -= time * 0.08;	
		else
			rotation += time * 0.105;
	}
	else {
		rotation = 540;
			step = MOVE_RIGHT_AND_TURN;
			rectangle.setFillColor(Color::Yellow);
	}
}

void step_three(RectangleShape &rectangle, float time, int number_block, float &x, float &y, int &step)
{
	if (y < WINDOW_SIZE.y - RECTANGLES_SIZE.y / 2)
	{
		y += time * 0.1;
	}
	else
		step = MOVE_DOWN_AND_CHANGE_SCALE;
}

void step_four(RectangleShape &rectangle, float time, int number_block, float &x, float &y, int &step, float &size_y)
{
	if ((x > RECTANGLES_SIZE.x / 2) && (y > RECTANGLES_SIZE.y / 2))
	{
		x -= time * 0.1;
		y -= time * 0.1;
		if (y > WINDOW_SIZE.y / 2)
			size_y += time * 0.001;
		else
			size_y -= time * 0.001;
	}
	else {
		step = MOVE_LEFT;
		size_y = 1;
	}
}

void step_five(RectangleShape &rectangle, float time, int number_block, float &x, float &y, int &step, float &size_y)
{
	if (x > RECTANGLES_SIZE.x / 2)
		x -= time * 0.1;
	else {
		step = MOVE_DOWN;
		rectangle.setFillColor(Color::Blue);
	}
}

struct initialization_blocks {
	RectangleShape rectangle;
	Vector2f pos;
	int step = 1;
	float rotation = 0;
	float size_y = 1;
	float size_x = 1;

	initialization_blocks(float x, float y, int count)
	{
		rectangle.setSize(RECTANGLES_SIZE);
		rectangle.setFillColor(Color::Blue);
		rectangle.setOrigin(RECTANGLES_SIZE.x / 2, RECTANGLES_SIZE.y / 2);
		pos.x = x;
		pos.y = y;
	}

	void update(float time, int number_block)
	{
		if (step == MOVE_DOWN)
			step_one(rectangle, time, pos.y, step);
		else if (step == MOVE_RIGHT_AND_UP)
			step_two(rectangle, time, pos.x, pos.y, step, number_block, rotation);
		else if (step == MOVE_RIGHT_AND_TURN)
			step_three(rectangle, time, number_block, pos.x, pos.y, step);
		else if (step == MOVE_DOWN_AND_CHANGE_SCALE)
			step_four(rectangle, time, number_block, pos.x, pos.y, step, size_y);
		else if (step == MOVE_LEFT)
			step_five(rectangle, time, number_block, pos.x, pos.y, step, size_y);
		rectangle.setScale(size_x, size_y);
		rectangle.setRotation(rotation);
		rectangle.setPosition(pos.x, pos.y);
	};

};

void run_animation(RenderWindow &window)
{
	Clock clock;
	std::vector<initialization_blocks*>  object;
	float x = RECTANGLES_SIZE.x / 2, y = 0;
	int number_block;
	for (int i = 0; i < NUMBER_BLOCKS; i++) {
		y += 10 + RECTANGLES_SIZE.y;
		object.push_back(new initialization_blocks(x, y, i));
	}
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		
		window.clear(Color::White);
		number_block = 0;
		for (auto i : object) {
			i->update(time, number_block);
			window.draw(i->rectangle);
			number_block++;
		}
		window.display();
	}
}

int main()
{
	RenderWindow window(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Animation");
	run_animation(window);
	return 0;
}