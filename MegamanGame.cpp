#include "MegamanGame.h"
#include "sprites.h"

using namespace megamanGameNS;

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
	level->update(frameTime, input, this);
}

void MegamanGame::ai()
{
	level->ai();
}

void MegamanGame::collisions()
{
	level->collisions();
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
	backdropTexture.onLostDevice();         // backdrop texture
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void MegamanGame::resetAll()
{
	backdropTexture.onResetDevice();
    return;
}
