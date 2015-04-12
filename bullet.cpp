#include "bullet.h"

//=============================================================================
// default constructor
//=============================================================================
Bullet::Bullet() : Entity()
{
	spriteData.width = bulletNS::WIDTH;           // size of Ship1
	spriteData.height = bulletNS::HEIGHT;
	spriteData.x = bulletNS::X;                   // location on screen
	spriteData.y = bulletNS::Y;
	spriteData.rect.bottom = bulletNS::HEIGHT;    // rectangle to select parts of an image
	spriteData.rect.right = bulletNS::WIDTH;
	velocity.x = bulletNS::SPEED;                 // velocity X
	velocity.y = bulletNS::SPEED;                 // velocity Y
	frameDelay = 1;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	radius = bulletNS::WIDTH / 2.0;
	collisionType = entityNS::CIRCLE;
	mass = bulletNS::MASS;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bullet::update(float frameTime)
{
	Entity::update(frameTime);

	// Bounce off walls
	if (spriteData.x > GAME_WIDTH - bulletNS::WIDTH + 50)  // if hit right screen edge + 50
	{
		//destroy
	}
	else if (spriteData.x < 0 - 50)                    // else if hit left screen edge - 50
	{
		//destroy
	}
}
