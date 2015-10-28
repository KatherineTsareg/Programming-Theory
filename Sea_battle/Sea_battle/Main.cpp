#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.h"
#include <iostream>
#include <ctime>

using namespace sf;
using namespace std;

const int maxLengthShip = 4;
const float blockSize = 32.f;
const int windowHeigh = blockSize * heighMap;
const int windowWidth = blockSize * widthMap;
const int fieldSize = 10;
const char wall = 's';
const char empty123 = ' ';
const char isShip = 'X';
const char dot = '.';
const char past = '1';//мимо
const char hurt = '2';//ранил
const int si = 3;
const int sj1 = 1;
const int sj2 = 12;
const int stopTime = 600;
const int upDir = 1;
const int rightDir = 2;
const int downDir = 3;
const int leftDir = 4;
const int falseReturn = 0;
const int trueReturn = 1;
const int errorReturn = 2;

struct GraphicResource {
	Image* im;
	Texture* tex;
	Sprite* spr;
};

void CleanTheMemory(GraphicResource & gr) {
	delete gr.im;
	delete gr.tex;
	delete gr.spr;
}

GraphicResource MakeTheSprite (String fileName) {
	GraphicResource gr;
	gr.im = new Image();
	gr.tex = new Texture();
	gr.spr = new Sprite();
	gr.im->loadFromFile("images/" + fileName);
	gr.tex->loadFromImage(*gr.im);
	gr.spr->setTexture(*gr.tex);
	return gr;
}

void DrawTheSprite(RenderWindow & window, Sprite & sprite, int i, int j, int spriteWidth, int spriteHeigh, int x, int y) {
	sprite.setTextureRect(IntRect(i, j, spriteWidth, spriteHeigh));//рисуем 
	sprite.setPosition(x, y);//задаем начальные координаты появления спрайта
	window.draw(sprite);//рисуем квадратики на экран
}

void DrawTheText(RenderWindow & window, Text text, String string, int x, int y) {
	text.setString(string);//задает строку тексту
	text.setPosition(x, y);//задаем позицию текста, центр камеры
	window.draw(text);//рисую этот текст
}

void Render(Sprite & background, RenderWindow & window, Sprite & map, Sprite & button1, Sprite & block, Sprite & button2, Sprite & button3, Text text, Game * g)
{
	window.clear();
	
	DrawTheSprite(window, background, 0, 0, windowWidth, windowHeigh, 0, 0);
	DrawTheSprite(window, button1, 0, 0, 130, 100, 50, 430);
	DrawTheSprite(window, button2, 0, 100, 130, 100, 300, 430);
	DrawTheSprite(window, button3, 0, 200, 130, 100, 550, 430);
	DrawTheSprite(window, block, 0, 0, 672, 90, 32, 10);
	
	if ((g->win) && (g->numbCompShips == 0))
		DrawTheText(window, text, L"Вы выйграли! Может сыграем еще?", 100, 25);
	else if ((g->win) && (g->numbPLayerShips == 0))
		DrawTheText(window, text, L"Вы проиграли. Может сыграем еще?", 120, 25);
	if ((!g->isHit) && (g->isStep) && (!g->win))
		DrawTheText(window, text, L"Ваш ход!", 300, 25);
	else if ((g->isHit) && (!g->isStep))
		DrawTheText(window, text, L"Ход противника!", 250, 25);

	for (int i = 0; i < heighMap; i++)
		for (int j = 0; j < widthMap; j++)
		{
			if (TileMap[i][j] == wall)
				map.setTextureRect(IntRect(160, 0, blockSize, blockSize)); //если встретили 's', то рисуем empty
			if ((TileMap[i][j] == isShip) && (i < si + fieldSize) && (i >= si) && (j < sj1 + fieldSize) && (j >= sj1))
				map.setTextureRect(IntRect(blockSize, 0, blockSize, blockSize));
			if ((TileMap[i][j] == isShip) && (i < si + fieldSize) && (i >= si) && (j < sj2 + fieldSize) && (j >= sj2))
				map.setTextureRect(IntRect(0, 0, blockSize, blockSize));//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!00000
			if (TileMap[i][j] == dot)
				map.setTextureRect(IntRect(0, 0, blockSize, blockSize));
			if ((TileMap[i][j] == past))
				map.setTextureRect(IntRect(blockSize * 2, 0, blockSize, blockSize));//если встретили '1', то рисуем картинку "попал мимо" (точка)
			if ((TileMap[i][j] == hurt))
				map.setTextureRect(IntRect(blockSize * 3, 0, blockSize, blockSize));// если встретили '2', то рисуем картинку "стоит корабль" (крестик)
			
			map.setPosition(j * blockSize, i * blockSize);
			window.draw(map);//рисуем квадратики на экран
		}
	window.display();
}

