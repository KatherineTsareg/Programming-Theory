#include <SFML/Graphics.hpp>
#include "pendulum.h"

using namespace sf;


void init_cogwheel(CircleShape &circle, float pos_x, float pos_y)
{
	circle.setRadius(150);
	circle.setOrigin(150, 150);
	circle.setPosition(pos_x, pos_y);
}

void init_pendulum()
{
	init_cogwheel(pendulum.left_cogwheel, INITAL_POSITION_LEFT_COGWHEEL.x, INITAL_POSITION_LEFT_COGWHEEL.y);
	init_cogwheel(pendulum.right_cogwheel, INITAL_POSITION_RIGHT_COGWHEEL.x, INITAL_POSITION_RIGHT_COGWHEEL.y);
	pendulum.right_cogwheel.scale(0.8f, 0.8f);

	pendulum.base.setPointCount(3);
	pendulum.base.setPoint(0, Vector2f(110, 200));
	pendulum.base.setPoint(1, Vector2f(120, 0));
	pendulum.base.setPoint(2, Vector2f(130, 200));
	position_convex(pendulum.base);

	pendulum.cargo.setRadius(60);
	pendulum.cargo.setOrigin(60, -180);
	pendulum.cargo.setPosition(INITAL_POSITION_PENDULUM.x, INITAL_POSITION_PENDULUM.y);
	pendulum.cargo.setFillColor(Color::Green);

	pendulum.left_arrow.setPointCount(3);
	pendulum.left_arrow.setPoint(0, Vector2f(0, 90));
	pendulum.left_arrow.setPoint(1, Vector2f(120, 0));
	pendulum.left_arrow.setPoint(2, Vector2f(120, 50));
	position_convex(pendulum.left_arrow);

	pendulum.right_arrow.setPointCount(3);
	pendulum.right_arrow.setPoint(0, Vector2f(240, 90));
	pendulum.right_arrow.setPoint(1, Vector2f(120, 0));
	pendulum.right_arrow.setPoint(2, Vector2f(120, 50));
	position_convex(pendulum.right_arrow);
}

void position_convex(ConvexShape &convex)
{
	convex.setPosition(INITAL_POSITION_PENDULUM.x, INITAL_POSITION_PENDULUM.y);
	convex.setFillColor(Color::Green);
	convex.setOrigin(120, 0);
}

void update()
{
	calculation_speed(pendulum.speed, pendulum.rotation, pendulum.acceleration, pendulum.downhill);
	update_rotation(pendulum.rotation);
}

void calculation_speed(float &speed, float &rotation, float &acceleration, bool &downhill)
{
	if (int(speed * 1000) == 0)
	{
		speed = pendulum.acceleration * 2;
		downhill = true;
	}
	if (int(rotation) == 0 && downhill)
	{
		speed += acceleration * 10;
		acceleration = -acceleration;
		downhill = false;
	}
	speed += acceleration;
	rotation += speed;
}

void update_rotation(float rotation)
{
	pendulum.base.setRotation(rotation);
	pendulum.cargo.setRotation(rotation);
	pendulum.left_arrow.setRotation(rotation);
	pendulum.right_arrow.setRotation(rotation);
	pendulum.left_cogwheel.setRotation(rotation);
	pendulum.right_cogwheel.setRotation(-rotation);
}

void draw_pendulum(RenderWindow &window)
{
	window.draw(pendulum.base);
	window.draw(pendulum.left_arrow);
	window.draw(pendulum.right_arrow);
	window.draw(pendulum.cargo);

	window.draw(pendulum.left_cogwheel);
	window.draw(pendulum.right_cogwheel);
}