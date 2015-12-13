#pragma once
#include <SFML/Graphics.hpp>
#include "Const.h"

using namespace sf;

struct Car {
	Sprite carBody;
	CircleShape frontWheel;
	CircleShape rearWheel;

	float acceleration = 0.0005;
	float rotation = 0;
	float speed = 0;
	bool speedReduction;

	Vector2f carCoord;

	void CalculateTheSpeed(Event event, float time)
	{
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right)
		{
			speed += acceleration * time;
			speedReduction = true;
		}
		else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left)
		{
			speed -= acceleration * time;
			speedReduction = false;
		}
		else {

			if ((speed > 0) && (speedReduction))
			{
				speed -= (acceleration * time) / 2;
			}
			if ((speed < 0) && (!speedReduction))
				speed += (acceleration * time) / 2;
		}
		CalculateTheRotation(event, time);
	}

	void CalculateTheRotation(Event event, float time)
	{
		carCoord.x += speed;
		rotation = carCoord.x * 360 / CIRCLE_LENGTH;
		MovementElements();
	}

	void MovementElements()
	{
		car.carBody.setPosition(BODY_X + carCoord.x, WINDOW_Y - 100);

		car.frontWheel.setPosition(FRONT_WHEEL_X + carCoord.x, WINDOW_Y - FRONT_WHEEL_RADIUS - 2);
		car.frontWheel.setRotation(rotation);

		car.rearWheel.setPosition(REAR_WHEEL_X + carCoord.x, WINDOW_Y - REAR_WHEEL_RADIUS);
		car.rearWheel.setRotation(rotation);
	}
}car;
