#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include <ctime>


using namespace sf; //�������� ������������ ���� sf, ����� ��������� �� ������ sf::
using namespace std;


const int maxLengthShip = 4;

const float BLOCK_SIZE = 32.f;
const int WindowHight = BLOCK_SIZE * HEIGHT_MAP;
const int WindowWidth = BLOCK_SIZE * WIDTH_MAP;


void render(RenderWindow & window, Sprite & s_map, Sprite & wallSprite)
{
	window.clear();

	wallSprite.setTextureRect(IntRect(0, 0, 736, 544));//������ 
	wallSprite.setPosition(0, 0);//������ ��������� ���������� ��������� �������
	window.draw(wallSprite);//������ ���������� �� �����

	/////////////////////////////������ �����/////////////////////
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP;j++)
		{
			if (TileMap[i][j] == 's')
				s_map.setTextureRect(IntRect(160, 0, 32, 32)); //���� ��������� 's', �� ������ empty
			if ((TileMap[i][j] == ' '))
				s_map.setTextureRect(IntRect(0, 0, 32, 32));//���� ��������� ' ', �� ������ �������� ��������
			if ((TileMap[i][j] == '1'))
				s_map.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� '1', �� ������ �������� "����� ����" (�����)
			if ((TileMap[i][j] == '2'))
				s_map.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� '2', �� ������ �������� "����� �������" (�������)
			if ((TileMap[i][j] == '3'))
				s_map.setTextureRect(IntRect(96, 0, 32, 32));//���� ��������� '3', �� ������ �������� "�����" (�������)
			if ((TileMap[i][j] == '4'))
				s_map.setTextureRect(IntRect(96, 0, 32, 32));//���� ��������� '4', �� ������ �������� "����" (������� �������)
			
			if ((TileMap[i][j] == '0'))
				s_map.setTextureRect(IntRect(0, 0, 32, 32));//���� ��������� '0', �� ������ �������� "����� �����" (�����)
			if ((TileMap[i][j] == 'X'))
				s_map.setTextureRect(IntRect(0, 0, 32, 32));//���� ��������� '�', �� ������ �������� "����� �������" (�������)

			s_map.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);//����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

			window.draw(s_map);//������ ���������� �� �����
		};
	

	window.display();
}

/*void text(RenderWindow & window, Sprite & s_map)
{
	/*std::ostringstream fourDeckString;    // �������� ����������
	fourDeckString << p.ostringstream;		//������� � ��� ����� �����, �� ���� ��������� ������
	text.setString(":" + fourDeckString.str());//������ ������ ������ � �������� �������������� ���� ������ ������� .str()
	text.setPosition(0, 0);//������ ������� ������, ����� ������
	window.draw(text);//����� ���� �����
}*/

bool seachShipByCompStep1(int i, int j, int numbPlayerShips) {
	bool hit = true;
	if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0')) { //�������, ������ ����
		TileMap[i][j] = '1';
		hit = false;
	}
	else if (TileMap[i][j] == '2') {//�����, ������ �����
		TileMap[i][j] = '3';
		numbPlayerShips--;
		hit = true;
	}
	else if ((TileMap[i][j] == '3')||(TileMap[i][j] == '1'))
		hit = true;
	return hit;

}

bool seachShipByCompStep2(){}
bool seachShipByCompStep3(){}

void genNewPos(int i, int j) {
	int loc;
	if (((i < 3) && (i > 12)) || ((j < 1) && (j > 10))) {
		i = rand() % 10 + 3;
		j = rand() % 10 + 1;
	}
	if (TileMap[i][j] == '1') {//��� ������� � ��� ������
		i = rand() % 10 + 3;
		j = rand() % 10 + 1;
	}
	else if (TileMap[i][j] == '3') {//��� �������, �����
		loc = rand() % 4 + 1;
		if (loc == 1) //������ �����
			j--;
		else if (loc == 2)
			i--;
		else if (loc == 3)
			j++;
		else if (loc == 4)
			i++;
	}
}

