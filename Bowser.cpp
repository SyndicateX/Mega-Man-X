#include "bowser.h"

//=============================================================================
// default constructor
//=============================================================================
Bowser::Bowser() : Enemy()
{
	spriteData.width = bowserNS::WIDTH;           // size of the bowser
	spriteData.height = bowserNS::HEIGHT;
	spriteData.x = startX_;                   // location on screen
	spriteData.y = startY_;
	spriteData.rect.bottom = bowserNS::HEIGHT;    // rectangle to select parts of an image
	spriteData.rect.right = bowserNS::WIDTH;
	spriteData.direction = LEFT;
	velocity.x = bowserNS::SPEED;                 // velocity X
	velocity.y = bowserNS::SPEED;                 // velocity Y
	edge.top = -bowserNS::HEIGHT / 2;			// set collision edges
	edge.bottom = bowserNS::HEIGHT / 2;
	edge.left = -bowserNS::WIDTH / 3;
	edge.right = bowserNS::WIDTH / 3;
	collisionType = entityNS::BOX;
	health = 100;
	active = false;
	visible = false;
	boss = true;
	flicker = 0;
}

bool Bowser::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	attackTimer = 1.0f;

	// Bowser sprite initialize
	bowserSpriteCoordinates.populateVector("pictures\\bowser.xml");
	if (!initializeCoords(bowserSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser"));

	//Idle
	bowserIdle.initialize(gamePtr->getGraphics(), bowserNS::WIDTH,
		bowserNS::HEIGHT, 0, textureM);
	if (!bowserIdle.initialize(bowserSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser"));
	bowserIdle.setFrames(bowserNS::IDLE_START_FRAME, bowserNS::IDLE_END_FRAME);
	bowserIdle.setCurrentFrame(bowserNS::IDLE_START_FRAME);
	bowserIdle.setFrameDelay(bowserNS::IDLE_ANIMATION_DELAY);

	//Spin Attack
	bowserSpin.initialize(gamePtr->getGraphics(), bowserNS::WIDTH,
		bowserNS::HEIGHT, 0, textureM);
	if (!bowserSpin.initialize(bowserSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser"));
	bowserSpin.setFrames(bowserNS::SPIN_START_FRAME, bowserNS::SPIN_END_FRAME);
	bowserSpin.setCurrentFrame(bowserNS::SPIN_START_FRAME);
	bowserSpin.setFrameDelay(bowserNS::SPIN_ANIMATION_DELAY);

	// Fire Ball Attack
	bowserFireBreath.initialize(gamePtr->getGraphics(), bowserNS::WIDTH,
		bowserNS::HEIGHT, 0, textureM);
	if (!bowserFireBreath.initialize(bowserSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser"));
	bowserFireBreath.setFrames(bowserNS::FIRE_BREATH_START_FRAME, bowserNS::FIRE_BREATH_END_FRAME);
	bowserFireBreath.setCurrentFrame(bowserNS::FIRE_BREATH_START_FRAME);
	bowserFireBreath.setFrameDelay(bowserNS::FIRE_BREATH_ANIMATION_DELAY);

	//Dying
	bowserDying.initialize(gamePtr->getGraphics(), bowserNS::WIDTH,
		bowserNS::HEIGHT, 0, textureM);
	if (!bowserDying.initialize(bowserSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser"));
	bowserDying.setFrames(bowserNS::DYING_START_FRAME, bowserNS::DYING_END_FRAME);
	bowserDying.setCurrentFrame(bowserNS::DYING_START_FRAME);
	bowserDying.setFrameDelay(bowserNS::DYING_ANIMATION_DELAY);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bowser::update(float frameTime)
{
	Entity::update(frameTime);

	//leftOfWall_ = false;
	//rightOfWall_ = false;

	if (spriteData.state == ATTACKING)
	{
		spinAttack(frameTime);
	}

	if (spriteData.state != DEAD)
	{
		gravity(frameTime);
		if (spriteData.direction == RIGHT)
		{
			flipHorizontal(false);
		}
		else
		{
			flipHorizontal(true);
		}
		bowserIdle.update(frameTime);
	}
	else if (spriteData.state == FIRE_BREATH)
	{
		bowserFireBreath.update(frameTime);
		if (bowserFireBreath.getAnimationComplete())
		{
			spriteData.state = STANDING;
			bowserIdle.update(frameTime);
		}
	}
	else
	{
		if (bowserDying.getAnimationComplete())
		{
			visible = false;
		}
		bowserDying.update(frameTime);
	}
}

void Bowser::handleCollisions(double wallX, double wallY, double wallWidth, double wallHeight)
{
	stop(wallX, wallY, wallWidth, wallHeight);
}

void Bowser::spinAttack(float frameTime)
{
	//figure out what kind of attack he is doing
	//...

	//for now spinAttack
	edge.top = -bowserNS::HEIGHT / 4;
	if (rightOfWall_)
	{
 		spriteData.direction = RIGHT;
	}
	else if (leftOfWall_)
	{
		spriteData.direction = LEFT;
	}

	if (spriteData.direction == RIGHT)
	{
		dx += frameTime * bowserNS::SPEED;
	}
	else
	{
		dx -= frameTime * bowserNS::SPEED;
	}


	bowserSpin.update(frameTime);
}
void Bowser::draw()
{
	if (isInvincible_)
	{
		flicker++;
	}
	if (!isInvincible_ || (isInvincible_ && flicker % 2 == 0))
	{
		if (spriteData.state == ATTACKING)
		{
			bowserSpin.draw(spriteData);
		}
		else if (spriteData.state == DEAD)
		{
			bowserDying.draw(spriteData);
		}
		else if (spriteData.state == FIRE_BREATH)
		{
			//bowserFireBreath.draw(spriteData);
			edge.top = -bowserNS::HEIGHT / 2;
			bowserIdle.draw(spriteData);
		}
		else
		{
			edge.top = -bowserNS::HEIGHT / 2;
			bowserIdle.draw(spriteData);
		}
	}
}
