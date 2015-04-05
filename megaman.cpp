
#include "megaman.h"

//=============================================================================
// default constructor
//=============================================================================
Megaman::Megaman() : Entity()
{
	spriteData.width = megamanNS::WIDTH;          // size of Ship1
	spriteData.height = megamanNS::HEIGHT;
	spriteData.x = megamanNS::X;                  // location on screen
	spriteData.y = megamanNS::Y;
	spriteData.rect.bottom = megamanNS::HEIGHT;   // rectangle to select parts of an image
	spriteData.rect.right = megamanNS::WIDTH;
	velocity.x = 0;						        // velocity X
	velocity.y = 0;							    // velocity Y
	frameDelay = 0.07;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	edge.top = -megamanNS::HEIGHT / 2;              // set collision edges
	edge.bottom = megamanNS::HEIGHT / 2;
	edge.left = -megamanNS::WIDTH / 2;
	edge.right = megamanNS::WIDTH / 2;
	collisionType = entityNS::ROTATED_BOX;
	mass = megamanNS::MASS;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Megaman::update(float frameTime, State state, Direction direction)
{
	Entity::update(frameTime);

	if (direction == LEFT)
	{
		spriteData.flipHorizontal = true;
	}
	else
	{
		spriteData.flipHorizontal = false;
	}

	if (state == CROUCHING)
	{
		edge.top = (-megamanNS::HEIGHT / 2) / 3;            // set collision edges for crouching megaman
	}
	else
	{
		edge.top = -megamanNS::HEIGHT / 2;				// reset collision edges
	}

	if (state == JUMPING && standingOnSurface_)//spriteData.y + spriteData.height == GAME_HEIGHT)
	{
		velocity.y = -230;								// Determines the height of megaman's jump -- needs adjusting
		spriteData.y += frameTime * velocity.y;         // move along Y
		standingOnSurface_ = false;
	}

	if (spriteData.y + spriteData.height == GAME_HEIGHT) //Bottom edge counts as a floor right now
	{
		floorCollision_ = true;
	}

	// Bounce off walls
	if (spriteData.x > GAME_WIDTH - megamanNS::WIDTH)		// if hit right screen edge
		spriteData.x = GAME_WIDTH - megamanNS::WIDTH;		// position at right screen edge
	else if (spriteData.x < 0)							// else if hit left screen edge
		spriteData.x = 0;								// position at left screen edge
	if (spriteData.y + spriteData.height >= GAME_HEIGHT) //else if at the bottom edge
	{
		spriteData.y = GAME_HEIGHT - spriteData.height;	 // position at the bottom edge
		velocity.y = 0;									 // stop y acceleration
		standingOnSurface_ = true;
	}

	// **************************    MEGAMAN IN FREE-FALL    ***************************
	// ********************    CODE COULD/SHOULD BE IMPROVED    **********************
	//In case megaman is mid-jump									//In case megaman steps off a surface
	if (spriteData.y + spriteData.height < GAME_HEIGHT && !standingOnSurface_ || !floorCollision_ && velocity.y >= 0)	//If in the air -- fall
	{
		spriteData.y += frameTime * velocity.y * 3;     // Determines speed and height of megaman's jump -- needs adjusting
		velocity.y += frameTime * GRAVITY;              // gravity
		setCurrentFrame(27);
		//setFrames(5, 5);
	}

	floorCollision_ = false;
}

void Megaman::stop(int wallX, int wallY, int wallLength, int wallHeight)
{
	if ((spriteData.x + spriteData.width > wallX && spriteData.x < wallX + wallLength) && spriteData.y + spriteData.height <= wallY + 10)
	{
		standingOnSurface_ = true;
		spriteData.y = wallY - spriteData.height;		 // position at the top of the wall
		velocity.y = 0;									 // stop y acceleration
		floorCollision_ = true;
	}
	else if ((spriteData.x + spriteData.width > wallX && spriteData.x < wallX + wallLength) && spriteData.y >= wallY - 10)
	{
		spriteData.y = wallY + wallHeight;
		velocity.y = 1;
		standingOnSurface_ = false;
		floorCollision_ = false;
	}
	else if (spriteData.x + spriteData.width >= wallX && spriteData.x + spriteData.width < wallX + wallLength)
	{
		spriteData.x = wallX - spriteData.width;
	}
	else if (spriteData.x < wallX + wallLength && spriteData.x + spriteData.width > wallX)
	{
		spriteData.x = wallX + wallLength;
	}
}

