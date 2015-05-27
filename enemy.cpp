#include "enemy.h"

//=============================================================================
// default constructor
//=============================================================================
Enemy::Enemy() : Entity()
{
	spriteData.width = enemyNS::WIDTH;          // size of enemy
	spriteData.height = enemyNS::HEIGHT;
	spriteData.x = enemyNS::X;                  // location on screen
	spriteData.y = enemyNS::Y;
	spriteData.rect.bottom = enemyNS::HEIGHT;   // rectangle to select parts of an image
	spriteData.rect.right = enemyNS::WIDTH;
	spriteData.direction = RIGHT;				  // enemy always faces right at the start of any level
	spriteData.state = STANDING;					
	velocity.x = 0;						        // velocity X
	velocity.y = 0;							    // velocity Y
	frameDelay = 1.0;// 0.07;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	edge.top = -enemyNS::HEIGHT / 2;              // set collision edges
	edge.bottom = enemyNS::HEIGHT / 2;
	edge.left = -enemyNS::WIDTH / 2;
	edge.right = enemyNS::WIDTH / 2;
	collisionType = entityNS::ROTATED_BOX;
	mass = enemyNS::MASS;
	visible = true;
	active = true;
	dx = 0;
	dy = 0;
}

//=============================================================================
// Initialize Enemy.
// Post: returns true if successful, false if failed
//=============================================================================
bool Enemy::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	// enemy sprite initialize
//	if (!mechaSonicTexture.initialize(graphics, ENEMY001))
//		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	mechaSonicSpriteCoordinates.populateVector("sonicsprites.txt");
	if (!initializeCoords(mechaSonicSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));


	//Idle
	if (!enemyIdle.initialize(mechaSonicSpriteCoordinates))
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
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Enemy::update(float frameTime)
{
	Entity::update(frameTime);
	dy += frameTime * velocity.y;
	enemyIdle.update(frameTime);
}

//=============================================================================
// stop
// Collision detection between Enemy and solid surfaces
//=============================================================================
//void Enemy::stop(int wallX, int wallY, int wallLength, int wallHeight)
//{
//	// Case: Above surface
//	if (((spriteData.x + spriteData.width > wallX && spriteData.x < wallX + wallLength) && spriteData.y + spriteData.height <= wallY + 10) && velocity.y >= 0)
//	{
//		standingOnSurface_ = true;
//		spriteData.y = wallY - spriteData.height;		 // position at the top of the wall
//		velocity.y = 0; 										 // stop y acceleration
//		floorCollision_ = true;
//	}
//	// Case: Below surface
//	else if ((spriteData.x + spriteData.width > wallX && spriteData.x < wallX + wallLength) && spriteData.y >= wallY + wallHeight - 10)
//	{
//		spriteData.y = wallY + wallHeight;
//		velocity.y = 1;
//		standingOnSurface_ = false;
//		floorCollision_ = false;
//	}
//	// Case: Left of surface
//	else if (spriteData.x + spriteData.width >= wallX && spriteData.x + spriteData.width < wallX + wallLength)
//	{
//		spriteData.x = wallX - spriteData.width;
//		if (velocity.y > 0)
//		{
//			canWallJump_ = true;
//		}
//	}
//	// Case: Right of surface
//	else if (spriteData.x < wallX + wallLength && spriteData.x + spriteData.width > wallX)
//	{
//		spriteData.x = wallX + wallLength;
//		if (velocity.y > 0)
//		{
//			canWallJump_ = true;
//		}
//	}
//}

//=============================================================================
// draw
// Drawing all of Enemy's different states
//=============================================================================
void Enemy::draw()
{
	enemyIdle.draw(spriteData);
}

