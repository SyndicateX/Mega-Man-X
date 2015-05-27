#include "MechaSonic.h"


MechaSonic::MechaSonic()
{
	//gravity_ = true;
}


MechaSonic::~MechaSonic()
{
}


//=============================================================================
// initialize
// Initialization for MechaSonic
//=============================================================================
bool MechaSonic::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)//, std::string spriteCoordinatesFileName)
{
	// enemy sprite initialize
	//	if (!mechaSonicTexture.initialize(graphics, ENEMY001))
	//		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));	
	
	std::string spriteCoordinatesFileName = "sonicsprites.txt";

	spriteCoordinates.populateVector(spriteCoordinatesFileName);
	if (!initializeCoords(spriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

	//Idle
	if (!enemyIdle.initialize(spriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	//megamanWallSliding.setFrames(megamanNS::WALL_SLIDING_MEGAMAN_START_FRAME, megamanNS::WALL_SLIDING_MEGAMAN_END_FRAME);

	enemyIdle.initialize(gamePtr->getGraphics(), enemyNS::WIDTH,
		enemyNS::HEIGHT, 0, textureM);
	//if (!enemyIdle.initialize(gamePtr->getGraphics(), enemyNS::WIDTH, enemyNS::HEIGHT, 9, textureM))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	enemyIdle.setFrames(enemyNS::IDLE_ENEMY_START_FRAME, enemyNS::IDLE_ENEMY_END_FRAME);
	enemyIdle.setCurrentFrame(enemyNS::IDLE_ENEMY_START_FRAME);
	enemyIdle.setFrameDelay(enemyNS::IDLE_ENEMY_ANIMATION_DELAY);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// draw
// Drawing all of Enemy's different states
//=============================================================================
void Enemy::draw()
{
	enemyIdle.draw(spriteData);
}




