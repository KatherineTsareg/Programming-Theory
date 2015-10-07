#include <SFML/Graphics.hpp>
#include "Map.h"
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <random> 
#include <ctime>


using namespace sf; //включаем пространство имен sf, чтобы посто€нно не писать sf::
using namespace std;


const int maxLengthShip = 4;
const int maxNumberSingleShip = 20;

const float BLOCK_SIZE = 32.f;
const int WindowHight = BLOCK_SIZE * HEIGHT_MAP;
const int WindowWidth = BLOCK_SIZE * WIDTH_MAP;


void render(RenderWindow & window, Sprite & s_map)
{
	window.clear();

	/////////////////////////////–исуем карту/////////////////////
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP;j++)
		{
			if (TileMap[i][j] == 's')
				s_map.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили 's', то рисуем картинку клеточки
			if (TileMap[i][j] == 'm')
				s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили 'm', то рисуем картинку дерева
			if ((TileMap[i][j] == '0'))
				s_map.setTextureRect(IntRect(32, 0, 32, 32));//если встретили '0', то рисуем картинку клеточки
			if (TileMap[i][j] == 'd')
				s_map.setTextureRect(IntRect(96, 0, 128, 32));//если встретили 'd', то рисуем картинку корабл€(крестик)
			if ((TileMap[i][j] == '1'))
				s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили '1', то рисуем картинку "мимо" (точка)
			if ((TileMap[i][j] == '2'))
				s_map.setTextureRect(IntRect(96, 0, 32, 32));//если встретили '2', то рисуем картинку "попал" (крестик)
			if ((TileMap[i][j] == '3'))
				s_map.setTextureRect(IntRect(96, 0, 32, 32));//если встретили '2', то рисуем картинку "попал" (крестик)
			s_map.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);//раскидывает квадратики, превраща€ в карту. то есть задает каждому из них позицию. если убрать, то вс€ карта нарисуетс€ в одном квадрате 32*32 и мы увидим один квадрат

			window.draw(s_map);//рисуем квадратики на экран
		};
	window.display();
}

void text(RenderWindow & window, Sprite & s_map)
{
	/*std::ostringstream fourDeckString;    // объ€вили переменную
	fourDeckString << p.ostringstream;		//занесли в нее число очков, то есть формируем строку
	text.setString(":" + fourDeckString.str());//задаем строку тексту и вызываем сформированную выше строку методом .str()
	text.setPosition(0, 0);//задаем позицию текста, центр камеры
	window.draw(text);//рисую этот текст*/
}

void placementShipPlayers(int i, int j, int ship) { //игрок расставл€ет свои корабли
	if ((TileMap[i][j] == '0') && (TileMap[i - 1][j - 1] != '2') && (TileMap[i - 1][j + 1] != '2') && (TileMap[i + 1][j - 1] != '2') && (TileMap[i + 1][j + 1] != '2')) {
		if (ship < maxNumberSingleShip)
		{
			ship ++;
			TileMap[i][j] = '2';
			std::cout << "SHIP=" << ship << "\n";
		}
	}
}

void seachShip(int i, int j) {
	if (TileMap[i][j] == '0')
		TileMap[i][j] = '1';
	else if (TileMap[i][j] == 'X')
		TileMap[i][j] = '2';
}

void alignmentOfComputerOneShip(int LengthShip)//расстановка кораблей компьютером
{
	for (int k = 1; k <= (5 - LengthShip); k++) {
		srand(time(NULL));
		int i = rand() % 10 + 3;
		int j = rand() % 10 + 12;
		int location = rand() % 2; //расположение корабл€, 0 - горизонтально, 1 - вертикально
		if ((j + LengthShip) > 21)
			j = 21;
		else
			j = j + LengthShip;
		if (LengthShip > 0)
			std::cout << "i=" << i << ",j=" << j << ",LEN= " << LengthShip << ",loc= " << location << "\n";
		if ((i <= 12) && (j <= 21) && (TileMap[i][j] == '0')) { //&& (TileMap[i - 1][j - 1] != 'X') && (TileMap[i - 1][j + 1] != 'X') && (TileMap[i + 1][j - 1] != 'X') && (TileMap[i + 1][j + 1] != 'X')) {
			std::cout << "YES YES TES\n";
			if (location != 1) {
				std::cout << "WHAT IS WRONG?\n";
				if ((TileMap[i - 1][j] != 'X') && (TileMap[i + 1][j] != 'X')) {
					TileMap[i][j] = 'X';
					LengthShip--;
					j--;
				}
			}
			else
			{
				if ((TileMap[i][j - 1] != 'X') && (TileMap[i][j + 1] != 'X')) {
					TileMap[i][j] = 'X';
					LengthShip--;
					j--;
				}
			}
		}else {
			std::cout << "FAIL\n";
			srand(time(NULL));
			int i = rand()%10 + 3;
			srand(time(NULL));
			int j = rand()%10 + 12;
			srand(time(NULL));
			int location = rand()%2;
		}
	}
}

	void workWithMap(RenderWindow & window, Sprite & s_map, int ship){
	if (sf::Mouse::isButtonPressed(Mouse::Left)){
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		if (((localPosition.x >= 0) && (localPosition.y >= 0)) && ((localPosition.x <= WindowWidth) && (localPosition.y <= WindowHight))) {
			int i = (localPosition.y / 32);
			int j = (localPosition.x / 32);
			std::cout << "PosPos " << i << ',' << j << "\n";
			if (((i >= 3) && (i <= 12)) && ((j >= 1) && (j <= 10))){ //если игрок нажимает мышью на свое поле
				placementShipPlayers(i, j, ship);
			}else if (((i >= 3) && (i <= 12)) && ((j >= 12) && (j <= 21))) //если игрок нажимает мышью на поле компьютера
				seachShip(i, j);
		}
	}render(window, s_map);
}


int main()
{
	RenderWindow window(VideoMode(736, 544), "See Battle");
	int ship = 0;

	Image mapImage; //создаЄм объект Image(изображение)
	mapImage.loadFromFile("map2.psd");
	Texture map;//текстура карты
	map.loadFromImage(mapImage);//зар€жаем текстуру картинкой
	Sprite s_map;//создаЄм спрайт дл€ карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	/*Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пиксел€х);//сам объект текст (не строка)
	//text.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold);//жирный текст*/

	for (int Ship = 4; Ship > 0; Ship--) {
		std::cout << "NEW" << Ship << "\n";
		alignmentOfComputerOneShip(Ship);
	}
		

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		render(window, s_map);
		workWithMap(window, s_map, ship);
		text(window, s_map);

	}
	return 0;
}
