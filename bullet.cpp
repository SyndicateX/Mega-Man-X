#include "bullet.h"

//=============================================================================bullet
// default constructor
//=============================================================================
Bullet::Bullet() : Entity()
{
	spriteData.width = bulletNS::WIDTH;           // size of bullets
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
	double initialY_;
	active = false;
	visible = false;
}

bool Bullet::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	// bullet sprite initialize
	bulletSpriteCoordinates.populateVector("pictures\\bulletsprites.xml");
	if (!initializeCoords(bulletSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullets"));

	//No Charge
	regularBullet.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	regularBullet.initialize(bulletSpriteCoordinates);
	regularBullet.setFrames(bulletNS::NO_CHARGE_START_FRAME, bulletNS::NO_CHARGE_END_FRAME);
	regularBullet.setCurrentFrame(bulletNS::NO_CHARGE_START_FRAME);
	regularBullet.setFrameDelay(bulletNS::NO_CHARGE_ANIMATION_DELAY);

	//Small Charge
	bulletSmall.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	bulletSmall.initialize(bulletSpriteCoordinates);
	bulletSmall.setFrames(bulletNS::SMALL_CHARGE_START_FRAME, bulletNS::SMALL_CHARGE_END_FRAME);
	bulletSmall.setCurrentFrame(bulletNS::SMALL_CHARGE_START_FRAME);
	bulletSmall.setFrameDelay(bulletNS::SMALL_CHARGE_ANIMATION_DELAY);

	//Large Charge
	bulletLarge.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	bulletLarge.initialize(bulletSpriteCoordinates);
	bulletLarge.setFrames(bulletNS::LARGE_CHARGE_START_FRAME, bulletNS::LARGE_CHARGE_END_FRAME);
	bulletLarge.setCurrentFrame(bulletNS::LARGE_CHARGE_START_FRAME);
	bulletLarge.setFrameDelay(bulletNS::LARGE_CHARGE_ANIMATION_DELAY);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Bullet::setInitialY(double initialY, bool wallSliding, bool dashing)
{
	if (spriteData.shotType == REGULAR_SHOT)
	{
		initialY_ = initialY + 23;
	}
	else if (spriteData.shotType == SMALL_CHARGE)
	{
		initialY_ = initialY + 15;
	}
	else
	{
		initialY_ = initialY;
	}
	if (wallSliding)
	{
		initialY_ += 30;
	}
	if (dashing)
	{
		initialY_ += 30;
	}
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
		audio->stopCue(LOAD);
		audio->playCue(FIRE);
	}
	else if (spriteData.shotType == SMALL_CHARGE)
	{
		bulletSmall.draw(spriteData);
		audio->stopCue(LOAD);
		audio->playCue(FIRE);
	}
	else if (spriteData.shotType == MEDIUM_CHARGE)
	{
		bulletLarge.draw(spriteData);
		audio->stopCue(LOAD);
		audio->playCue(MAXFIRE);
	}
}
