#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"
#include <iostream>
#include <ctime>

using namespace sf; //включаем пространство имен sf, чтобы постоянно не писать sf::
using namespace std;

const int maxLengthShip = 4;
const float BLOCK_SIZE = 32.f;
const int WindowHight = BLOCK_SIZE * HEIGHT_MAP;
const int WindowWidth = BLOCK_SIZE * WIDTH_MAP;
const char wall = 's';
const char empty = ' ';
const char one_ship = 'X';
const char dot = '.';
const char past = '1';//мимо
const char hurt = '2';//ранил
const int si = 3;
const int sj1 = 1;
const int sj2 = 12;
const int stop_time = 600;

void render(RenderWindow & window, Sprite & s_map, Sprite & wallSprite, Sprite & s_button1, Sprite & s_block, Sprite & s_button2, Sprite & s_button3, Text & text, bool isHit, bool isStep, bool win)
{
	window.clear();
	wallSprite.setTextureRect(IntRect(0, 0, 736, 544));//рисуем 
	wallSprite.setPosition(0, 0);//задаем начальные координаты появления спрайта
	window.draw(wallSprite);//рисуем квадратики на экран
	
	s_button1.setTextureRect(IntRect(0, 0, 130, 100));//рисуем 
	s_button1.setPosition(50, 430);//задаем начальные координаты появления спрайта
	window.draw(s_button1);//рисуем квадратики на экран

	s_button2.setTextureRect(IntRect(0, 100, 130, 100));//рисуем 
	s_button2.setPosition(300, 430);//задаем начальные координаты появления спрайта
	window.draw(s_button2);//рисуем квадратики на экран

	s_button3.setTextureRect(IntRect(0, 200, 130, 100));//рисуем 
	s_button3.setPosition(550, 430);//задаем начальные координаты появления спрайта
	window.draw(s_button3);//рисуем квадратики на экран

	s_block.setTextureRect(IntRect(0, 0, 672, 90));//рисуем 
	s_block.setPosition(32, 10);//задаем начальные координаты появления спрайта
	window.draw(s_block);//рисуем квадратики на экран
	
	if ((win == true) && (isStep == false)) {
		text.setString(L"Вы выйграли! Может сыграем еще? ");//задает строку тексту
		text.setPosition(100, 25);//задаем позицию текста, центр камеры
		window.draw(text);//рисую этот текст
	}
	else if (win == true) {
		text.setString(L"Вы проиграли. Может сыграем еще?");//задает строку тексту
		text.setPosition(120, 25);//задаем позицию текста, центр камеры
		window.draw(text);//рисую этот текст
	}
	if ((isHit == false) && (isStep == true) && (win == false)) {
		text.setString(L"Ваш ход!");//задает строку тексту
		text.setPosition(300, 25);//задаем позицию текста, центр камеры
		window.draw(text);//рисую этот текст
	}
	else if ((isHit != false) && (isStep != true)) {
		text.setString(L"Ход противника!");//задает строку тексту
		text.setPosition(250, 25);//задаем позицию текста, центр камеры
		window.draw(text);//рисую этот текст
	}

	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP;j++)
		{
			if (TileMap[i][j] == wall)
				s_map.setTextureRect(IntRect(160, 0, 32, 32)); //если встретили 's', то рисуем empty
			if ((TileMap[i][j] == one_ship) && (i <= 12) && (i >= 3) && (j <= 10) && (j >= 1))
				s_map.setTextureRect(IntRect(32, 0, 32, 32));
			if ((TileMap[i][j] == one_ship) && (i <= 12) && (i >= 3) && (j <= 21) && (j >= 12))
				s_map.setTextureRect(IntRect(0, 0, 32, 32));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!00000
			if (TileMap[i][j] == dot)
				s_map.setTextureRect(IntRect(0, 0, 32, 32));
			if ((TileMap[i][j] == '1'))
				s_map.setTextureRect(IntRect(64, 0, 32, 32));//если встретили '1', то рисуем картинку "попал мимо" (точка)
			if ((TileMap[i][j] == '2'))
				s_map.setTextureRect(IntRect(96, 0, 32, 32));// если встретили '2', то рисуем картинку "стоит корабль" (крестик)
			s_map.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
			window.draw(s_map);//рисуем квадратики на экран
		};
	window.display();
}

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

