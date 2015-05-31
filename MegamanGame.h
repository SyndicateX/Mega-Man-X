#ifndef _MEGA_MAN_X_H              // Prevent multiple definitions if this
#define _MEGA_MAN_X_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "megaman.h"
#include "enemy.h"
#include "bullet.h"
#include "chargingSprites.h"
#include <vector>
#include "sprites.h"
#include "Levels.h"
#include "Level1.h"
#include "Level2.h"

//=============================================================================
// This class is the core of the game
//=============================================================================
class MegamanGame : public Game
{
private:
    // game items
	Levels* level;

public:
    // Constructor
	MegamanGame();
    // Destructor
	virtual ~MegamanGame();
    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
	void updateMap();
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();
};

#endif