void alignmentOfComputerOneShip(int LengthShip)//����������� �������� �����������
{
	int k = 1; //���������� ���������� � ��� �����
	int i;
	int j;
	int location;
	bool loose;
	int count;
	while (k <= (5 - LengthShip)) {
		i = rand() % 10 + 3;
		j = rand() % 10 + 12;
		location = rand() % 2 + 1; //������������ �������, 1 - �������������, 2 - �����������
		//std::cout << "i=" << i << ",j=" << j << ",LEN= " << LengthShip << ",loc= " << location << ", k= " << k << "\n";
		if ((i <= 12) && (j <= 21) && (TileMap[i][j] == ' ')) {
			if (location == 1) {
				loose = true;
				if ((j + LengthShip) <= 21){
					if (TileMap[i][j - 1] == 'X')
						loose = false;
					for (count = 0; count <= LengthShip; count++)//���������, �������� �� ������ �����
						if ((TileMap[i][j + count] != ' ')||(TileMap[i - 1][j + count] == 'X')||(TileMap[i + 1][j + count] == 'X'))
							loose = false;
					if ((TileMap[i][j + LengthShip] == 'X')||(TileMap[i - 1][j + LengthShip] == 'X')||(TileMap[i + 1][j + LengthShip] == 'X'))//��������� ��������� ��������� �� ����������� ��������� ��������
						loose = false;
					if (loose == true) { //���� ����� ���������� �������, �� ���������
						if (TileMap[i][j - 1] != 's')
							TileMap[i][j - 1] = '0';
						if (TileMap[i - 1][j - 1] != 's')
							TileMap[i - 1][j - 1] = '0';
						if (TileMap[i + 1][j - 1] != 's')
							TileMap[i + 1][j - 1] = '0';
						if (TileMap[i][j + LengthShip] != 's')
							TileMap[i][j + LengthShip] = '0';
						if (TileMap[i - 1][j + LengthShip] != 's')
							TileMap[i - 1][j + LengthShip] = '0';
						if (TileMap[i + 1][j + LengthShip] != 's')
							TileMap[i + 1][j + LengthShip] = '0';
						for (count = 0; count < LengthShip; count++) {
							TileMap[i][j + count] = 'X';
							if (TileMap[i - 1][j + count] != 's')
								TileMap[i - 1][j + count] = '0';
							if (TileMap[i + 1][j + count] != 's')
								TileMap[i + 1][j + count] = '0';
						}
						//std::cout << "set01: " << i << " , " << j << "\n";
						k++;
					}
				}
			}else if (location == 2) { //�����������
				loose = true;
				if ((i + LengthShip) <= 12) {
					if (TileMap[i - 1][j] == 'X')//��������� ���������� ������
						loose = false;
					for (count = 1; count < LengthShip; count++)//���������, �������� �� ������ �����
						if ((TileMap[i + count][j] != ' ') || (TileMap[i + count][j + 1] == 'X') || (TileMap[i + count][j -1] == 'X'))
							loose = false;
					if ((TileMap[i + LengthShip][j] == 'X') || (TileMap[i + LengthShip][j - 1] == 'X') || (TileMap[i + LengthShip][j + 1] == 'X'))//��������� ��������� ��������� �� ����������� ��������� ��������
						loose = false;
					if (loose == true) {
						if (TileMap[i - 1][j] != 's')
							TileMap[i - 1][j] = '0';
						if (TileMap[i - 1][j - 1] != 's')
							TileMap[i - 1][j - 1] = '0';
						if (TileMap[i - 1][j + 1] != 's')
							TileMap[i - 1][j + 1] = '0';
						if (TileMap[i + LengthShip][j] != 's')
							TileMap[i + LengthShip][j] = '0';
						if (TileMap[i + LengthShip][j - 1] != 's')
							TileMap[i + LengthShip][j - 1] = '0';
						if (TileMap[i + LengthShip][j + 1] != 's')
							TileMap[i + LengthShip][j + 1] = '0';
						for (count = 0; count < LengthShip; count++) {
							TileMap[i + count][j] = 'X';
							if (TileMap[i + count][j + 1] != 's')
								TileMap[i + count][j + 1] = '0';
							if (TileMap[i + count][j - 1] != 's')
								TileMap[i + count][j - 1] = '0';  
						}
						//std::cout << "set02: " << i << " , " << j << "\n";
						k++;
					}
				}
			}
		}
	}
}
void randomAligmentShipsForPlayer(int LengthShip) { //�������� ����������� ������� ��� ������
	int k = 1; //���������� ���������� � ��� �����
	int i;
	int j;
	int location;
	bool loose;
	int count;
	while (k <= (5 - LengthShip)) {
		i = rand() % 10 + 3;
		j = rand() % 10 + 1;
		location = rand() % 2 + 1; //������������ �������, 1 - �������������, 2 - �����������
		//std::cout << "i=" << i << ",j=" << j << ",LEN= " << LengthShip << ",loc= " << location << ", k= " << k << "\n";
		if ((i <= 12) && (j <= 10) && (TileMap[i][j] == ' ')) {
			if (location == 1) {
				loose = true;
				if ((j + LengthShip) <= 10) {
					if (TileMap[i][j - 1] == '2')
						loose = false;
					for (count = 0; count <= LengthShip; count++)//���������, �������� �� ������ �����
						if ((TileMap[i][j + count] != ' ') || (TileMap[i - 1][j + count] == '2') || (TileMap[i + 1][j + count] == '2'))
							loose = false;
					if ((TileMap[i][j + LengthShip] == '2') || (TileMap[i - 1][j + LengthShip] == '2') || (TileMap[i + 1][j + LengthShip] == '2'))//��������� ��������� ��������� �� ����������� ��������� ��������
						loose = false;
					if (loose == true) { //���� ����� ���������� �������, �� ���������
						if (TileMap[i][j - 1] != 's')
							TileMap[i][j - 1] = '0';
						if (TileMap[i - 1][j - 1] != 's')
							TileMap[i - 1][j - 1] = '0';
						if (TileMap[i + 1][j - 1] != 's')
							TileMap[i + 1][j - 1] = '0';
						if (TileMap[i][j + LengthShip] != 's')
							TileMap[i][j + LengthShip] = '0';
						if (TileMap[i - 1][j + LengthShip] != 's')
							TileMap[i - 1][j + LengthShip] = '0';
						if (TileMap[i + 1][j + LengthShip] != 's')
							TileMap[i + 1][j + LengthShip] = '0';
						for (count = 0; count < LengthShip; count++) {
							TileMap[i][j + count] = '2';
							if (TileMap[i - 1][j + count] != 's')
								TileMap[i - 1][j + count] = '0';
							if (TileMap[i + 1][j + count] != 's')
								TileMap[i + 1][j + count] = '0';
						}
						//std::cout << "set01: " << i << " , " << j << "\n";
						k++;
					}
				}
			}
			else if (location == 2) { //�����������
				loose = true;
				if ((i + LengthShip) <= 12) {
					if (TileMap[i - 1][j] == '2')//��������� ���������� ������
						loose = false;
					for (count = 1; count < LengthShip; count++)//���������, �������� �� ������ �����
						if ((TileMap[i + count][j] != ' ') || (TileMap[i + count][j + 1] == '2') || (TileMap[i + count][j - 1] == '2'))
							loose = false;
					if ((TileMap[i + LengthShip][j] == '2') || (TileMap[i + LengthShip][j - 1] == '2') || (TileMap[i + LengthShip][j + 1] == '2'))//��������� ��������� ��������� �� ����������� ��������� ��������
						loose = false;
					if (loose == true) {
						if (TileMap[i - 1][j] != 's')
							TileMap[i - 1][j] = '0';
						if (TileMap[i - 1][j - 1] != 's')
							TileMap[i - 1][j - 1] = '0';
						if (TileMap[i - 1][j + 1] != 's')
							TileMap[i - 1][j + 1] = '0';
						if (TileMap[i + LengthShip][j] != 's')
							TileMap[i + LengthShip][j] = '0';
						if (TileMap[i + LengthShip][j - 1] != 's')
							TileMap[i + LengthShip][j - 1] = '0';
						if (TileMap[i + LengthShip][j + 1] != 's')
							TileMap[i + LengthShip][j + 1] = '0';
						for (count = 0; count < LengthShip; count++) {
							TileMap[i + count][j] = '2';
							if (TileMap[i + count][j + 1] != 's')
								TileMap[i + count][j + 1] = '0';
							if (TileMap[i + count][j - 1] != 's')
								TileMap[i + count][j - 1] = '0';
						}
						//std::cout << "set02: " << i << " , " << j << "\n";
						k++;
					}
				}
			}
		}
	}
}

