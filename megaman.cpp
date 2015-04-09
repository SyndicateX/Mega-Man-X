
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
	spriteData.direction = RIGHT;				  // megaman always faces right at the start of any level
	spriteData.state = STANDING;					
	velocity.x = 0;						        // velocity X
	velocity.y = 0;							    // velocity Y
	frameDelay = 1.0;// 0.07;
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
// Initialize Mario.
// Post: returns true if successful, false if failed
//=============================================================================
bool Megaman::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	// megaman sprite initialize
	megamanSpriteCoordinates.populateVector("xcoords.txt");
	if (!initializeCoords(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	
	//Idle
	megamanIdle.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanIdle.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanIdle.setFrames(megamanNS::IDLE_MEGAMAN_START_FRAME, megamanNS::IDLE_MEGAMAN_END_FRAME);
	megamanIdle.setCurrentFrame(megamanNS::IDLE_MEGAMAN_START_FRAME);
	megamanIdle.setFrameDelay(megamanNS::IDLE_MEGAMAN_ANIMATION_DELAY);

	//Walking
	megamanWalking.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,				
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanWalking.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanWalking.setFrames(megamanNS::WALKING_MEGAMAN_START_FRAME, megamanNS::WALKING_MEGAMAN_END_FRAME);
	megamanWalking.setCurrentFrame(megamanNS::WALKING_MEGAMAN_START_FRAME);
	megamanWalking.setFrameDelay(megamanNS::WALKING_MEGAMAN_ANIMATION_DELAY);
	
	//Jumping Up
	megamanJumping.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanJumping.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanJumping.setFrames(megamanNS::JUMPING_MEGAMAN_START_FRAME, megamanNS::JUMPING_MEGAMAN_END_FRAME);
	megamanJumping.setCurrentFrame(megamanNS::JUMPING_MEGAMAN_START_FRAME);
	megamanJumping.setFrameDelay(megamanNS::JUMPING_MEGAMAN_ANIMATION_DELAY);
	
	//Jump Peak
	megamanJumpPeak.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanJumpPeak.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanJumpPeak.setFrames(megamanNS::JUMP_PEAK_MEGAMAN_START_FRAME, megamanNS::JUMP_PEAK_MEGAMAN_END_FRAME);
	megamanJumpPeak.setCurrentFrame(megamanNS::JUMP_PEAK_MEGAMAN_START_FRAME);
	megamanJumpPeak.setFrameDelay(megamanNS::JUMP_PEAK_MEGAMAN_ANIMATION_DELAY);
	
	//Falling
	megamanFalling.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanFalling.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanFalling.setFrames(megamanNS::FALLING_MEGAMAN_START_FRAME, megamanNS::FALLING_MEGAMAN_END_FRAME);
	megamanFalling.setCurrentFrame(megamanNS::FALLING_MEGAMAN_START_FRAME);
	megamanFalling.setFrameDelay(megamanNS::FALLING_MEGAMAN_ANIMATION_DELAY);

	//Shooting - Idle
	megamanShooting.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanShooting.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanShooting.setFrames(megamanNS::SHOOTING_MEGAMAN_START_FRAME, megamanNS::SHOOTING_MEGAMAN_END_FRAME);
	megamanShooting.setCurrentFrame(megamanNS::SHOOTING_MEGAMAN_START_FRAME);
	megamanShooting.setFrameDelay(megamanNS::SHOOTING_MEGAMAN_ANIMATION_DELAY);

	//Shooting & Jumping
	megamanShootingJump.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanShootingJump.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanShootingJump.setFrames(megamanNS::SHOOTING_JUMP_MEGAMAN_START_FRAME, megamanNS::SHOOTING_JUMP_MEGAMAN_END_FRAME);
	megamanShootingJump.setCurrentFrame(megamanNS::SHOOTING_JUMP_MEGAMAN_START_FRAME);
	megamanShootingJump.setFrameDelay(megamanNS::SHOOTING_JUMP_MEGAMAN_ANIMATION_DELAY);

	//Shooting & Jump Peak
	megamanShootingJumpPeak.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanShootingJumpPeak.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanShootingJumpPeak.setFrames(megamanNS::SHOOTING_JUMP_PEAK_MEGAMAN_START_FRAME, megamanNS::SHOOTING_JUMP_PEAK_MEGAMAN_END_FRAME);
	megamanShootingJumpPeak.setCurrentFrame(megamanNS::SHOOTING_JUMP_PEAK_MEGAMAN_START_FRAME);
	megamanShootingJumpPeak.setFrameDelay(megamanNS::SHOOTING_JUMP_PEAK_MEGAMAN_ANIMATION_DELAY);

	//Shooting & Falling
	megamanShootingFalling.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanShootingFalling.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanShootingFalling.setFrames(megamanNS::SHOOTING_FALLING_MEGAMAN_START_FRAME, megamanNS::SHOOTING_FALLING_MEGAMAN_END_FRAME);
	megamanShootingFalling.setCurrentFrame(megamanNS::SHOOTING_FALLING_MEGAMAN_START_FRAME);
	megamanShootingFalling.setFrameDelay(megamanNS::SHOOTING_FALLING_MEGAMAN_ANIMATION_DELAY);

	megamanDashing.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanDashing.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanDashing.setFrames(megamanNS::DASHING_MEGAMAN_START_FRAME, megamanNS::DASHING_MEGAMAN_END_FRAME);
	megamanDashing.setCurrentFrame(megamanNS::DASHING_MEGAMAN_START_FRAME);
	megamanDashing.setFrameDelay(megamanNS::DASHING_MEGAMAN_ANIMATION_DELAY);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}
//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Megaman::update(float frameTime)
{
	Entity::update(frameTime);

	if (spriteData.direction == LEFT)
	{
		spriteData.flipHorizontal = true;
		megamanWalking.update(frameTime);
	}
	else
	{
		spriteData.flipHorizontal = false;
		megamanWalking.update(frameTime);
	}

	if (spriteData.state == JUMPING && standingOnSurface_) // && !floorCollision_)//spriteData.y + spriteData.height == GAME_HEIGHT)
	{
		velocity.y = -230;								// Determines the height of megaman's jump -- needs adjusting
		spriteData.y += frameTime * velocity.y;         // move along Y
		standingOnSurface_ = false;
	}
	//else if (input->isUpKeyUp() && wallJump_ && !standingOnSurface_)
	//{
	//	velocity.y = -230;								// Determines the height of megaman's jump -- needs adjusting
	//	spriteData.y += frameTime * velocity.y;         // move along Y
	//	standingOnSurface_ = false;
	//}
	else if (spriteData.state != JUMPING && velocity.y < 0)	// Upward velocity set to 0 if the user releases the UP arrow
	{														// This enables a variable jump height
		velocity.y = 0;										
	}														


	if (spriteData.y + spriteData.height == GAME_HEIGHT) //Bottom edge counts as a floor right now
	{
		floorCollision_ = true;
	}

	// Stay within boundaries
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

	// *********************************************************************************
	// **************************    MEGAMAN IN FREE-FALL    ***************************
	// ********************    CODE COULD/SHOULD BE IMPROVED    ************************
	//In case megaman is mid-jump									//In case megaman steps off a surface
	if (spriteData.y + spriteData.height < GAME_HEIGHT && !standingOnSurface_ || !floorCollision_ && velocity.y >= 0)	//If in the air -- fall
	{
		spriteData.y += frameTime * velocity.y * 3;     // Determines speed and height of megaman's jump -- needs adjusting
		velocity.y += frameTime * GRAVITY;              // gravity
		spriteData.state = JUMPING;
	}

	if (spriteData.state == STANDING && standingOnSurface_)
	{
		megamanIdle.update(frameTime);
	}

	if (spriteData.state == DASHING && standingOnSurface_)
	{
		megamanDashing.update(frameTime);
	}

	if (velocity.y > 0)								////
	{
		input->setCanJump(false);					//// This prevents megaman from jumping multiple times
	}
	if (input->isUpKeyUp() && standingOnSurface_)	//// when the user holds down the UP arrow key
	{
		input->setCanJump(true);					////
	}

	floorCollision_ = false;
	wallJump_ = false;
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
	else if ((spriteData.x + spriteData.width > wallX && spriteData.x < wallX + wallLength) && spriteData.y >= wallY + wallHeight - 10)
	{
		spriteData.y = wallY + wallHeight;
		velocity.y = 1;
		standingOnSurface_ = false;
		floorCollision_ = false;
	}
	else if (spriteData.x + spriteData.width >= wallX && spriteData.x + spriteData.width < wallX + wallLength)
	{
		spriteData.x = wallX - spriteData.width;
		wallJump_ = true;
	}
	else if (spriteData.x < wallX + wallLength && spriteData.x + spriteData.width > wallX)
	{
		spriteData.x = wallX + wallLength;
		wallJump_ = true;
	}
}

void Megaman::draw()
{
	if (spriteData.state == WALKING)
	{
		megamanWalking.draw(spriteData);
	}
	else if (spriteData.state == JUMPING)
	{
		//if (spriteData.state2 != SHOOTING)
		if (spriteData.shotState == NONE)
		{
			if (velocity.y < -40)
				megamanJumping.draw(spriteData);
			else if (velocity.y < 40)
				megamanJumpPeak.draw(spriteData);
			else
				megamanFalling.draw(spriteData);
		}
		else
		{
			if (velocity.y < -40)
				megamanShootingJump.draw(spriteData);
			else if (velocity.y < 40)
				megamanShootingJumpPeak.draw(spriteData);
			else
				megamanShootingFalling.draw(spriteData);
		}
	}
	//else if (spriteData.state2 == SHOOTING && spriteData.state == STANDING)
	else if (spriteData.shotState == SHOOT)// && spriteData.state == STANDING)
	{
		megamanShooting.draw(spriteData);
	}
	else if (spriteData.state == DASHING)
	{
		megamanDashing.draw(spriteData);
	}
	else if (spriteData.state == STANDING)
	{
		megamanIdle.draw(spriteData);
	}
	else
	{
		Image::draw();
	}

}

