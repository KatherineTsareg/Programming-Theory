#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"
#include <iostream>
#include <ctime>

using namespace sf; //�������� ������������ ���� sf, ����� ��������� �� ������ sf::
using namespace std;

const int maxLengthShip = 4;
const float blockSize = 32.f;
const int windowHight = blockSize * heighMap;
const int windowWidth = blockSize * widthMap;
const char wall = 's';
const char empty = ' ';
const char oneShip = 'X';
const char dot = '.';
const char past = '1';//����
const char hurt = '2';//�����
const int si = 3;
const int sj1 = 1;
const int sj2 = 12;
const int stopTime = 600;

struct intelligence {
	bool isStep = true;
	bool win = false;
	bool isHit = false;
	bool step1 = true;
	bool step2 = false;
	bool step3 = false;
	bool step4 = false;
	bool step5 = false;
	bool hit = true;
	int dir = 0;
	int step;
	int numbCompShips = 20;
	int numbPLayerShips = 20;
	int numbStep = 0;
	int x = 0;
	int y = 0;
};
void render(RenderWindow & window, Sprite & mapSprite, Sprite & wallSprite, Sprite & button1, Sprite & blockSprite, Sprite & button2, Sprite & button3, Text & text, bool isHit, bool isStep, bool win)
{
	window.clear();
	wallSprite.setTextureRect(IntRect(0, 0, windowWidth, windowHight));//������ 
	wallSprite.setPosition(0, 0);//������ ��������� ���������� ��������� �������
	window.draw(wallSprite);//������ ���������� �� �����
	
	button1.setTextureRect(IntRect(0, 0, 130, 100));//������ 
	button1.setPosition(50, 430);//������ ��������� ���������� ��������� �������
	window.draw(button1);//������ ���������� �� �����

	button2.setTextureRect(IntRect(0, 100, 130, 100));//������ 
	button2.setPosition(300, 430);//������ ��������� ���������� ��������� �������
	window.draw(button2);//������ ���������� �� �����

	button3.setTextureRect(IntRect(0, 200, 130, 100));//������ 
	button3.setPosition(550, 430);//������ ��������� ���������� ��������� �������
	window.draw(button3);//������ ���������� �� �����

	blockSprite.setTextureRect(IntRect(0, 0, 672, 90));//������ 
	blockSprite.setPosition(32, 10);//������ ��������� ���������� ��������� �������
	window.draw(blockSprite);//������ ���������� �� �����
	
	if ((win == true) && (isStep == false)) {
		text.setString(L"�� ��������! ����� ������� ���? ");//������ ������ ������
		text.setPosition(100, 25);//������ ������� ������, ����� ������
		window.draw(text);//����� ���� �����
	}
	else if (win == true) {
		text.setString(L"�� ���������. ����� ������� ���?");//������ ������ ������
		text.setPosition(120, 25);//������ ������� ������, ����� ������
		window.draw(text);//����� ���� �����
	}
	if ((isHit == false) && (isStep == true) && (win == false)) {
		text.setString(L"��� ���!");//������ ������ ������
		text.setPosition(300, 25);//������ ������� ������, ����� ������
		window.draw(text);//����� ���� �����
	}
	else if ((isHit != false) && (isStep != true)) {
		text.setString(L"��� ����������!");//������ ������ ������
		text.setPosition(250, 25);//������ ������� ������, ����� ������
		window.draw(text);//����� ���� �����
	}

	for (int i = 0; i < heighMap; i++)
		for (int j = 0; j < widthMap; j++)
		{
			if (TileMap[i][j] == wall)
				mapSprite.setTextureRect(IntRect(160, 0, blockSize, blockSize)); //���� ��������� 's', �� ������ empty
			if ((TileMap[i][j] == oneShip) && (i < si + 10) && (i >= si) && (j < sj1 + 10) && (j >= sj1))
				mapSprite.setTextureRect(IntRect(blockSize, 0, blockSize, blockSize));
			if ((TileMap[i][j] == oneShip) && (i < si + 10) && (i >= si) && (j < sj2 + 10) && (j >= sj2))
				mapSprite.setTextureRect(IntRect(0, 0, blockSize, blockSize));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!00000
			if (TileMap[i][j] == dot)
				mapSprite.setTextureRect(IntRect(0, 0, blockSize, blockSize));
			if ((TileMap[i][j] == '1'))
				mapSprite.setTextureRect(IntRect(blockSize * 2, 0, blockSize, blockSize));//���� ��������� '1', �� ������ �������� "����� ����" (�����)
			if ((TileMap[i][j] == '2'))
				mapSprite.setTextureRect(IntRect(blockSize * 3, 0, blockSize, blockSize));// ���� ��������� '2', �� ������ �������� "����� �������" (�������)
			mapSprite.setPosition(j * blockSize, i * blockSize);
			window.draw(mapSprite);//������ ���������� �� �����
		};
	window.display();
}
int seachShipByCompStep1(int i, int j, Music & hit_music) {
	if (TileMap[i][j] == dot) { //�������, ������ ����
		sleep(milliseconds(stopTime));
		TileMap[i][j] = past;
		return 0;
	}else if (TileMap[i][j] == oneShip) {//�����, ������ �����
		sleep(milliseconds(stopTime));
		hit_music.play();
		TileMap[i][j] = hurt;
		return 1;
	}else if ((TileMap[i][j] == hurt)||(TileMap[i][j] == past))
		return 2;
}
int seachShipByCompStep2(int i, int j){
	int dir;//1-�����, 2-������, 3-����, 4-�����
	bool hit = true;
	bool left = true;
	bool right = true;
	bool up = true;
	bool down = true;
	if (TileMap[i - 1][j] == past)
		up = false; //������� ���
	if (TileMap[i][j + 1] == past)
		right = false;
	if (TileMap[i + 1][j] == past)
		down = false;
	if (TileMap[i][j - 1] == past)
		left = false;
	if (TileMap[i - 1][j] == past) //��������� ������ ������
		up = false;
	if (TileMap[i + 1][j] == wall)
		down = false;
	if (TileMap[i][j - 1] == wall)
		 left = false;
	if (TileMap[i][j + 1] == wall)
		right = false;
	if (((TileMap[i - 1][j] == hurt) || (TileMap[i - 1][j] == past)) && ((TileMap[i + 1][j] == hurt) || (TileMap[i + 1][j] == past)) && ((TileMap[i][j - 1] == hurt) || (TileMap[i][j - 1] == past)) && ((TileMap[i][j + 1] == hurt) || (TileMap[i][j + 1] == past)))
		hit = false;
	if ((up == false) && (down == false) && (left == false) && (right == false))//���� ������ ��� �����, �� ��� ������������ ������� � �� ����
		hit = false;
	else if ((up == true) && (down == true) && (left == true) && (right == true)) {//���� ������ ��� "�����", �������� ���� �����������
		dir = rand() % 4 + 1;
		if (dir == 1){ //up
			up = true;
			down = false;
			right = false;
			left = false;
		}
		else if (dir == 2) { //right
			up = false;
			down = false;
			right = true;
			left = false;
		}
		else if (dir == 3) { //down
			up = false;
			down = true;
			right = false;
			left = false;
		}
		else if (dir == 4) { //left
			up = false;
			down = false;
			right = false;
			left = true;
		}		
	}
	if (hit != false) {
		int* Arr;
		Arr = new int[4];
		int k = 0;
		if (up == true) {
			Arr[k] = 1;
			k++;
		}if (right == true) {
			Arr[k] = 2;
			k++;
		}if (down == true) {
			Arr[k] = 3;
			k++;
		}if (left == true) {
			Arr[k] = 4;
			k++;
		}
		if (k == 1)
			return Arr[0];
		else {
			dir = rand() % k;
			return Arr[dir];
		}
		delete[]Arr;
	}
	else
		return 0;
}
bool seachShipByCompStep3(int dir, int i, int j, Music & hit_music) {
	bool wounded = false;
	if (dir == 1)
		i--;
	if (dir == 2)
		j++;
	if (dir == 3)
		i++;
	if (dir == 4)
		j--;
	if (TileMap[i][j] == oneShip) {
		sleep(milliseconds(stopTime));
		hit_music.play();
		TileMap[i][j] = hurt;
		wounded = true;
	}
	else if ((TileMap[i][j] == wall) || (TileMap[i][j] == hurt) || (TileMap[i][j] == past))
		wounded = false;//error
	else {
		sleep(milliseconds(stopTime));
		TileMap[i][j] = past;
	}
	return wounded;
}
int seachShipByCompStep4(int dir, int i, int j, Music & hit_music){
	int wounded = 0;
	if (dir == 1)
		i--;
	if (dir == 2)
		j++;
	if (dir == 3)
		i++;
	if (dir == 4)
		j--;
	if (TileMap[i][j] == oneShip) {
		sleep(milliseconds(stopTime));
		hit_music.play();
		TileMap[i][j] = hurt;
		wounded = 1;
	}else if (TileMap[i][j] == hurt) {
		TileMap[i][j] = hurt;
		wounded = 1;
	}else if (TileMap[i][j] == wall)
		wounded = 2;//error
	else if (TileMap[i][j] == past)
		wounded = 0;
	else {
		sleep(milliseconds(stopTime));
		TileMap[i][j] = past;
		wounded = 0;
	}
	return wounded;
}
bool seachShipByCompStep5(int dir, int i, int j, Music & hit_music) {
	if (dir == 1) {
		while (TileMap[i][j] == hurt)
			i++;
		if ((TileMap[i][j] == past) || (TileMap[i][j] == dot)) {
			sleep(milliseconds(stopTime));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == oneShip) || (TileMap[i][j] == hurt)) {
			sleep(milliseconds(stopTime));
			hit_music.play();
			TileMap[i][j] = hurt;
			i++;
			return true;
		}
		else if (TileMap[i][j] == wall)
			return false;
	}
	if (dir == 2) {
		while (TileMap[i][j] == hurt)
			j--;
		if ((TileMap[i][j] == past) || (TileMap[i][j] == dot)) {
			sleep(milliseconds(stopTime));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == oneShip) || (TileMap[i][j] == hurt)) {
			sleep(milliseconds(stopTime));
			hit_music.play();
			TileMap[i][j] = hurt;
			j--;
			return true;
		}
		else if (TileMap[i][j] == wall)
			return false;
	}
	if (dir == 3) {
		while (TileMap[i][j] == hurt)
			i--;
		if ((TileMap[i][j] == past) || (TileMap[i][j] == dot)) {
			sleep(milliseconds(stopTime));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == oneShip) || (TileMap[i][j] == '3')) {
			sleep(milliseconds(stopTime));
			hit_music.play();
			TileMap[i][j] = hurt;
			i--;
			return true;
		}
		else if (TileMap[i][j] == wall)
			return false;
	}
	if (dir == 4) {
		while (TileMap[i][j] == hurt)
			j++;
		if ((TileMap[i][j] == past) || (TileMap[i][j] == dot)) {
			sleep(milliseconds(stopTime));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == oneShip) || (TileMap[i][j] == hurt)) {
			sleep(milliseconds(stopTime));
			hit_music.play();
			TileMap[i][j] = hurt;
			j++;
			return true;
		}
		else if (TileMap[i][j] == wall)
			return false;
	}
}
void alignmentOfComputerOneShip(int si, int sj)//����������� �������� �����������
{	
	for (int i = 0; i < heighMap; i++)
		for (int j = 0; j < widthMap; j++)
			if ((TileMap[i][j] != ' ') && (i >= si) && (i < si + 10) && (j >= sj) && (j < sj + 10))
				TileMap[i][j] = ' ';
	for (int LengthShip = 4; LengthShip > 0; LengthShip--) {
		int k = 1; //���������� ���������� � ��� �����
		int i;
		int j;
		int location;
		bool loose;
		int count;
		while (k <= (5 - LengthShip)) {
			i = rand() % 10 + si;
			j = rand() % 10 + sj;
			location = rand() % 2 + 1; //������������ �������, 1 - �������������, 2 - �����������
			if ((i < si + 10) && (j < sj + 10) && (TileMap[i][j] == ' '))
				if (location == 1) {
					loose = true;
					if ((j + LengthShip) < sj + 10) {
						if (TileMap[i][j - 1] == oneShip)
							loose = false;
						for (count = 0; count <= LengthShip; count++)//���������, �������� �� ������ �����
							if ((TileMap[i][j + count] != ' ') || (TileMap[i - 1][j + count] == oneShip) || (TileMap[i + 1][j + count] == oneShip))
								loose = false;
						if ((TileMap[i][j + LengthShip] == oneShip) || (TileMap[i - 1][j + LengthShip] == oneShip) || (TileMap[i + 1][j + LengthShip] == oneShip))//��������� ��������� ��������� �� ����������� ��������� ��������
							loose = false;
						if (loose == true) { //���� ����� ���������� �������, �� ���������
							if (TileMap[i][j - 1] != wall)
								TileMap[i][j - 1] = dot;
							if (TileMap[i - 1][j - 1] != wall)
								TileMap[i - 1][j - 1] = dot;
							if (TileMap[i + 1][j - 1] != wall)
								TileMap[i + 1][j - 1] = dot;
							if (TileMap[i][j + LengthShip] != wall)
								TileMap[i][j + LengthShip] = dot;
							if (TileMap[i - 1][j + LengthShip] != wall)
								TileMap[i - 1][j + LengthShip] = dot;
							if (TileMap[i + 1][j + LengthShip] != wall)
								TileMap[i + 1][j + LengthShip] = dot;
							for (count = 0; count < LengthShip; count++) {
								TileMap[i][j + count] = oneShip;
								if (TileMap[i - 1][j + count] != wall)
									TileMap[i - 1][j + count] = dot;
								if (TileMap[i + 1][j + count] != wall)
									TileMap[i + 1][j + count] = dot;
							}
							k++;
						}
					}
					else if (location == 2) { //�����������
						loose = true;
						if ((i + LengthShip) < si + 10)
							if (TileMap[i - 1][j] == oneShip)//��������� ���������� ������
								loose = false;
						for (count = 1; count < LengthShip; count++)//���������, �������� �� ������ �����
							if ((TileMap[i + count][j] != ' ') || (TileMap[i + count][j + 1] == oneShip) || (TileMap[i + count][j - 1] == oneShip))
								loose = false;
						if ((TileMap[i + LengthShip][j] == oneShip) || (TileMap[i + LengthShip][j - 1] == oneShip) || (TileMap[i + LengthShip][j + 1] == oneShip))//��������� ��������� ��������� �� ����������� ��������� ��������
							loose = false;
						if (loose == true) {
							if (TileMap[i - 1][j] != wall)
								TileMap[i - 1][j] = dot;
							if (TileMap[i - 1][j - 1] != wall)
								TileMap[i - 1][j - 1] = dot;
							if (TileMap[i - 1][j + 1] != wall)
								TileMap[i - 1][j + 1] = dot;
							if (TileMap[i + LengthShip][j] != wall)
								TileMap[i + LengthShip][j] = dot;
							if (TileMap[i + LengthShip][j - 1] != wall)
								TileMap[i + LengthShip][j - 1] = dot;
							if (TileMap[i + LengthShip][j + 1] != wall)
								TileMap[i + LengthShip][j + 1] = dot;
							for (count = 0; count < LengthShip; count++) {
								TileMap[i + count][j] = oneShip;
								if (TileMap[i + count][j + 1] != wall)
									TileMap[i + count][j + 1] = dot;
								if (TileMap[i + count][j - 1] != wall)
									TileMap[i + count][j - 1] = dot;
							}
							k++;
						}
					}
				}
		}
	}
	for (int i = 0; i < heighMap; i++)
		for (int j = 0; j < widthMap; j++)
			if ((TileMap[i][j] == ' ') && (i >= si) && (i < si + 10) && (j >= sj) && (j < sj + 10))
				TileMap[i][j] = dot;
}
void ComputerMove(struct intelligence *intel, Music & hit_music) {
	(*intel).isHit = true;
	if ((*intel).step1 == true) {
		(*intel).x = rand() % 10 + si;
		(*intel).y = rand() % 10 + sj1;
		(*intel).step = seachShipByCompStep1((*intel).x, (*intel).y, hit_music);
		if ((*intel).step == 0) {//�� ������ ���� ������ ����
			(*intel).isHit = false;
			(*intel).step2 = false;
			(*intel).step1 = true;
		}
		else if ((*intel).step == 1) {
			(*intel).isHit = true;
			(*intel).step2 = true;
			(*intel).step1 = false;
			(*intel).numbPLayerShips--;
		}
		else if ((*intel).step == 2) {
			(*intel).step1 = true;
			(*intel).isHit = true;
			(*intel).step2 = false;
		}
	}
	///////////////////////��� 2//////////////////
	if ((*intel).step2 == true) {//����� ����������� ��� ����� �� ��������� ������, ������ ����� � ������ ��� ������ �� ������
		(*intel).dir = seachShipByCompStep2((*intel).x, (*intel).y);
		if ((*intel).dir != 0) {
			(*intel).step2 = false;
			(*intel).step3 = true;
			(*intel).isHit = true;
		}
		else {
			(*intel).step1 = true;
			(*intel).step2 = false;
			(*intel).isHit = true;
		}
	}
	///////////////////////��� 3/////////////////
	if ((*intel).step3 == true) {//��������� �����������
		if (seachShipByCompStep3((*intel).dir, (*intel).x, (*intel).y, hit_music) == true) {
			(*intel).step4 = true;
			(*intel).step3 = false;
			(*intel).isHit = true;
		}
		else {
			(*intel).step2 = true;
			(*intel).step3 = false;
			(*intel).isHit = false;
		}
	}
	///////////////////////��� 4//////////////////
	if ((*intel).step4 == true) {//�������� ������� �� ��������� �����������
		if (seachShipByCompStep4((*intel).dir, (*intel).x, (*intel).y, hit_music) == 0) {
			(*intel).isHit = false;
			(*intel).step4 = false;
			(*intel).step5 = true;//������ � ������ ������� ��������� �� ������ �������� �������
		}
		else if (seachShipByCompStep4((*intel).dir, (*intel).x, (*intel).y, hit_music) == 1) {
			(*intel).isHit = true;
			(*intel).numbPLayerShips--;
			if ((*intel).dir == 1)
				(*intel).x--;
			if ((*intel).dir == 2)
				(*intel).y++;
			if ((*intel).dir == 3)
				(*intel).x++;
			if ((*intel).dir == 4)
				(*intel).y--;
		}
		else if (seachShipByCompStep4((*intel).dir, (*intel).x, (*intel).y, hit_music) == 2) {//��������� ������ ��� ��� �������� �� ��� ����������
			(*intel).isHit = true;
			(*intel).step4 = false;
			(*intel).step5 = true;//������ � ������ ������� ��������� �� ������ �������� �������
		}
	}
	///////////////////////��� 5//////////////////
	if (((*intel).step5 == true) && ((*intel).isHit = true)) {
		if (seachShipByCompStep5((*intel).dir, (*intel).x, (*intel).y, hit_music) == false) {
			(*intel).step5 = false;
			(*intel).step1 = true;
			(*intel).isHit = false;
		}
		else {
			(*intel).isHit = true;
			(*intel).step5 = true;
			(*intel).numbPLayerShips--;
		}
	}
}
void PlayerMove(int i, int j, struct intelligence *intel, Music & hit_music) {
	if (TileMap[i][j] == dot) { //����
		TileMap[i][j] = past; //������ �����
		//water_music.play();
		(*intel).hit = false;
		//intel.numbStep++;
	}
	else if (TileMap[i][j] == oneShip) {//�����
		TileMap[i][j] = hurt;//������ �������
		hit_music.play();
		(*intel).numbCompShips--;
		//intel.numbStep++;
		(*intel).hit = true;
	}
	(*intel).isStep = false;
}
int main()
{
	RenderWindow window(VideoMode(736, 544), "Sea War!");	
	intelligence intel;
	
	Image mapImage; //������ ������ Image(�����������)
	mapImage.loadFromFile("images/map.psd");
	Texture map;//�������� �����
	map.loadFromImage(mapImage);//�������� �������� ���������
	Sprite mapSprite;//������ ������ ��� �����
	mapSprite.setTexture(map);//�������� �������� ��������

	Image wallImage; //������ ������ Image(�����������)
	wallImage.loadFromFile("images/wall.png");
	Texture wall;//�������� �����
	wall.loadFromImage(wallImage);//�������� �������� ���������
	Sprite wallSprite;//������ ������ ��� ����� 
	wallSprite.setTexture(wall);//�������� �������� ��������

	Image buttonImage; //������ ������ Image(�����������)
	buttonImage.loadFromFile("images/buttons.psd");
	Texture button;//�������� �����
	button.loadFromImage(buttonImage);//�������� �������� ���������
	Sprite button1;//������ ������ ��� ����� 
	button1.setTexture(button);//�������� �������� ��������
	Sprite button2;//������ ������ ��� ����� 
	button2.setTexture(button);//�������� �������� ��������
	Sprite button3;//������ ������ ��� ����� 
	button3.setTexture(button);//�������� �������� ��������

	Image blockImage; //������ ������ Image(�����������)
	blockImage.loadFromFile("images/block.psd");
	Texture block;//�������� �����
	block.loadFromImage(blockImage);//�������� �������� ���������
	Sprite blockSprite;//������ ������ ��� ����� 
	blockSprite.setTexture(block);//�������� �������� ��������

	Music hit_music;
	SoundBuffer hit_buffer;
	Sound hit_sound;
	hit_sound.setBuffer(hit_buffer);
	if (!hit_music.openFromFile("audio/hit.wav"))
		return -1; // error

	/*Music water_music;
	SoundBuffer water_buffer;
	Sound water_sound;
	water_sound.setBuffer(water_buffer);
	if (!water_music.openFromFile("audio/water.wav"))
		return -1; // error*/

	Font font;//����� 
	font.loadFromFile("font.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 30);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setColor(Color::Black);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	
	/////////////////////����������� �������///////////////////
	srand(time(NULL));

	alignmentOfComputerOneShip(si, sj1);
	alignmentOfComputerOneShip(si, sj2);
	//////////////////////////�����///////////////////////////////
	while (window.isOpen())
	{
		sf::Vector2i pixPos = sf::Mouse::getPosition(window);
		int i = (pixPos.y / 32);
		int j = (pixPos.x / 32);

		render(window, mapSprite, wallSprite, button1, blockSprite, button2, button3, text, intel.isHit, intel.isStep, intel.win);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					if (button3.getGlobalBounds().contains(pixPos.x, pixPos.y))
						window.close();
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					if (button2.getGlobalBounds().contains(pixPos.x, pixPos.y)) {
						alignmentOfComputerOneShip(si, sj1);

					}
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					if (button1.getGlobalBounds().contains(pixPos.x, pixPos.y)) {
						alignmentOfComputerOneShip(si, sj1);
						alignmentOfComputerOneShip(si, sj2);
						intel.isStep = true;
						intel.hit = true;
						intel.isHit = false;
						intel.step1 = true;
						intel.step2 = false;
						intel.step3 = false;
						intel.step4 = false;
						intel.step5 = false;
						intel.win = false;
						intel.dir = 0;
						intel.numbCompShips = 20;
						intel.numbPLayerShips = 20;
						intel.numbStep = 0;
						intel.x = 0;
						intel.y = 0;
					}
			i = (pixPos.y / 32);
			j = (pixPos.x / 32);
			if ((intel.numbCompShips > 0) && (intel.numbPLayerShips > 0)) {
				///////////////////////��� ������///////////////////////////
				//cout << "MOVE THE PLAYER! isStep = "<< intel.isStep << ", isHit = " << intel.isHit << "\n";
				if ((intel.isStep == true) && (intel.isHit == false)) {
					if (event.type == Event::MouseButtonPressed)
						if (event.key.code == Mouse::Left)
							if ((i <= 12) && (i >= 3) && (j <= 21) && (j >= 12)) //� ��� ���� ���������� ������� ��������
								PlayerMove(i, j, &intel, hit_music);
				}else
					intel.isStep = false;
				render(window, mapSprite, wallSprite, button1, blockSprite, button2, button3, text, intel.isHit, intel.isStep, intel.win);
				////////////////////////��� ����////////////////////////////////
				if ((intel.isStep == false) && (intel.hit == false)) {
					ComputerMove(&intel, hit_music);
					intel.isStep = true;
				}
				else
					intel.isStep = true;
			}
			else if ((intel.numbCompShips <= 0) || (intel.numbPLayerShips <= 0)) {
				intel.isHit = false;
				intel.isStep = false;
				intel.step1 = false;
				intel.step2 = false;
				intel.step3 = false;
				intel.step4 = false;
				intel.step5 = true;
				intel.win = true;
			}
			if (intel.numbPLayerShips <= 0)
				intel.isStep = true;
		}
	}
	return 0;
}