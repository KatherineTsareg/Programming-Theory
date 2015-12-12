#include <SFML/Graphics.hpp>
#include<iostream>

using namespace sf;

const int WINDOW_X = 1280;
const int WINDOW_Y = 400;
const int rearWheelPos = 64;
const int frontWheelPos = 271;
const int bodyPos = 0;


struct Car{
	Sprite carBody;
	CircleShape frontWheel;
	CircleShape rearWheel;
}car;

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
	car.frontWheel.setRadius(49 / 2);
	car.frontWheel.setOrigin(49 / 2, 49 / 2);
	
	car.rearWheel.setRadius(58 / 2);
	car.rearWheel.setOrigin(58 / 2, 58 / 2);
	
}

void MovementElements(float rotation)
{
	car.carBody.setPosition(bodyPos + rotation, WINDOW_Y - 100 - 20);
	
	car.frontWheel.setPosition(frontWheelPos + rotation, WINDOW_Y - 49 / 2 - 2 - 20);
	car.frontWheel.setRotation(rotation);
	
	car.rearWheel.setPosition(rearWheelPos + rotation, WINDOW_Y - 58 / 2 - 20);
	car.rearWheel.setRotation(rotation);
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
	bool posit = true;
	float rotation = 0;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		MovementElements(rotation);
		DrawTheCar(window);

		if ((rotation < WINDOW_X) && (posit))
			rotation += 0.1f;
		else if ((rotation < -300) && (!posit)) {
			rotation += 0.1f;
			posit = true;
		}
		else{
			rotation -= 0.1f;
			posit = false;
		}

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