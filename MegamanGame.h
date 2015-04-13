#ifndef _GRAVITY_H              // Prevent multiple definitions if this
#define _GRAVITY_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "megaman.h"
#include "paddle.h"
#include "bullet.h"
#include "bulletChargedSmall.h"
#include "chargingSprites.h"
#include <vector>

//=============================================================================
// This class is the core of the game
//=============================================================================
class MegamanGame : public Game
{
private:
    // game items
    TextureManager backdropTexture;		 // backdrop texture
	TextureManager paddleTexture;		// paddle texture
    TextureManager megamanTexture;		// megaman texture
	TextureManager bulletTexture;		// bullet texture
	TextureManager bulletChargedSmallTexture;	//bullet charged small texture
	TextureManager chargingSpritesTexture;
	Paddle	paddle;						// paddle
    Megaman	megaman;					// megaman
	chargingSprites chargingSprites;	// charging Sprites
	std::vector<Bullet>	bullet;			// bullet
	BulletChargedSmall bulletChargedSmall;				// bullet charged small
    Image   backdrop;					// backdrop image
	float   mapX, mapY;

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
