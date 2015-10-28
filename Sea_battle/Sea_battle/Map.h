#include <SFML\Graphics.hpp>
#include <vld.h>

const int heighMap = 17;//размер карты высота
const int widthMap = 23;//размер карты ширина 

sf::String TileMap[heighMap] = {
	"sssssssssssssssssssssss",
	"sssssssssssssssssssssss",
	"sssssssssssssssssssssss",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"s          s          s",
	"sssssssssssssssssssssss",
	"sssssssssssssssssssssss",
	"sssssssssssssssssssssss",
	"sssssssssssssssssssssss",
};

struct Game {
	bool hit = true;
	bool alighmentShips = true;
	bool isStep = true;
	bool step1 = true;
	bool win = false;
	bool isHit = false;
	bool step2 = false;
	bool step3 = false;
	bool step4 = false;
	bool step5 = false;
	int step;
	int numbPLayerShips = 20;
	int numbCompShips = 20;
	int dir = 0;
	int x = 0;
	int y = 0;
};

struct AlignmentShips {
	int k = 1;
	int i, j, location, count, horizontal, vertical;
	bool loose;
};