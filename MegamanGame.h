#ifndef _MEGA_MAN_X_H              // Prevent multiple definitions if this
#define _MEGA_MAN_X_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "megaman.h"
#include "bullet.h"
#include "bulletChargedSmall.h"
#include "chargingSprites.h"
#include <vector>

namespace megamanGameNS
{
	const char FONT[] = "Arial Bold";  // font
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const int TEXTURE_SIZE = 128;
	const int TEXTURE_COLS = 0;
	const int MH = 8;
	const int MW = 16;
	const float SCROLL_RATE = 1;
	//const int POST_TOP = 11;
	//const int POST_BOTTOM = 15;
	//const float POST_X = TEXTURE_SIZE * 7;        // post position in map
	//const float POST_TOP_Y = TEXTURE_SIZE * 6;    //  "
	//const float POST_BOTTOM_Y = TEXTURE_SIZE * 7; //  "
	// collision box {left, top, right ,bottom} relative to center
	const RECT MEGAMAN_EDGE = { -TEXTURE_SIZE / 2, -TEXTURE_SIZE / 2, TEXTURE_SIZE / 2, TEXTURE_SIZE / 2 };

	// map tile legend 
	// C=cloud, G=grass, F=flower, W=web, S=seed, L=leaf, P=post
	// F=floor
	//const int C1 = 0, C2 = 1, F1T = 2, F2T = 3;
	//const int G1 = 4, G2 = 5, F1B = 6, F2B = 7;
	//const int W1 = 8, W2 = 9, S = 10, PT = 11;
	//const int CL = 12, CR = 13, L = 14, PB = 15;

	const int F1 = 0;
	const int __ = -1;                  // empty tile

	// The world map
	const int tileMap[MH][MW] = {
		F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, __,//0
		F1, __, __, __, __, F1, __, __, __, __, __, __, __, __, __, __,//1
		__, __, F1, __, __, F1, __, __, __, __, __, __, __, __, __, __,//2
		F1, F1, __, __, __, F1, __, __, __, __, __, __, __, __, __, __,//3
		__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,//4
		__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,//5
		__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __,//6
		__, __, __, __, __, __, __, __, __, __, __, __, __, __, __, __ //7
	};//0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15
}

//=============================================================================
// This class is the core of the game
//=============================================================================
class MegamanGame : public Game
{
private:
    // game items
    TextureManager backdropTexture;		// backdrop texture
	TextureManager tileTextures;		// tiles
    TextureManager megamanTexture;		// megaman texture
	TextureManager bulletTexture;		// bullet texture
	TextureManager bulletChargedSmallTexture;	//bullet charged small texture
	TextureManager chargingSpritesTexture;
    Megaman	megaman;					// megaman
	chargingSprites chargingSprites;	// charging Sprites
	std::vector<Bullet>	bullet;			// bullet
	BulletChargedSmall bulletChargedSmall;				// bullet charged small
	Image   backdrop;					// backdrop image
	Image	tile;
	std::vector<Entity>	floor;
	float   mapX, mapY, tileMapX, tileMapY;

public:
    // Constructor
	MegamanGame();
    // Destructor
	virtual ~MegamanGame();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
	void moveMegaman(double moveRate); // handles megaman's and the map's movements after input
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