int SeachShipByCompStep1(int i, int j, Music & hit_music) {
	if (TileMap[i][j] == dot) { //пустота, значит мимо
		sleep(milliseconds(stopTime));
		TileMap[i][j] = past;
		return falseReturn;
	}else if (TileMap[i][j] == isShip) {//попал, значит ранил
		sleep(milliseconds(stopTime));
		hit_music.play();
		TileMap[i][j] = hurt;
		return trueReturn;
	}else if ((TileMap[i][j] == hurt)||(TileMap[i][j] == past))
		return errorReturn;
}

int SeachShipByCompStep2(int i, int j) {
	int dir;//1-вверх, 2-вправо, 3-вниз, 4-влево
	bool hit = true;
	bool left = true;
	bool right = true;
	bool up = true;
	bool down = true;
	if ((TileMap[i - 1][j] == past) || (TileMap[i - 1][j] == wall) || (TileMap[i - 1][j] == hurt))
		up = false; //прохода нет
	if ((TileMap[i][j + 1] == past) || (TileMap[i][j + 1] == wall) || (TileMap[i][j + 1] == hurt))
		right = false;
	if ((TileMap[i + 1][j] == past) || (TileMap[i + 1][j] == wall) || (TileMap[i + 1][j] == hurt))
		down = false;
	if ((TileMap[i][j - 1] == past) || (TileMap[i][j - 1] == wall) || (TileMap[i][j - 1] == hurt))
		left = false;
	if ((!up) && (!down) && (!left) && (!right))//если вокруг все точки, то это однопалубный корабль и он убит
		hit = false;
	if (hit) {
		int* Arr;
		Arr = new int[4];
		int k = 0;
		if (up) {
			Arr[k] = 1;
			k++;
		}if (right) {
			Arr[k] = 2;
			k++;
		}if (down) {
			Arr[k] = 3;
			k++;
		}if (left) {
			Arr[k] = 4;
			k++;
		}if (k == 1)
			return Arr[0];
		else {
			dir = rand() % k;
			return Arr[dir];
		}
		delete[]Arr;
	}
	else {
		return falseReturn;
	}
}

