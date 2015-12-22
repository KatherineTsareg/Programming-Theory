#include <SFML/Graphics.hpp> 
#include "pendulum.h"
#include "const.h"

using namespace sf;

void draw_elements(RenderWindow &window)
{
	window.clear(Color::White);

	window.draw(pendulum.base);
	window.draw(pendulum.left_arrow);
	window.draw(pendulum.right_arrow);
	window.draw(pendulum.cargo);

	window.draw(pendulum.left_cogwheel);
	window.draw(pendulum.right_cogwheel);

	window.display();
}

void start_move(RenderWindow &window)
{
	pendulum.init_elements_of_pendulum();
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		pendulum.update();
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

