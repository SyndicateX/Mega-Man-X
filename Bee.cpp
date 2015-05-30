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
	frameDelay = 1;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;								// last frame of animation
	currentFrame = startFrame;
	edge.top = -beeNS::HEIGHT / 2;			// set collision edges
	edge.bottom = beeNS::HEIGHT / 2;
	edge.left = -beeNS::WIDTH / 2;
	edge.right = beeNS::WIDTH / 2;
	collisionType = entityNS::BOX;
	mass = beeNS::MASS;
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
		}
		else
		{
			dx -= frameTime * beeNS::SPEED;
		}
		beeFlying.update(frameTime);
	}
	else
	{
		if (beeDying.getCurrentFrame() == beeNS::DYING_BEE_END_FRAME)
		{
			visible = false;
		}
		beeDying.update(frameTime);
	}
}

void Bee::handleCollisions(int wallX, int wallY, int wallLength, int wallHeight)
{
	stop(wallX, wallLength);
}

void Bee::draw()
{
	if (spriteData.state == DEAD)
	{
		beeDying.draw(spriteData);
		audio->playCue(EXPLODE);
	}
	else
	{
		beeFlying.draw(spriteData);
	}
}