#include <SFML/Graphics.hpp> 
#include "pendulum.h"

#include "const.h"

using namespace sf;

void draw_elements(RenderWindow &window)
{
	window.clear(Color::White);
	draw_pendulum(window);
	window.display();
}

void start_move(RenderWindow &window)
{
	init_pendulum();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		update();
		draw_elements(window);
	}
}

int main() {
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	Texture cogwheels;
	cogwheels.loadFromFile("cogwheel.psd");
	pendulum.left_cogwheel.setTexture(&cogwheels);
	pendulum.right_cogwheel.setTexture(&cogwheels);

	RenderWindow window(VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y), "Pendulum", Style::Default, settings);
	start_move(window);
	return 0;
}

