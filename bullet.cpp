#include "bullet.h"

//=============================================================================bullet
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
	collisionType = entityNS::BOX;
	mass = bulletNS::MASS;
}

bool Bullet::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	//No Charge
	regularBullet.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	regularBullet.setCurrentFrame(bulletNS::NO_CHARGE_START_FRAME);

	//Small Charge
	bulletSmall.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	bulletSmall.setCurrentFrame(bulletNS::SMALL_CHARGE_START_FRAME);

	//Large Charge
	bulletLarge.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	bulletLarge.setCurrentFrame(bulletNS::LARGE_CHARGE_START_FRAME);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}


//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bullet::update(float frameTime)
{
	Entity::update(frameTime);

	regularBullet.update(frameTime);
	bulletSmall.update(frameTime);
	bulletLarge.update(frameTime);
}

void Bullet::draw()
{
	if (spriteData.shotType == REGULAR_SHOT)
	{
		regularBullet.draw(spriteData);
	}
	else if (spriteData.shotType == SMALL_CHARGE)
	{
		RECT rect = { 26, 0, 85, 58 };
		bulletSmall.setSpriteDataRect(rect);
		bulletSmall.draw(spriteData);
	}
	else if (spriteData.shotType == MEDIUM_CHARGE)
	{
		RECT rect = { 85, 0, 217, 65 };
		bulletLarge.setSpriteDataRect(rect);
		bulletLarge.draw(spriteData);
	}
}