bool SeachShipByCompStep3(int dir, int i, int j, Music & hit_music) 
{
	bool wounded = false;
	if (dir == upDir)
		i--;
	if (dir == rightDir)
		j++;
	if (dir == downDir)
		i++;
	if (dir == leftDir)
		j--;
	if (TileMap[i][j] == isShip) {
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

int SeachShipByCompStep4(int dir, int i, int j, Music & hit_music)
{
	int wounded = 0;
	if (dir == upDir)
		i--;
	if (dir == rightDir)
		j++;
	if (dir == downDir)
		i++;
	if (dir == leftDir)
		j--;
	if (TileMap[i][j] == isShip) {
		sleep(milliseconds(stopTime));
		hit_music.play();
		TileMap[i][j] = hurt;
		wounded = 1;
	}else if (TileMap[i][j] == hurt) {
		TileMap[i][j] = hurt;
		wounded = trueReturn;
	}else if (TileMap[i][j] == wall)
		wounded = errorReturn;//error
	else if (TileMap[i][j] == past)
		wounded = falseReturn;
	else {
		sleep(milliseconds(stopTime));
		TileMap[i][j] = past;
		wounded = falseReturn;
	}
	return wounded;
}

bool Firing(int i, int j, Music & hit_music)
{
	if ((TileMap[i][j] == past) || (TileMap[i][j] == dot))
	{
		sleep(milliseconds(stopTime));
		TileMap[i][j] = past;
		return false;
	}
	else if ((TileMap[i][j] == isShip) || (TileMap[i][j] == hurt))
	{
		sleep(milliseconds(stopTime));
		hit_music.play();
		TileMap[i][j] = hurt;
		return true;
	}
	else if (TileMap[i][j] == wall)
		return false;
}

bool SeachShipByCompStep5(int dir, int i, int j, Music & hit_music) {
	bool hit = false;
	while (TileMap[i][j] == hurt) {
		if (dir == upDir)
			i++;
		if (dir == rightDir)
			j--;
		if (dir == downDir)
			i--;
		if (dir == leftDir)
			j++;
	}
	if (Firing(i, j, hit_music)) {
		hit = true;
		if (dir == upDir)
			i++;
		if (dir == rightDir)
			j--;
		if (dir == downDir)
			i--;
		if (dir == leftDir)
			j++;
	}
	return hit;
}

void CompleteField(int si, int sj, char symbol) {
	for (int i = si; i < si + fieldSize; i++)
		for (int j = sj; j < sj + fieldSize; j++) 
		{
			if (symbol == empty123) {
				if (TileMap[i][j] != empty123)
					TileMap[i][j] = symbol;
			}
			else if (symbol == dot) 
			{
				if (TileMap[i][j] == empty123)
					TileMap[i][j] = symbol;
			}
		}
}

void TestingAndPlacementOfShips(AlignmentShips * as, int sj, int si, int LengthShip, int horizontal, int vertical) {
	as->loose = true;
	if ((as->j * horizontal + as->i * vertical + LengthShip) < (sj *horizontal + si*vertical + fieldSize)) {
		if (TileMap[as->i - vertical][as->j - horizontal] == isShip)
			as->loose = false;
		for (int count = 0; count <= LengthShip; count++)//проверяем, совбодны ли ячейки рядом
			if ((TileMap[as->i + (count * vertical)][as->j + count * horizontal] != empty123) || (TileMap[as->i - horizontal + count * vertical][as->j - vertical + count * horizontal] == isShip) || (TileMap[as->i + horizontal + count * vertical][as->j + vertical + count * horizontal] == isShip))
				as->loose = false;
		if ((TileMap[as->i + LengthShip * vertical][as->j + LengthShip * horizontal] == isShip) || (TileMap[as->i - horizontal + LengthShip * vertical][as->j + LengthShip * horizontal - vertical] == isShip) || (TileMap[as->i + horizontal + LengthShip * vertical][as->j + LengthShip * horizontal + vertical] == isShip))//проверяем последний квадратик на возможность поставить кораблик
			as->loose = false;
		if (as->loose) { //если можно разместить корабль, то размещаем
			if (TileMap[as->i - vertical][as->j - horizontal] != wall)
				TileMap[as->i - vertical][as->j - horizontal] = dot;
			if (TileMap[as->i - horizontal - vertical][as->j - horizontal - vertical] != wall)
				TileMap[as->i - horizontal - vertical][as->j - horizontal - vertical] = dot;
			if (TileMap[as->i + horizontal - vertical][as->j - horizontal + vertical] != wall)
				TileMap[as->i + horizontal - vertical][as->j - horizontal + vertical] = dot;
			if (TileMap[as->i + LengthShip * vertical][as->j + LengthShip * horizontal] != wall)
				TileMap[as->i + LengthShip * vertical][as->j + LengthShip * horizontal] = dot;
			if (TileMap[as->i - horizontal + LengthShip * vertical][as->j + LengthShip * horizontal - vertical] != wall)
				TileMap[as->i - horizontal + LengthShip * vertical][as->j + LengthShip * horizontal - vertical] = dot;
			if (TileMap[as->i + horizontal + LengthShip * vertical][as->j + LengthShip * horizontal + vertical] != wall)
				TileMap[as->i + horizontal + LengthShip * vertical][as->j + LengthShip * horizontal + vertical] = dot;
			for (int count = 0; count < LengthShip; count++) {
				TileMap[as->i + count * vertical][as->j + count * horizontal] = isShip;
				if (TileMap[as->i - horizontal + count * vertical][as->j + count * horizontal - vertical] != wall)
					TileMap[as->i - horizontal + count * vertical][as->j + count * horizontal - vertical] = dot;
				if (TileMap[as->i + horizontal + count * vertical][as->j + count * horizontal + vertical] != wall)
					TileMap[as->i + horizontal + count * vertical][as->j + count * horizontal + vertical] = dot;
			}
			as->k++;
		}
	}
}

void AlignmentOfComputerOneShip(AlignmentShips * as, int si, int sj)//расстановка кораблей компьютером
{
	CompleteField(si, sj, empty123);
	for (int LengthShip = 4; LengthShip > 0; LengthShip--) 
	{
		as->k = 1;
		while (as->k <= (5 - LengthShip))
		{
			as->i = rand() % fieldSize + si;
			as->j = rand() % fieldSize + sj;
			as->location = rand() % 2 + 1; //расположение корабля, 1 - горизонтально, 2 - вертикально
			if ((as->i < si + fieldSize) && (as->j < sj + fieldSize) && (TileMap[as->i][as->j] == empty123))
				if (as->location == 1) //если расположение ГОРИЗОНТАЛЬНОЕ
				{ 
					as->horizontal = 1;
					as->vertical = 0;
					TestingAndPlacementOfShips(as, sj, si, LengthShip, as->horizontal, as->vertical);
				}
				else if (as->location == 2)  //вертикально
				{
					as->horizontal = 0;
					as->vertical = 1;
					TestingAndPlacementOfShips(as, sj, si, LengthShip, as->horizontal, as->vertical);
				}
		}
	}
	CompleteField(si, sj, dot);
}

void ComputerMove (Game * g, Music & hit_music) {
	g->isHit = true;
	g->isStep = true;
	if (g->step1) {
		g->x = rand() % fieldSize + si;
		g->y = rand() % fieldSize + sj1;
		g->step = SeachShipByCompStep1(g->x, g->y, hit_music);
		if (g->step == falseReturn) {//на первом шаге попали мимо
			g->isHit = false;
			g->step2 = false;
			g->step1 = true;
		}else if (g->step == trueReturn) {
			g->isHit = true;
			g->step2 = true;
			g->step1 = false;
			g->numbPLayerShips--;
		}else if (g->step == errorReturn) {
			g->step1 = true;
			g->isHit = true;
			g->step2 = false;
		}
	}
	if (g->step2) {//поиск направления для удара по следующей ячейке, только когда в первый раз попали по ячейке
		g->dir = SeachShipByCompStep2(g->x, g->y);
		if (g->dir != falseReturn) {
			g->step2 = false;
			g->step3 = true;
			g->isHit = true;
		}else {
			g->step1 = true;
			g->step2 = false;
			g->isHit = true;
		}
	}
	if (g->step3) {//проверяем направление
		if (SeachShipByCompStep3(g->dir, g->x, g->y, hit_music) == true) {
			g->step4 = true;
			g->step3 = false;
			g->isHit = true;
		}else {
			g->step2 = true;
			g->step3 = false;
			g->isHit = false;
		}
	}
	if (g->step4) {//прострел корабля по заданному направлению
		if (SeachShipByCompStep4(g->dir, g->x, g->y, hit_music) == falseReturn) {
			g->isHit = false;
			g->isStep = true;
			g->step4 = false;
			g->step5 = true;//пойдем в другую чторону проверять на полное убийство корабля
		}else if (SeachShipByCompStep4(g->dir, g->x, g->y, hit_music) == trueReturn) {
			g->isHit = true;
			g->numbPLayerShips--;
			if (g->dir == upDir)
				g->x--;
			if (g->dir == rightDir)
				g->y++;
			if (g->dir == downDir)
				g->x++;
			if (g->dir == leftDir)
				g->y--;
		}else if (SeachShipByCompStep4(g->dir, g->x, g->y, hit_music) == errorReturn) {//встретили стенку или уже попадали на эти координаты
			g->isHit = true;
			g->step4 = false;
			g->step5 = true;//пойдем в другую чторону проверять на полное убийство корабля
		}
	}
	if ((g->step5) && (g->isHit)) {
		if (!SeachShipByCompStep5(g->dir, g->x, g->y, hit_music)) {
			g->step5 = false;
			g->step1 = true;
			g->isHit = false;
		}else {
			g->isHit = true;
			g->step5 = true;
			g->numbPLayerShips--;
		}
	}
}

void PlayerMove(int i, int j, struct Game *g, Music & hit_music) {
	if (TileMap[i][j] == dot) { //мимо
		TileMap[i][j] = past; //ставим точку
		g->hit = false;
	}
	else if (TileMap[i][j] == isShip) {//попал
		TileMap[i][j] = hurt;//ставим крестик
		hit_music.play();
		g->numbCompShips--;
		g->hit = true;
	}
	g->isStep = false;
}

void Reset (Game *g) {
	g->isStep = true;
	g->hit = true;
	g->isHit = false;
	g->step1 = true;
	g->step2 = false;
	g->step3 = false;
	g->step4 = false;
	g->step5 = false;
	g->win = false;
	g->alighmentShips = true;
	g->dir = 0;
	g->numbCompShips = 20;
	g->numbPLayerShips = 20;
	g->x = 0;
	g->y = 0;
}

void GameWithComputer(RenderWindow & window, Sprite & button1, Sprite & button2, Sprite & button3, Sprite & background, Sprite & map, Sprite & block,  Music & hit_music, Text & mainText, GraphicResource stMap, GraphicResource stBackground, GraphicResource stButton, GraphicResource stBlock) {
	Game g;
	AlignmentShips as;

	srand(time(NULL));
	AlignmentOfComputerOneShip(&as, si, sj1);
	AlignmentOfComputerOneShip(&as, si, sj2);

	while (window.isOpen())
	{
		Vector2i pixPos = Mouse::getPosition(window);
		int i = (pixPos.y / blockSize);
		int j = (pixPos.x / blockSize);

		Render(background, window, map, button1, block, button2, button3, mainText, &g);
		Event event;
		while (window.pollEvent(event))
		{
			////////////////////проверяем на нажатие кнопок////////////////////////////
			if (event.type == Event::Closed)
				window.close();
			if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Left))
				if (button1.getGlobalBounds().contains(pixPos.x, pixPos.y)) {
					AlignmentOfComputerOneShip(&as, si, sj1);
					AlignmentOfComputerOneShip(&as, si, sj2);
					Reset(&g);
				}
				else if (button2.getGlobalBounds().contains(pixPos.x, pixPos.y) && (g.alighmentShips == true))
					AlignmentOfComputerOneShip(&as, si, sj1);
				else if (button3.getGlobalBounds().contains(pixPos.x, pixPos.y))
					window.close();
				//////////////////////////НАЧАЛО ИГРЫ/////////////////////////////
				if ((g.numbCompShips > 0) && (g.numbPLayerShips > 0)) {
					///////////////////////ХОД ИГРОКА///////////////////////////
					if ((g.isStep) && (!g.isHit)) {
						if ((event.type == Event::MouseButtonPressed) && (event.key.code == Mouse::Left))
							if ((i < si + fieldSize) && (i >= si) && (j < sj2 + fieldSize) && (j >= sj2)) { //и при этом координата курсора попадает
								PlayerMove(i, j, &g, hit_music);
								g.alighmentShips = false;
							}
					}
					else
						g.isStep = false;
					Render(background, window, map, button1, block, button2, button3, mainText, &g);
					////////////////////////ХОД БОТА////////////////////////////////
					if ((!g.isStep) && (!g.hit)) {
						ComputerMove(&g, hit_music);
						g.isStep = true;
					}
					else
						g.isStep = true;
				}
				else if ((g.numbCompShips <= 0) || (g.numbPLayerShips <= 0)) {
					g.isHit = false;
					g.isStep = false;
					g.step1 = false;
					g.step2 = false;
					g.step3 = false;
					g.step4 = false;
					g.step5 = true;
					g.win = true;
				}
		}
	}
	CleanTheMemory(stMap);
	CleanTheMemory(stBackground);
	CleanTheMemory(stButton);
	CleanTheMemory(stBlock);
}

int main(){
	RenderWindow window(VideoMode(windowWidth, windowHeigh), "Sea War!");
	Game g;
	AlignmentShips as;

	GraphicResource stMap = MakeTheSprite("map.psd");
	Sprite map = *stMap.spr;
	GraphicResource stBackground = MakeTheSprite("wall.png");
	Sprite background = *stBackground.spr;
	GraphicResource stButton = MakeTheSprite("buttons.psd");
	Sprite button1 = *stButton.spr;
	Sprite button2 = *stButton.spr;
	Sprite button3 = *stButton.spr;
	GraphicResource stBlock = MakeTheSprite("block.psd");
	Sprite block = *stBlock.spr;

	Font font;//шрифт 
	font.loadFromFile("font.ttf");//передаем нашему шрифту файл шрифта
	Text mainText("", font, 30);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	mainText.setColor(Color::Black);//покрасили текст в черный

	Music hit_music;
	SoundBuffer hit_buffer;
	Sound hit_sound;
	hit_sound.setBuffer(hit_buffer);
	if (!hit_music.openFromFile("audio/hit.wav"))
		return -1;

	GameWithComputer(window, button1, button2, button3, background, map, block, hit_music, mainText, stMap, stBackground, stButton, stBlock);
	return 0;
}