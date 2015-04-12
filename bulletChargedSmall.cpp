#include "bulletChargedSmall.h"

//=============================================================================
// default constructor
//=============================================================================
BulletChargedSmall::BulletChargedSmall() : Entity()
{
	spriteData.width = bulletChargedSmallNS::WIDTH;           // size of Ship1
	spriteData.height = bulletChargedSmallNS::HEIGHT;
	spriteData.x = bulletChargedSmallNS::X;                   // location on screen
	spriteData.y = bulletChargedSmallNS::Y;
	spriteData.rect.bottom = bulletChargedSmallNS::HEIGHT;    // rectangle to select parts of an image
	spriteData.rect.right = bulletChargedSmallNS::WIDTH;
	velocity.x = bulletChargedSmallNS::SPEED;                 // velocity X
	velocity.y = bulletChargedSmallNS::SPEED;                 // velocity Y
	frameDelay = 1;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	radius = bulletChargedSmallNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	mass = bulletChargedSmallNS::MASS;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void BulletChargedSmall::update(float frameTime)
{
	Entity::update(frameTime);

	// Bounce off walls
	if (spriteData.x > GAME_WIDTH - bulletChargedSmallNS::WIDTH + 50)  // if hit right screen edge + 50
	{
		//destroy
	}
	else if (spriteData.x < 0 - 50)                    // else if hit left screen edge - 50
	{
		//destroy
	}
}
