#ifndef _LEVELS_H              // Prevent multiple definitions if this
#define _LEVELS_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "megaman.h"
#include "enemy.h"
#include "bullet.h"
#include "chargingSprites.h"
#include <vector>

//=============================================================================
// This class is the core of all the levels
//=============================================================================
class Levels
{
protected:
	// game items
	TextureManager backdropTexture;		// backdrop texture
	TextureManager tileTextures;		// tiles
	TextureManager megamanTexture;		// megaman texture
	TextureManager mechaSonicTexture;	// Mecha_Sonic.Forte.32 texture
	TextureManager bulletTexture;		// bullet texture
	TextureManager chargingSpritesTexture;

	Megaman	megaman;					// megaman
	Enemy mechaSonic;					// enemy

	chargingSprites chargingSprites;	// charging Sprites
	std::vector<Bullet> bullet;				// bullet charged small
	Image   backdrop;					// backdrop image
	Image	tile;
	std::vector<Entity>	floor;
	double   mapX, mapY, tileMapX, tileMapY;
	double oldX_;
	double oldY_;
	bool directionChange_;

public:
	// Constructor
	Levels();
	// Destructor
	virtual ~Levels();
	// Initialize the game
	void initialize(HWND& hwnd, Graphics* graphics, Input* input, Game* game);
	virtual void initializeAdditional(HWND& hwnd, Graphics* graphics, Input* input, Game* game) = 0;
	virtual void update(float frameTime, Input* input, Game* game) = 0;// { updateMegaman(100, 100); };
	virtual void updateMegaman(double MAP_WIDTH, double MAP_HEIGHT, float frameTime, Input* input, Game* game);
	void update2(double MAP_WIDTH, double MAP_HEIGHT);      // must override pure virtual from Game
	void leftRightInput(Direction direction, double MAP_WIDTH, double MAP_HEIGHT);
	void moveMegaman(double moveRatedouble, double MAP_WIDTH, double MAP_HEIGHT); // handles megaman's and the map's movements after input
	void shoot();		// shoots megaman's x-buster bullets
	virtual void ai() = 0;          // "
	virtual void collisions() = 0;  // "
	virtual void render(Graphics* graphics) = 0;      // "
	virtual void releaseAll();
	virtual void resetAll();
};

#endif
