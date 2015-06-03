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
	health = 200;
	active = false;
	visible = false;
	boss = true;
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

	if (spriteData.state != DEAD)
	{
		gravity(frameTime);
		if (spriteData.direction == RIGHT)
		{
			//dx += frameTime * bowserNS::SPEED;
			flipHorizontal(false);
		}
		else
		{
			//dx -= frameTime * bowserNS::SPEED;
			flipHorizontal(true);
		}
		bowserIdle.update(frameTime);
	}
	else
	{
		if (bowserDying.getCurrentFrame() == bowserNS::DYING_END_FRAME)
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

void Bowser::draw()
{
	if (spriteData.state == DEAD)
	{
		bowserDying.draw(spriteData);
	}
	else
	{
		bowserIdle.draw(spriteData);
	}
}