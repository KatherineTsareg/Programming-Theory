#include <SFML/Graphics.hpp>
#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Const.h"
#include "CarModel.h"

using namespace sf;

struct GraphicResource {
	Image* im;
	Texture* tex;
	Sprite* spr;
};

GraphicResource MakeTheSprite(String fileName, int i, int j, int width, int height) {
	GraphicResource gr;
	gr.im = new Image();
	gr.tex = new Texture();
	gr.spr = new Sprite();
	gr.im->loadFromFile("images/" + fileName);
	gr.tex->loadFromImage(*gr.im, IntRect(i, j, width, height));
	gr.tex->setSmooth(true);
	gr.spr->setTexture(*gr.tex);
	return gr;
}

void CleanTheMemory(GraphicResource & gr) {
	delete gr.im;
	delete gr.tex;
	delete gr.spr;
}

void PositionTheCarElements()
{
	car.frontWheel.setRadius(FRONT_WHEEL_RADIUS);
	car.frontWheel.setOrigin(FRONT_WHEEL_RADIUS, FRONT_WHEEL_RADIUS);
	
	car.rearWheel.setRadius(REAR_WHEEL_RADIUS);
	car.rearWheel.setOrigin(REAR_WHEEL_RADIUS, REAR_WHEEL_RADIUS);
	
}

void DrawTheCar(RenderWindow &window) {
	window.clear(Color::White);
	window.draw(car.carBody);
	window.draw(car.frontWheel);
	window.draw(car.rearWheel);
	window.display();
}

void MovementTheAuto(RenderWindow &window)
{
	PositionTheCarElements();
	Clock clock;
	
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 1000;

		car.CalculateTheSpeed(event, time);
		DrawTheCar(window);

	}
}

int main()
{
	RenderWindow window(VideoMode(WINDOW_X, WINDOW_Y), "Movement of the car");
		
	GraphicResource bodyGR = MakeTheSprite("car.psd", 0, 0, 330, 100);
	car.carBody = *bodyGR.spr;

	GraphicResource frontWheelGR = MakeTheSprite("2 wheel.psd", 0, 0, 49, 49);
	car.frontWheel.setTexture(&(*frontWheelGR.tex)); 
	
	GraphicResource rearWheelGR = MakeTheSprite("1 wheel.psd", 0, 0, 58, 58);
	car.rearWheel.setTexture(&(*rearWheelGR.tex));
	
	MovementTheAuto(window);
	
	CleanTheMemory(bodyGR);
	CleanTheMemory(frontWheelGR);
	CleanTheMemory(rearWheelGR);

	return 0;
}