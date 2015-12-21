#pragma once
#include <SFML/Graphics.hpp> 
#include "const.h"

using namespace sf;

struct Pendulum {
	CircleShape left_cogwheel, right_cogwheel;
	ConvexShape base, left_arrow, right_arrow;
	float rotation = 30, rot_gear_left = 0, rot_gear_right = 26;
	float speed = -0.001;
	float acceleration = -0.0005;
	bool downhill = true;

	void init_elements_of_pendulum()
	{
		init_cogwheel(left_cogwheel, INITAL_POSITION_LEFT_COGWHEEL.x, INITAL_POSITION_LEFT_COGWHEEL.y);
		init_cogwheel(right_cogwheel, INITAL_POSITION_RIGHT_COGWHEEL.x, INITAL_POSITION_RIGHT_COGWHEEL.y);
		right_cogwheel.scale(0.8f, 0.8f);
		init_pendulum();
		left_cogwheel.setRotation(rot_gear_left);
		right_cogwheel.setRotation(rot_gear_right);
	}

	void init_cogwheel(CircleShape &circle, float pos_x, float pos_y)
	{
		circle.setRadius(150);
		circle.setOrigin(150, 150);
		circle.setPosition(pos_x, pos_y);
	}

	void init_pendulum()
	{
		base.setPointCount(3);
		base.setPoint(0, Vector2f(100, 300));
		base.setPoint(1, Vector2f(120, 0));
		base.setPoint(2, Vector2f(140, 300));
		position_convex(base);

		left_arrow.setPointCount(3);
		left_arrow.setPoint(0, Vector2f(0, 90));
		left_arrow.setPoint(1, Vector2f(120, 0));
		left_arrow.setPoint(2, Vector2f(120, 50));
		position_convex(left_arrow);

		right_arrow.setPointCount(3);
		right_arrow.setPoint(0, Vector2f(240, 90));
		right_arrow.setPoint(1, Vector2f(120, 0));
		right_arrow.setPoint(2, Vector2f(120, 50));
		position_convex(right_arrow);
	}

	void position_convex(ConvexShape &convex)
	{
		convex.setPosition(INITAL_POSITION_PENDULUM.x, INITAL_POSITION_PENDULUM.y);
		convex.setFillColor(Color::Green);
		convex.setOrigin(120, 0);
	}

	void update()
	{
		calculation_speed();
		update_rotation();
	}
	
	void calculation_speed() 
	{
		if (int(speed * 1000) == 0) 
		{
			speed = acceleration * 2;
			downhill = true;
		}
		if (int(rotation) == 0 && downhill) 
		{
			speed += acceleration * 10;
			acceleration = -acceleration;
			downhill = false;
		}
		rot_gear_left -= acceleration * 100;
		rot_gear_right += acceleration * 100;
		speed += acceleration;
		rotation += speed;
	}

	void update_rotation()
	{
		base.setRotation(rotation);
		left_arrow.setRotation(rotation);
		right_arrow.setRotation(rotation);
		left_cogwheel.setRotation(rotation);
		right_cogwheel.setRotation(-rotation);
	}

}pendulum;