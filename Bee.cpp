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
	collisionType = entityNS::BOX;
	mass = beeNS::MASS;

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
		dx += frameTime * 150;
	}
	else
	{
		dx -= frameTime * 150;
	}
	
	//spriteData.x += frameTime * velocity.x;         // move along X 
	//spriteData.y += frameTime * velocity.y;         // move along Y

	//// Bounce off walls
	//if (spriteData.x > GAME_WIDTH - beeNS::WIDTH)  // if hit right screen edge
	//{
	//	spriteData.x = GAME_WIDTH - beeNS::WIDTH;  // position at right screen edge
	//	velocity.x = -velocity.x;                   // reverse X direction
	//}
	//else if (spriteData.x < 0)                    // else if hit left screen edge
	//{
	//	spriteData.x = 0;                           // position at left screen edge
	//	velocity.x = -velocity.x;                   // reverse X direction
	//}
	//if (spriteData.y > GAME_HEIGHT - beeNS::HEIGHT) // if hit bottom screen edge
	//{
	//	spriteData.y = GAME_HEIGHT - beeNS::HEIGHT; // position at bottom screen edge
	//	velocity.y -= beeNS::ENERGY_LOSS;
	//	if (velocity.y < beeNS::MIN_VY)            // if bee has small bounce
	//	{
	//		spriteData.y = GAME_HEIGHT / 4;
	//		velocity.x = beeNS::SPEED;
	//	}
	//	velocity.y = -velocity.y;                   // reverse Y direction
	//}
	//else if (spriteData.y < 0)                    // else if hit top screen edge
	//{
	//	spriteData.y = 0;                           // position at top screen edge
	//	velocity.y = -velocity.y;                   // reverse Y direction
	//}

	//velocity.y += frameTime * GRAVITY;              // gravity

	beeFlying.update(frameTime);
}

void Bee::stop(double wallX, double wallWidth)
{
	if (spriteData.direction == RIGHT)
	{
		spriteData.direction = LEFT;
		flipHorizontal(true);
		dx -= spriteData.x + spriteData.width - wallX;
	}
	else
	{
		spriteData.direction = RIGHT;
		flipHorizontal(false);
		dx += wallX + wallWidth - spriteData.x;
	}
}

void Bee::draw()
{
	beeFlying.draw(spriteData);
}