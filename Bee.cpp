#include "bee.h"

//=============================================================================
// default constructor
//=============================================================================
Bee::Bee() : Enemy()
{
	spriteData.width = beeNS::WIDTH;           // size of the bee
	spriteData.height = beeNS::HEIGHT;
	spriteData.x = startX_;                   // location on screen
	spriteData.y = startY_;
	spriteData.rect.bottom = beeNS::HEIGHT;    // rectangle to select parts of an image
	spriteData.rect.right = beeNS::WIDTH;
	spriteData.direction = RIGHT;
	velocity.x = beeNS::SPEED;                 // velocity X
	velocity.y = beeNS::SPEED;                 // velocity Y
	edge.top = -beeNS::HEIGHT / 2;			// set collision edges
	edge.bottom = beeNS::HEIGHT / 2;
	edge.left = -beeNS::WIDTH / 3;
	edge.right = beeNS::WIDTH / 3;
	collisionType = entityNS::BOX;
	health = 8;
	active = true;
	visible = true;
}

bool Bee::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	// BEE sprite initialize
	beeSpriteCoordinates.populateVector("pictures\\baddies.xml");
	if (!initializeCoords(beeSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee"));

	//Idle
	beeFlying.initialize(gamePtr->getGraphics(), beeNS::WIDTH,
		beeNS::HEIGHT, 0, textureM);
	if (!beeFlying.initialize(beeSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee"));
	beeFlying.setFrames(beeNS::FLYING_BEE_START_FRAME, beeNS::FLYING_BEE_END_FRAME);
	beeFlying.setCurrentFrame(beeNS::FLYING_BEE_START_FRAME);
	beeFlying.setFrameDelay(beeNS::FLYING_BEE_ANIMATION_DELAY);

	//Dying
	beeDying.initialize(gamePtr->getGraphics(), beeNS::WIDTH,
		beeNS::HEIGHT, 0, textureM);
	if (!beeDying.initialize(beeSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee"));
	beeDying.setFrames(beeNS::DYING_BEE_START_FRAME, beeNS::DYING_BEE_END_FRAME);
	beeDying.setCurrentFrame(beeNS::DYING_BEE_START_FRAME);
	beeDying.setFrameDelay(beeNS::DYING_BEE_ANIMATION_DELAY);
	beeDying.setLoop(false);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Bee::update(float frameTime) 
{
	Entity::update(frameTime);
	
	if (spriteData.state != DEAD)
	{
		if (spriteData.direction == RIGHT)
		{
			dx += frameTime * beeNS::SPEED;
			flipHorizontal(false);
		}
		else
		{
			dx -= frameTime * beeNS::SPEED;
		}
		beeFlying.update(frameTime);
	}
	else
	{
		if (beeDying.getAnimationComplete())
		{
			visible = false;
			flipHorizontal(true);
		}
		beeDying.update(frameTime);
	}
}

void Bee::handleCollisions(double wallX, double wallY, double wallWidth, double wallHeight)
{
	stop(wallX, wallWidth);
}

void Bee::draw()
{
	if (spriteData.state == DEAD)
	{
		beeDying.draw(spriteData);
	}
	else
	{
		beeFlying.draw(spriteData);
	}
}