int main()
{
	RenderWindow window(VideoMode(736, 544), "See War!");

	bool isStep = true;
	bool hit = true;
	bool isHit1 = false;
	bool isHit2 = false;
	bool isHit3 = false;
	int numbCompShips = 20;
	int numbPlayerShips = 20;
	

	Image mapImage; //������ ������ Image(�����������)
	mapImage.loadFromFile("images/map.psd");
	Texture map;//�������� �����
	map.loadFromImage(mapImage);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������

	Image wallImage; //������ ������ Image(�����������)
	wallImage.loadFromFile("images/wall.png");
	Texture wall;//�������� �����
	wall.loadFromImage(wallImage);//�������� �������� ���������
	Sprite wallSprite;//������ ������ ��� ����� 
	wallSprite.setTexture(wall);//�������� �������� ��������

	/////////////////////����������� �������///////////////////
	srand(time(NULL));
	for (int Ship = 4; Ship > 0; Ship--) { //��������� ����������� �������
		//std::cout << "NEW" << Ship << "\n";
		alignmentOfComputerOneShip(Ship);
	}
	for (int Ship = 4; Ship > 0; Ship--) { //��������� ����������� �������� ��� ������
		randomAligmentShipsForPlayer(Ship);
	}
	//////////////////////////�����///////////////////////////////

	
	while (window.isOpen())
	{
		
		sf::Vector2i pixPos = sf::Mouse::getPosition(window);
		int i = (pixPos.y / 32);
		int j = (pixPos.x / 32);
		//std::cout << "PosPos " << i << ',' << j << "\n";

		int x, y;
		

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			//////////////////////////////////////////////////////////////////////



			///////////////////////��� ������///////////////////////////

			if ((isStep == true) && (isHit == false)){//&&(i <= 12)&&(i >= 3)&&(j <= 21 )&&(j >= 12)) {
				//std::cout << "Your turn!\n";
				if (event.type == Event::MouseButtonPressed)
					if (event.key.code == Mouse::Left)
						if ((pixPos.x >= 0) && (pixPos.y >= 0) && (pixPos.x <= WindowWidth) && (pixPos.y <= WindowHight)) //� ��� ���� ���������� ������� ��������
						{
							if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0')) { //����
								TileMap[i][j] = '1'; //������ �����
								hit = false;
							}
							else if (TileMap[i][j] == 'X') {//�����
								TileMap[i][j] = '3';//������ �������
								hit = true;
								numbPlayerShips--;
							}
							isStep = false;
						}
			}
			else
				isStep = false;
			////////////////////////��� ����////////////////////////////////
			if ((isStep == false) && (hit == false)) { //��� ����������
				std::cout << "Turn of the enemy!\n";
				x = rand() % 10 + 3;
				y = rand() % 10 + 1;
				isHit1 = seachShipByCompStep1(x, y, numbPlayerShips);
				isStep = true;
				if (isHit == false) {
					std::cout << "Your turn!\n";
					std::cout << "number ships which you should kill = "<< numbPlayerShips<<"\n";
					//std::cout << "number ships witch conputer should kill = " << numbCompShips << "\n";
				}else
					numbCompShips--;
			}
			else
				isStep = true;
			render(window, s_map, wallSprite);
			//text(window, s_map);
		}
	}
	return 0;
}

