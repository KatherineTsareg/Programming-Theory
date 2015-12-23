#pragma once
#include <SFML/Graphics.hpp> 
#include "const.h"

using namespace sf;

struct Pendulum {
	CircleShape left_cogwheel, right_cogwheel, cargo;
	ConvexShape base, left_arrow, right_arrow;
	float rotation = 30;
	float speed = -0.001;
	float acceleration = -0.0005;
	bool downhill = true;
}pendulum;

void init_cogwheel(CircleShape &circle, float pos_x, float pos_y);
void init_pendulum();
void position_convex(ConvexShape &convex);
void update();
void calculation_speed(float &speed, float &rotation, float &acceleration, bool &downhill);
void update_rotation(float rotation);
void draw_pendulum(RenderWindow &window);