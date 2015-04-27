#ifndef _LEVEL1_H              // Prevent multiple definitions if this
#define _LEVEL1_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "Levels.h"
#include "megaman.h"

namespace level1NS
{
	const char FONT[] = "Arial Bold";  // font
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const int TEXTURE_SIZE = 64;
	const int TEXTURE_COLS = 0;
	const int TILE_ROWS = 32;
	const int TILE_COLUMNS = 64;
	const float SCROLL_RATE = 1;
	const UINT MAP_WIDTH = TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH;

	const RECT MEGAMAN_EDGE = { -TEXTURE_SIZE / 2, -TEXTURE_SIZE / 2, TEXTURE_SIZE / 2, TEXTURE_SIZE / 2 };

	const int F1 = 0;
	const int __ = -1;                  // empty tile

	// The world map
	const int tileMap[TILE_ROWS][TILE_COLUMNS] = {
		F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, //0
		F1, __, __, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//1
		F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//2
		F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//3
		F1, __, __, __, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//4
		F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//5
		F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//6
		F1, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, __, __, __, F1, F1, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,//7
		F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,//8
		F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,//9
		F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,//10
		F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,//11
		F1, __, __, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, __, __, F1, F1, F1, __, __, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, __, __, F1, F1,//12
		F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1,//13
		F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1,//14
		F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
		F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,
		F1, __, __, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, __, __, __, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1, F1, F1, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, F1, __, __, __, __, F1, F1,
		F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,
		F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,
		F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,
		F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1,
		F1, __, __, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, __, __, F1, F1, F1, __, __, F1, F1, F1, F1, __, __, __, __, __, F1, F1, F1, F1, __, __, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, __, __, F1, F1,
		F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1,
		F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1, F1, __, __, __, __, __, __, F1, F1, F1, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __, F1,
		F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1, F1
	};//0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
}

class Level1 : public Levels
{
public:
	Level1();
	~Level1();

	// Initialize the game
	void initializeAdditional(HWND& hwnd, Graphics* graphics, Input* input, Game* game);

	void update(float frameTime, Input* input, Game* game);
	void ai();          // "
	void collisions();  // "
	void render(Graphics* graphics);      // "
	void releaseAll();
	void resetAll();
};

#endif