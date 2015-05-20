#include "bee.h"

//=============================================================================
// default constructor
//=============================================================================
Bee::Bee() : Entity()
{
	spriteData.width = beeNS::WIDTH;           // size of the bee
	spriteData.height = beeNS::HEIGHT;
	spriteData.x = beeNS::X;                   // location on screen
	spriteData.y = beeNS::Y;
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

	dx = 0;
	dy = 0;
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
	

	//spriteData.x += frameTime * 50;         // move along X 
	if (spriteData.direction == RIGHT)
	{
		dx += frameTime * beeNS::SPEED;
	}
	else
	{
		dx -= frameTime * 150;
	}

	beeFlying.update(frameTime);
}

void Bee::stop(double wallX, double wallWidth)
{
	if (spriteData.direction == RIGHT)
	{
		spriteData.direction = LEFT;
		flipHorizontal(true);
		dx -= spriteData.x + spriteData.width - wallX +5;
	}
	else
	{
		spriteData.direction = RIGHT;
		flipHorizontal(false);
		dx += wallX + wallWidth - spriteData.x + 5;
	}
}

void Bee::draw()
{
	beeFlying.draw(spriteData);
}