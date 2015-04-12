

#ifndef _GRAVITY_H              // Prevent multiple definitions if this
#define _GRAVITY_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "ball.h"
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
    TextureManager ballTexture;			 // ball texture
	TextureManager paddleTexture;		// paddle texture
    TextureManager megamanTexture;		// megaman texture
	TextureManager bulletTexture;		// bullet texture
	TextureManager bulletChargedSmallTexture;	//bullet charged small texture
	TextureManager chargingSpritesTexture;
    Ball    ball;						// ball
	Paddle	paddle;						// paddle
    Megaman	megaman;					// megaman
	chargingSprites chargingSprites;	// charging Sprites
	std::vector<Bullet>	bullet;			// bullet
	BulletChargedSmall bulletChargedSmall;				// bullet charged small
    Image   backdrop;					// backdrop image

public:
    // Constructor
	MegamanGame();
    // Destructor
	virtual ~MegamanGame();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
