#include "MegamanGame.h"
#include "sprites.h"

//=============================================================================
// Constructor
//=============================================================================
MegamanGame::MegamanGame()
{
	level = new Level1();
}

//=============================================================================
// Destructor
//=============================================================================
MegamanGame::~MegamanGame()
{
    releaseAll();           // call onLostDevice() for every graphics item
	delete level;
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void MegamanGame::initialize(HWND hwnd)
{
	Game::initialize(hwnd);
	level->initialize(hwnd, graphics, input, this);
	level->initializeAdditional(hwnd, graphics, input, this);

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void MegamanGame::update()
{
	// A menu should go here

	if (level->isLevelComplete())
	{
		static int lCount = 0;
		delete level;
		if (lCount % 2 == 0)
		{
			level = new Level2();
		}
		else
		{
			level = new Level1();
		}
		level->initialize(hwnd, graphics, input, this);
		level->initializeAdditional(hwnd, graphics, input, this);
		lCount++;
	}
	level->update(frameTime, input, this);
}

void MegamanGame::ai()
{
	level->ai();
}

void MegamanGame::collisions()
{
	level->collisions(frameTime);
}

void MegamanGame::render()
{
	graphics->spriteBegin();                // begin drawing sprites
	level->render(graphics);
	graphics->spriteEnd();					// end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void MegamanGame::releaseAll()
{
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void MegamanGame::resetAll()
{
    return;
}
