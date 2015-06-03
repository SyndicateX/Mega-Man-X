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
	bulletSpriteCoordinates.populateVector("pictures\\bulletsprites2.xml");
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
	fireball.initialize(gamePtr->getGraphics(), bulletNS::WIDTH,
		bulletNS::HEIGHT, 1, textureM);
	fireball.initialize(bulletSpriteCoordinates);
	fireball.setFrames(bulletNS::FIREBALL_START_FRAME, bulletNS::FIREBALL_END_FRAME);
	fireball.setCurrentFrame(bulletNS::FIREBALL_START_FRAME);
	fireball.setFrameDelay(bulletNS::FIREBALL_ANIMATION_DELAY);

	//Fireball

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Bullet::setInitialY(double initialY, bool wallSliding, bool dashing)
{
	if (spriteData.shotType == REGULAR_SHOT)
	{
		initialY_ = initialY + 23;
		audio->playCue(FIRE);
	}
	else if (spriteData.shotType == SMALL_CHARGE)
	{
		initialY_ = initialY + 15;
		audio->playCue(FIRE);
	}
	else if (spriteData.shotType == MEDIUM_CHARGE)
	{
		initialY_ = initialY;
		audio->playCue(FIRE);
	}
	else if (spriteData.shotType == FIREBALL)
	{
		initialY_ = initialY;
		spriteData.width = 205;           // size of fireballs
		spriteData.height = 120;
		edge.top = -spriteData.height / 2;			// set collision edges
		edge.bottom = spriteData.height / 2;
		edge.left = -spriteData.width / 3;
		edge.right = spriteData.width / 3;
		audio->playCue(FIRE);
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
	fireball.update(frameTime);
}

void Bullet::draw()
{
	if (spriteData.shotType == REGULAR_SHOT)
	{
		regularBullet.draw(spriteData);
		audio->stopCue(LOAD);
		//audio->playCue(FIRE);
	}
	else if (spriteData.shotType == SMALL_CHARGE)
	{
		bulletSmall.draw(spriteData);
		audio->stopCue(LOAD);
		//audio->playCue(FIRE);
	}
	else if (spriteData.shotType == MEDIUM_CHARGE)
	{
		bulletLarge.draw(spriteData);
		audio->stopCue(LOAD);
		//audio->playCue(MAXFIRE);
	}
	else if (spriteData.shotType == FIREBALL)
	{
		fireball.draw(spriteData);
	}
}