int seachShipByCompStep1(int i, int j, Music & hit_music) {
	int hit = 1;
	if (TileMap[i][j] == dot) { //пустота, значит мимо
		sleep(milliseconds(stop_time));
		TileMap[i][j] = past;
		hit = 0;
	}else if (TileMap[i][j] == one_ship) {//попал, значит ранил
		sleep(milliseconds(stop_time));
		hit_music.play();
		TileMap[i][j] = hurt;
		hit = 1;
	}else if ((TileMap[i][j] == hurt)||(TileMap[i][j] == past))
		hit = 2;
	return hit;
}
int seachShipByCompStep2(int i, int j){
	int dir;//1-вверх, 2-вправо, 3-вниз, 4-влево
	bool hit = true;
	bool left = true;
	bool right = true;
	bool up = true;
	bool down = true;
	if (TileMap[i - 1][j] == past)
		up = false; //прохода нет
	if (TileMap[i][j + 1] == past)
		right = false;
	if (TileMap[i + 1][j] == past)
		down = false;
	if (TileMap[i][j - 1] == past)
		left = false;
	if (TileMap[i - 1][j] == past) //встретили вверху стенку
		up = false;
	if (TileMap[i + 1][j] == wall)
		down = false;
	if (TileMap[i][j - 1] == wall)
		 left = false;
	if (TileMap[i][j + 1] == wall)
		right = false;
	if (((TileMap[i - 1][j] == hurt) || (TileMap[i - 1][j] == past)) && ((TileMap[i + 1][j] == hurt) || (TileMap[i + 1][j] == past)) && ((TileMap[i][j - 1] == hurt) || (TileMap[i][j - 1] == past)) && ((TileMap[i][j + 1] == hurt) || (TileMap[i][j + 1] == past)))
		hit = false;
	if ((up == false) && (down == false) && (left == false) && (right == false))//если вокруг все точки, то это однопалубный корабль и он убит
		hit = false;
	else if ((up == true) && (down == true) && (left == true) && (right == true)) {//если вокруг нет "точек", рандомно ищем направление
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
	if (TileMap[i][j] == one_ship) {
		sleep(milliseconds(stop_time));
		hit_music.play();
		TileMap[i][j] = hurt;
		wounded = true;
	}
	else if ((TileMap[i][j] == wall) || (TileMap[i][j] == hurt) || (TileMap[i][j] == past))
		wounded = false;//error
	else {
		sleep(milliseconds(stop_time));
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
	if (TileMap[i][j] == one_ship) {
		sleep(milliseconds(stop_time));
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
		sleep(milliseconds(stop_time));
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
			sleep(milliseconds(stop_time));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == one_ship) || (TileMap[i][j] == hurt)) {
			sleep(milliseconds(stop_time));
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
			sleep(milliseconds(stop_time));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == one_ship) || (TileMap[i][j] == hurt)) {
			sleep(milliseconds(stop_time));
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
			sleep(milliseconds(stop_time));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == one_ship) || (TileMap[i][j] == '3')) {
			sleep(milliseconds(stop_time));
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
			sleep(milliseconds(stop_time));
			TileMap[i][j] = past;
			return false;
		}
		else if ((TileMap[i][j] == one_ship) || (TileMap[i][j] == hurt)) {
			sleep(milliseconds(stop_time));
			hit_music.play();
			TileMap[i][j] = hurt;
			j++;
			return true;
		}
		else if (TileMap[i][j] == wall)
			return false;
	}
}
void alignmentOfComputerOneShip(int si, int sj)//расстановка кораблей компьютером
{	
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
			if ((TileMap[i][j] != ' ') && (i >= si) && (i < si + 10) && (j >= sj) && (j < sj + 10))
				TileMap[i][j] = ' ';
	for (int LengthShip = 4; LengthShip > 0; LengthShip--) {
		int k = 1; //используем переменную к для цикла
		int i;
		int j;
		int location;
		bool loose;
		int count;
		while (k <= (5 - LengthShip)) {
			i = rand() % 10 + si;
			j = rand() % 10 + sj;
			location = rand() % 2 + 1; //расположение корабля, 1 - горизонтально, 2 - вертикально
			if ((i < si + 10) && (j < sj + 10) && (TileMap[i][j] == ' '))
				if (location == 1) {
					loose = true;
					if ((j + LengthShip) < sj + 10) {
						if (TileMap[i][j - 1] == one_ship)
							loose = false;
						for (count = 0; count <= LengthShip; count++)//проверяем, совбодны ли ячейки рядом
							if ((TileMap[i][j + count] != ' ') || (TileMap[i - 1][j + count] == one_ship) || (TileMap[i + 1][j + count] == one_ship))
								loose = false;
						if ((TileMap[i][j + LengthShip] == one_ship) || (TileMap[i - 1][j + LengthShip] == one_ship) || (TileMap[i + 1][j + LengthShip] == one_ship))//проверяем последний квадратик на возможность поставить кораблик
							loose = false;
						if (loose == true) { //если можно разместить корабль, то размещаем
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
								TileMap[i][j + count] = one_ship;
								if (TileMap[i - 1][j + count] != wall)
									TileMap[i - 1][j + count] = dot;
								if (TileMap[i + 1][j + count] != wall)
									TileMap[i + 1][j + count] = dot;
							}
							k++;
						}
					}
					else if (location == 2) { //вертикально
						loose = true;
						if ((i + LengthShip) < si + 10)
							if (TileMap[i - 1][j] == one_ship)//проверяем предыдущую ячейку
								loose = false;
						for (count = 1; count < LengthShip; count++)//проверяем, совбодны ли ячейки рядом
							if ((TileMap[i + count][j] != ' ') || (TileMap[i + count][j + 1] == one_ship) || (TileMap[i + count][j - 1] == one_ship))
								loose = false;
						if ((TileMap[i + LengthShip][j] == one_ship) || (TileMap[i + LengthShip][j - 1] == one_ship) || (TileMap[i + LengthShip][j + 1] == one_ship))//проверяем последний квадратик на возможность поставить кораблик
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
								TileMap[i + count][j] = one_ship;
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
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
			if ((TileMap[i][j] == ' ') && (i >= si) && (i < si + 10) && (j >= sj) && (j < sj + 10))
				TileMap[i][j] = dot;
}
void ComputerMove(struct intelligence *intel, Music & hit_music) {
	(*intel).isHit = true;
	if ((*intel).step1 == true) {
		(*intel).x = rand() % 10 + 3;
		(*intel).y = rand() % 10 + 1;
		(*intel).step = seachShipByCompStep1((*intel).x, (*intel).y, hit_music);
		if ((*intel).step == 0) {//на первом шаге попали мимо
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
	///////////////////////ШАГ 2//////////////////
	if ((*intel).step2 == true) {//поиск направления для удара по следующей ячейке, только когда в первый раз попали по ячейке
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
	///////////////////////ШАГ 3/////////////////
	if ((*intel).step3 == true) {//проверяем направление
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
	///////////////////////ШАГ 4//////////////////
	if ((*intel).step4 == true) {//прострел корабля по заданному направлению
		if (seachShipByCompStep4((*intel).dir, (*intel).x, (*intel).y, hit_music) == 0) {
			(*intel).isHit = false;
			(*intel).step4 = false;
			(*intel).step5 = true;//пойдем в другую чторону проверять на полное убийство корабля
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
		else if (seachShipByCompStep4((*intel).dir, (*intel).x, (*intel).y, hit_music) == 2) {//встретили стенку или уже попадали на эти координаты
			(*intel).isHit = true;
			(*intel).step4 = false;
			(*intel).step5 = true;//пойдем в другую чторону проверять на полное убийство корабля
		}
	}
	///////////////////////ШАГ 5//////////////////
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
	if (TileMap[i][j] == dot) { //мимо
		TileMap[i][j] = past; //ставим точку
		//water_music.play();
		(*intel).hit = false;
		//intel.numbStep++;
	}
	else if (TileMap[i][j] == one_ship) {//попал
		TileMap[i][j] = hurt;//ставим крестик
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
	
	Image mapImage; //создаём объект Image(изображение)
	mapImage.loadFromFile("images/map.psd");
	Texture map;//текстура карты
	map.loadFromImage(mapImage);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Image wallImage; //создаём объект Image(изображение)
	wallImage.loadFromFile("images/wall.png");
	Texture wall;//текстура карты
	wall.loadFromImage(wallImage);//заряжаем текстуру картинкой
	Sprite wallSprite;//создаём спрайт для карты 
	wallSprite.setTexture(wall);//заливаем текстуру спрайтом

	Image buttonImage; //создаём объект Image(изображение)
	buttonImage.loadFromFile("images/buttons.psd");
	Texture button;//текстура карты
	button.loadFromImage(buttonImage);//заряжаем текстуру картинкой
	Sprite s_button1;//создаём спрайт для карты 
	s_button1.setTexture(button);//заливаем текстуру спрайтом
	Sprite s_button2;//создаём спрайт для карты 
	s_button2.setTexture(button);//заливаем текстуру спрайтом
	Sprite s_button3;//создаём спрайт для карты 
	s_button3.setTexture(button);//заливаем текстуру спрайтом

	Image blockImage; //создаём объект Image(изображение)
	blockImage.loadFromFile("images/block.psd");
	Texture block;//текстура карты
	block.loadFromImage(blockImage);//заряжаем текстуру картинкой
	Sprite s_block;//создаём спрайт для карты 
	s_block.setTexture(block);//заливаем текстуру спрайтом

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

	Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Black);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	
	/////////////////////РАССТАВЛЯЕМ КОРАБЛИ///////////////////
	srand(time(NULL));

	alignmentOfComputerOneShip(si, sj1);
	alignmentOfComputerOneShip(si, sj2);
	//////////////////////////КОНЕЦ///////////////////////////////
	while (window.isOpen())
	{
		sf::Vector2i pixPos = sf::Mouse::getPosition(window);
		int i = (pixPos.y / 32);
		int j = (pixPos.x / 32);

		render(window, s_map, wallSprite, s_button1, s_block, s_button2, s_button3, text, intel.isHit, intel.isStep, intel.win);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					if (s_button3.getGlobalBounds().contains(pixPos.x, pixPos.y))
						window.close();
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					if (s_button2.getGlobalBounds().contains(pixPos.x, pixPos.y)) {
						alignmentOfComputerOneShip(si, sj1);

					}
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					if (s_button1.getGlobalBounds().contains(pixPos.x, pixPos.y)) {
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
				///////////////////////ХОД ИГРОКА///////////////////////////
				//cout << "MOVE THE PLAYER! isStep = "<< intel.isStep << ", isHit = " << intel.isHit << "\n";
				if ((intel.isStep == true) && (intel.isHit == false)) {
					if (event.type == Event::MouseButtonPressed)
						if (event.key.code == Mouse::Left)
							if ((i <= 12) && (i >= 3) && (j <= 21) && (j >= 12)) //и при этом координата курсора попадает
								PlayerMove(i, j, &intel, hit_music);
				}else
					intel.isStep = false;
				render(window, s_map, wallSprite, s_button1, s_block, s_button2, s_button3, text, intel.isHit, intel.isStep, intel.win);
				////////////////////////ХОД БОТА////////////////////////////////
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