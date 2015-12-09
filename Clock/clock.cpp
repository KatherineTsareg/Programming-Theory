#include "stdafx.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

const int WINDOW_X = 400;
const int WINDOW_Y = WINDOW_X;
const int START_X = WINDOW_X / 2;
const int START_Y = WINDOW_Y / 2;
const int AMOUNT_POINTS = 60;
const int SCALE = 180;

struct Shapes {
	RectangleShape secArrow;
	RectangleShape minArrow;
	RectangleShape hourArrow;
	CircleShape centre;
	CircleShape point;
}watch;

void CoordPoints(Vector2f(&coordinatePoints)[AMOUNT_POINTS]) {
	Vector2f coordinatePoint;
	for (int i = 0; i < AMOUNT_POINTS; i++) {
		coordinatePoint.x = START_X + SCALE * cos(i * 6 * float(M_PI) / 180);
		coordinatePoint.y = START_Y + SCALE * sin(i * 6 * float(M_PI) / 180);
		coordinatePoints[i] = { coordinatePoint.x, coordinatePoint.y };
	}
}

void DrawPoints(RenderWindow &window, Vector2f(&coordinatePoints)[AMOUNT_POINTS]) {
	for (int i = 0; i < AMOUNT_POINTS; i++) {
		if (i % 15 == 0) {
			watch.point.setRadius(6);
			watch.point.setOrigin(6 / 2, 6 / 2);
			watch.point.setFillColor(Color::White);
		}
		else if (i % 5 == 0) {
			watch.point.setRadius(4);
			watch.point.setOrigin(4 / 2, 4 / 2);
			watch.point.setFillColor(Color::White);
		}
		else {
			watch.point.setRadius(1);
			watch.point.setOrigin(1 / 2, 1 / 2);
			watch.point.setFillColor(Color::White);
		}
		watch.point.setPosition(coordinatePoints[i].x, coordinatePoints[i].y);
		window.draw(watch.point);
	}
}

void PositionOneArrow(RectangleShape &arrow, float height, float width) {
	arrow.setSize(Vector2f(height, width));
	arrow.setOrigin(Vector2f(0, 2));
	arrow.setPosition(float(START_X), float(START_Y));
}

void PositionArrows() {
	PositionOneArrow(watch.hourArrow, 100, 6);
	watch.hourArrow.setFillColor(Color::White);
	PositionOneArrow(watch.minArrow, 130, 4);
	watch.minArrow.setFillColor(Color::White);
	PositionOneArrow(watch.secArrow, 150, 2);
	watch.secArrow.setFillColor(Color::Red);

	watch.centre.setRadius(7);
	watch.centre.setFillColor(Color::White);
	watch.centre.setPosition((WINDOW_X / 2) - watch.centre.getRadius(), (WINDOW_Y / 2) - watch.centre.getRadius());
}

void TimeIsOn(RenderWindow &window)
{
	PositionArrows();
	Vector2f coordinatePoints[AMOUNT_POINTS];
	CoordPoints(coordinatePoints);
	SYSTEMTIME sysTime;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		GetSystemTime(&sysTime);
		watch.secArrow.setRotation(float(sysTime.wSecond * 360 / 60 - 90));
		watch.minArrow.setRotation(float(sysTime.wMinute * 360 / 60 - 90 + sysTime.wSecond * 6 / 60));
		watch.hourArrow.setRotation(float((sysTime.wHour + 3) * 30 + (sysTime.wMinute * 30 / 60) - 90));
		std::cout << sysTime.wHour + 3 << ":" << sysTime.wMinute << ":" << sysTime.wSecond << "\n";

		window.clear(Color(43, 40, 61));
		window.draw(watch.hourArrow);
		window.draw(watch.minArrow);
		window.draw(watch.secArrow);
		window.draw(watch.centre);
		DrawPoints(window, coordinatePoints);

		window.display();
	}
}

int main()
{
	RenderWindow window(VideoMode(WINDOW_X, WINDOW_Y), "Clock");
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	TimeIsOn(window);
	return 0;
}