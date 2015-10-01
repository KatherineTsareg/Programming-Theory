#include <SFML/Graphics.hpp>
#include "Map.h"
#include <sstream>
#include <iostream>


using namespace sf; //�������� ������������ ���� sf, ����� ��������� �� ������ sf::
using namespace std;

//const int fourDeck = 1;
//const int threeDeck = 2;
//const int doubleDeck = 3;
const int maxLengthShip = 4;
const int maxNumberSingleShip = 20;

const float BLOCK_SIZE = 32.f;
const int WindowHight = BLOCK_SIZE * HEIGHT_MAP;
const int WindowWidth = BLOCK_SIZE * WIDTH_MAP;


void render(RenderWindow & window, Sprite & s_map)
{
	window.clear();

	/////////////////////////////������ �����/////////////////////
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP;j++)
		{
			if (TileMap[i][j] == 's')
				s_map.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� 's', �� ������ �������� ��������
			if (TileMap[i][j] == 'm')
				s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� 'm', �� ������ �������� ������
			if ((TileMap[i][j] == '0'))
				s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� '0', �� ������ �������� ��������
			if (TileMap[i][j] == 'd')
				s_map.setTextureRect(IntRect(96, 0, 128, 32));//���� ��������� 'd', �� ������ �������� �������(�������)
			if ((TileMap[i][j] == '1'))
				s_map.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� '1', �� ������ �������� "����" (�����)
			if ((TileMap[i][j] == '2'))
				s_map.setTextureRect(IntRect(96, 0, 32, 32));//���� ��������� '2', �� ������ �������� "�����" (�������)
			if ((TileMap[i][j] == '3'))
				s_map.setTextureRect(IntRect(128, 0, 32, 32));//���� ��������� '3', �� ������ �������� "����" (������� �������)
			
			s_map.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);//����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

			window.draw(s_map);//������ ���������� �� �����
		};
	window.display();
}

void text(RenderWindow & window, Sprite & s_map)
{
	/*std::ostringstream fourDeckString;    // �������� ����������
	fourDeckString << p.ostringstream;		//������� � ��� ����� �����, �� ���� ��������� ������
	text.setString(":" + fourDeckString.str());//������ ������ ������ � �������� �������������� ���� ������ ������� .str()
	text.setPosition(0, 0);//������ ������� ������, ����� ������
	window.draw(text);//����� ���� �����*/
}

void placementShipPlayers(int i, int j, int ship) { //����� ����������� ���� �������
	if ((TileMap[i][j] == '0') && (TileMap[i - 1][j - 1] != '2') && (TileMap[i - 1][j + 1] != '2') && (TileMap[i + 1][j - 1] != '2') && (TileMap[i + 1][j + 1] != '2'))
	{
		ship = ship + 3;
		TileMap[i][j] = '2';
		std::cout << "SHIP=" << ship<< "\n";
	}
}

void seachShip(int i, int j) {
	if (TileMap[i][j] == '0')
		TileMap[i][j] = '1';
	else if (TileMap[i][j] == 'd')
		TileMap[i][j] = '2';
}

	void workWithMap(RenderWindow & window, Sprite & s_map, int ship){
	if (sf::Mouse::isButtonPressed(Mouse::Left)){
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		if (((localPosition.x >= 0) && (localPosition.y >= 0)) && ((localPosition.x <= WindowWidth) && (localPosition.y <= WindowHight))) {
			int i = (localPosition.y / 32);
			int j = (localPosition.x / 32);
			std::cout << "PosPos " << i << ',' << j << "\n";
			if (((i >= 3) && (i <= 12)) && ((j >= 1) && (j <= 10))){ //���� ����� �������� ����� �� ���� ����
				ship++;
				placementShipPlayers(i, j, ship);
				ship++;
			}else if (((i >= 3) && (i <= 12)) && ((j >= 12) && (j <= 21))) //���� ����� �������� ����� �� ���� ����������
				seachShip(i, j);
		}
	}render(window, s_map);
}


int main()
{
	RenderWindow window(VideoMode(736, 544), "See Battle");
	int ship = 0;

	Image mapImage; //������ ������ Image(�����������)
	mapImage.loadFromFile("map2.psd");
	Texture map;//�������� �����
	map.loadFromImage(mapImage);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������

	/*Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	//text.setColor(Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	text.setStyle(sf::Text::Bold);//������ �����*/

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
