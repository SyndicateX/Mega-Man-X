#include "megaman.h"

//=============================================================================
// default constructor
//=============================================================================
Megaman::Megaman() : Entity()
{
	spriteData.width = megamanNS::WIDTH;          // size of megaman
	spriteData.height = megamanNS::HEIGHT;
	spriteData.x = megamanNS::X;                  // location on screen
	spriteData.y = megamanNS::Y;
	spriteData.rect.bottom = megamanNS::HEIGHT;   // rectangle to select parts of an image
	spriteData.rect.right = megamanNS::WIDTH;
	spriteData.direction = RIGHT;				  // megaman always faces right at the start of any level
	spriteData.state = STANDING;					
	velocity.x = 0;						        // velocity X
	velocity.y = 0;							    // velocity Y
	edge.top = -megamanNS::HEIGHT / 2;			// set collision edges
	edge.bottom = megamanNS::HEIGHT / 2;
	edge.left = -megamanNS::WIDTH / 2;
	edge.right = megamanNS::WIDTH / 2;
	collisionType = entityNS::BOX;
	damageTimer = 0.0f;							// time remaining until user can control Mega Man
	invincibleTimer = 0.0f;
	isInvincible_ = false;
	flicker = 0;
}

//=============================================================================
// Initialize Mega Man.
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

	//Shooting and falling
	megamanShootingFalling.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanShootingFalling.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanShootingFalling.setFrames(megamanNS::SHOOTING_FALLING_MEGAMAN_START_FRAME, megamanNS::SHOOTING_FALLING_MEGAMAN_END_FRAME);
	megamanShootingFalling.setCurrentFrame(megamanNS::SHOOTING_FALLING_MEGAMAN_START_FRAME);
	megamanShootingFalling.setFrameDelay(megamanNS::SHOOTING_FALLING_MEGAMAN_ANIMATION_DELAY);

	//Dashing
	megamanDashing.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanDashing.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanDashing.setFrames(megamanNS::DASHING_MEGAMAN_START_FRAME, megamanNS::DASHING_MEGAMAN_END_FRAME);
	megamanDashing.setCurrentFrame(megamanNS::DASHING_MEGAMAN_START_FRAME);
	megamanDashing.setFrameDelay(megamanNS::DASHING_MEGAMAN_ANIMATION_DELAY);

	////////////Walking and shooting																				// Needs to modify sprite sheet for this to work
	//////////megamanShootingWalking.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,							// Could possible work by modifying currentFrame when updating frames
	//////////	megamanNS::HEIGHT, 0, textureM);
	//////////if (!megamanShootingWalking.initialize(megamanSpriteCoordinates))
	//////////	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	//////////megamanShootingWalking.setFrames(megamanNS::SHOOTING_WALKING_MEGAMAN_START_FRAME, megamanNS::SHOOTING_WALKING_MEGAMAN_END_FRAME);
	//////////megamanShootingWalking.setCurrentFrame(megamanNS::SHOOTING_WALKING_MEGAMAN_START_FRAME);
	//////////megamanShootingWalking.setFrameDelay(megamanNS::SHOOTING_WALKING_MEGAMAN_ANIMATION_DELAY);

	//Dashing and shooting
	megamanShootingDashing.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanShootingDashing.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanShootingDashing.setFrames(megamanNS::SHOOTING_DASHING_MEGAMAN_START_FRAME, megamanNS::SHOOTING_DASHING_MEGAMAN_END_FRAME);
	megamanShootingDashing.setCurrentFrame(megamanNS::SHOOTING_DASHING_MEGAMAN_START_FRAME);
	megamanShootingDashing.setFrameDelay(megamanNS::SHOOTING_DASHING_MEGAMAN_ANIMATION_DELAY);

	//Sliding down a wall
	megamanWallSliding.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanWallSliding.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanWallSliding.setFrames(megamanNS::WALL_SLIDING_MEGAMAN_START_FRAME, megamanNS::WALL_SLIDING_MEGAMAN_END_FRAME);
	megamanWallSliding.setCurrentFrame(megamanNS::WALL_SLIDING_MEGAMAN_START_FRAME);
	megamanWallSliding.setFrameDelay(megamanNS::WALL_SLIDING_MEGAMAN_ANIMATION_DELAY);

	//Taking damage
	megamanDamaged.initialize(gamePtr->getGraphics(), megamanNS::WIDTH,
		megamanNS::HEIGHT, 0, textureM);
	if (!megamanDamaged.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	megamanDamaged.setFrames(megamanNS::DAMAGED_MEGAMAN_START_FRAME, megamanNS::DAMAGED_MEGAMAN_END_FRAME);
	megamanDamaged.setCurrentFrame(megamanNS::DAMAGED_MEGAMAN_START_FRAME);
	megamanDamaged.setFrameDelay(megamanNS::DAMAGED_MEGAMAN_ANIMATION_DELAY);

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

	//----------------------------------------------------------------------
	//--------------------- IF MEGA MAN TAKES DAMAGE -----------------------
	//----------------------------------------------------------------------
	if (spriteData.state == DAMAGED)				
	{
		megamanDamaged.update(frameTime);
		if (spriteData.direction == RIGHT)
		{
			spriteData.x -= megamanNS::SPEED/50;
		}
		else
		{
			spriteData.x += megamanNS::SPEED/50;
		}
		damageTimer -= frameTime;                     // time remaining until damage animation ends
		if (damageTimer < 0)                           // if ready to fire
		{
			velocity.y = 0;
			spriteData.state = WALKING;
			isInvincible_ = true;
			invincibleTimer = INVINCIBILITY_TIME;
		}

		if (spriteData.y + spriteData.height < GAME_HEIGHT && !standingOnSurface_ || !floorCollision_ && velocity.y >= 0)	//If in the air -- fall
		{
			spriteData.y += frameTime * velocity.y * 5;     // Determines speed and height of Mega Man's jump -- can be adjusted
			velocity.y += frameTime * GRAVITY;              // gravity
		}
	}
	//----------------------------------------------------------------------
	//----------------- IF MEGA MAN IS NOT TAKING DAMAGE -------------------
	//----------------------------------------------------------------------
	else
	{
		invincibleTimer -= frameTime;
		if (invincibleTimer < 0)
		{
			isInvincible_ = false;
		}

		static bool wallJumped = true;

		//-------------------------------------------------------------------------
		//---------------------- UPDATE WALKING SPRITES ---------------------------
		//-------------------------------------------------------------------------
		if (spriteData.direction == LEFT)
		{
			spriteData.flipHorizontal = true;
			//if (spriteData.shotType != NONE)
			//{
			//	megamanShootingWalking.update(frameTime);
			//}
			//else
			//{
				megamanWalking.update(frameTime);
			//}
		}
		else
		{
			spriteData.flipHorizontal = false;
			//if (spriteData.shotType != NONE)
			//{
			//	megamanShootingWalking.update(frameTime);
			//}
			//else
			//{
				megamanWalking.update(frameTime);
			//}
		}

		if (canWallJump_)
		{
			spriteData.state = WALL_SLIDING;
		}

		if (spriteData.state == JUMPING && (standingOnSurface_))
		{
			velocity.y = JUMP_VELOCITY;							// Determines the height of megaman's jump -- can be adjusted
			spriteData.y += frameTime * velocity.y;				// move along Y
			standingOnSurface_ = false;
		}
		else if (spriteData.state != JUMPING && velocity.y < 0)	// Upward velocity set to 0 if the user releases the UP arrow
			//else if (!wallJumped && spriteData.state != JUMPING && velocity.y < 0)	
		{														// This enables a variable jump height
			velocity.y = 0;
			wallJumped = false;
		}

		if (spriteData.y + spriteData.height == GAME_HEIGHT) //Bottom edge counts as a floor right now
		{
			floorCollision_ = true;
		}

		// Keep Mega Man within boundaries
		if (spriteData.x > GAME_WIDTH - megamanNS::WIDTH)		// if hit right screen edge
			spriteData.x = GAME_WIDTH - megamanNS::WIDTH;		// position at right screen edge
		else if (spriteData.x < 0)							// else if hit left screen edge
			spriteData.x = 0;								// position at left screen edge
		if (spriteData.y + spriteData.height >= MAP_HEIGHT) //else if at the bottom edge
		{
			spriteData.y = GAME_HEIGHT - spriteData.height;	 // position at the bottom edge
			velocity.y = 0;									 // stop y acceleration
			standingOnSurface_ = true;
			isDashJumping_ = false;
			isDashing_ = false;
		}

		spriteData.x += frameTime * velocity.x * 3;

		// --------------------------------------------------------------------------------
		// --------------------------    MEGAMAN IN FREE-FALL    --------------------------
		// ------------------------   AND WALL JUMPING MECHANICS   ------------------------
		// --------------------------------------------------------------------------------
		//In case Mega Man is mid-jump						//In case megaman steps off a surface
		if (spriteData.y + spriteData.height < GAME_HEIGHT && !standingOnSurface_ || !floorCollision_ && velocity.y >= 0)	//If in the air -- fall
			//if ((!standingOnSurface_ || !floorCollision_) && velocity.y >= 0)
		{
			if (spriteData.state == WALL_SLIDING)
			{
				velocity.y = 45;		//if sliding down wall, fall at constant rate
			}
			spriteData.y += frameTime * velocity.y * 5;     // Determines speed and height of Mega Man's jump -- can be adjusted
			velocity.y += frameTime * GRAVITY;              // gravity
			spriteData.state = JUMPING;
			standingOnSurface_ = false;
			floorCollision_ = false;

			if (doWallJump_) // If Mega Man jumped off a wall
			{
				wallJumped = true;
				velocity.y = JUMP_VELOCITY;
				spriteData.y += frameTime * velocity.y;

				//////////velocity.x = 70;		//Mega Man is forced away from wall after jumping off it
				//////////if (isDashJumping_)
				//////////{
				//////////	velocity.x *= 1.35;
				//////////}
				//////////if (spriteData.direction == RIGHT)
				//////////{
				//////////	velocity.x = -velocity.x;
				//////////}

				canWallJump_ = false;
				doWallJump_ = false;
			}
			canDash_ = false;			// Mega Man cannot dash in the air
			if (velocity.y >= 0)
			{
				canJump_ = false;
			}

		}
		if (velocity.y >= 0)
		{
			wallJumped = false;
		}

		// -----------------------------------------------------------------
		// -------------- JUMPING AND DASHING INPUT MECHANICS --------------
		// -----------------------------------------------------------------
		if ((!(input->isKeyDown(SPACE_KEY)) && !(input->getGamepadB(0))) && standingOnSurface_)	//
		{													// Reset dash ability after user releases dash button
			canDash_ = true;
		}													//
		if (!standingOnSurface_)
		{
			canDash_ = false;
		}

		if (velocity.y > 0)														////
		{																		////
			canJump_ = false;													//// Prevents mega man from jumping multiple times
		}																		//// when the user holds down the jump button
		if ((!(input->isKeyDown(UP_KEY)) && !(input->getGamepadA(0))) && (standingOnSurface_ || canWallJump_))	/// 	
		{																		////
			canJump_ = true;													////
		}

		// ---------------------------------------------------------
		// -------------------- UPDATE DRAWINGS --------------------
		// ---------------------------------------------------------
		if (spriteData.state == STANDING && standingOnSurface_)
		{
			megamanIdle.update(frameTime);
		}

		if (spriteData.state == DASHING)
		{
			megamanDashing.update(frameTime);
		}

		if (canWallJump_ && !standingOnSurface_)
		{
			spriteData.state = WALL_SLIDING;
			megamanWallSliding.update(frameTime);
			isDashJumping_ = false;
			isDashing_ = false;
		}

		if (velocity.y > TERMINAL_VELOCITY)
		{
			velocity.y = TERMINAL_VELOCITY;
		}

		floorCollision_ = false;
		canWallJump_ = false;
	}
}

//=============================================================================
// stop
// Collision detection between Mega Man and solid surfaces
//=============================================================================
void Megaman::stop(std::vector<VECTOR2> collisionVector, std::vector<RECT> tileCoordinates)
{
	if (collisionVector.size() == 1)
	{
		stop(tileCoordinates[0].left, tileCoordinates[0].top, tileCoordinates[0].right - tileCoordinates[0].left, tileCoordinates[0].bottom - tileCoordinates[0].top);
	}
	else
	{
		while (collisionVector.empty() == false)
		{
			VECTOR2 tempCV = collisionVector[0];
			collisionVector.erase(collisionVector.begin());

			RECT tempTC = tileCoordinates[0];
			tileCoordinates.erase(tileCoordinates.begin());

			bool blocksInLine = false;
			bool onTop = false;
			bool onBottom = false;
			bool onLeft = false;
			bool onRight = false;

			for (int i = 0; i < collisionVector.size(); i++)
			{
 				if (!collisionVector.empty() && tempTC.top == tileCoordinates[i].top)
				{
					collisionVector.erase(collisionVector.begin() + i);
					tileCoordinates.erase(tileCoordinates.begin() + i);
					i--;
					blocksInLine = true;
				}
			}
			// If blocks lined up on the same y-coordinate (top y)
			if (blocksInLine) 
			{
				//top/bottom collision
				if (tempCV.y > 0)
				{
					topCollision(tempTC.top);
				}
				else
				{
					bottomCollision(tempTC.top, tempTC.bottom - tempTC.top);
				}
			}
			else
			{
				for (int i = 0; i < collisionVector.size(); i++)
				{
					if (!collisionVector.empty() && tempTC.left == tileCoordinates[i].left)
					{
						collisionVector.erase(collisionVector.begin() + i);
						tileCoordinates.erase(tileCoordinates.begin() + i);
						i--;
						blocksInLine = true;
					}
				}

				// If blocks lined up on the same x-coordinate (left x)
				if (blocksInLine)
				{
					//left/right collision
					if (tempCV.x > 0)
					{
						leftCollision(tempTC.left);
					}
					else
					{
						rightCollision(tempTC.left, tempTC.right - tempTC.left);
					}
				}
				else
				{
					stop(tempTC.left, tempTC.top, tempTC.right - tempTC.left, tempTC.bottom - tempTC.top);
				}
			}
			if (collisionVector.size() == 1 || collisionVector.empty())
			{
				if (!collisionVector.empty())
				{
					stop(tileCoordinates[0].left, tileCoordinates[0].top, tileCoordinates[0].right - tileCoordinates[0].left, tileCoordinates[0].bottom - tileCoordinates[0].top);
					collisionVector.erase(collisionVector.begin());
					tileCoordinates.erase(tileCoordinates.begin());
				}
			}
		}
	}
}

void Megaman::stop(double wallX, double wallY, double wallWidth, double wallHeight)
{
	// Case: Below surface
	if ((spriteData.x + spriteData.width > wallX) && (spriteData.x < wallX + wallWidth) && spriteData.y >= wallY + wallHeight - 10)
	{
		bottomCollision(wallY, wallHeight);
	}
	// Case: Above surface
	else if (((spriteData.x + spriteData.width > wallX && spriteData.x < wallX + wallWidth) && spriteData.y + spriteData.height <= wallY + 20) && velocity.y >= 0)
	{
		topCollision(wallY);
	}
	// Case: Left of surface
	else if (spriteData.x + spriteData.width >= wallX && spriteData.x + spriteData.width < wallX + wallWidth && spriteData.y < wallY + wallHeight)
	{
		leftCollision(wallX);
	}
	// Case: Right of surface
	else if (spriteData.x < wallX + wallWidth && spriteData.x + spriteData.width > wallX  && spriteData.y < wallY + wallHeight)
	{
		rightCollision(wallX, wallWidth);
	}
}

//=============================================================================
// Collision Handlers
// Helper functions for stop() functions. Resolve Megaman's collisions
// for each direction when Mega Man collides with a wall or floor.
//=============================================================================
void Megaman::topCollision(double wallY)
{
	standingOnSurface_ = true;
	spriteData.y = wallY - spriteData.height + 1;		 // position at the top of the wall
	velocity.y = 0; 										 // stop y acceleration
	floorCollision_ = true;
	isDashJumping_ = false;
	isDashing_ = false;
	canWallJump_ = false;
}
void Megaman::leftCollision(double wallX)
{
	spriteData.x = wallX - spriteData.width;			// position at the left of the wall
	//velocity.x = 0;
	//if (velocity.y > 0 && !input->isKeyDown(UP_KEY) && !input->getGamepadA(0))
	//{
	//		canWallJump_ = true;
	//}
	if (velocity.y > 0 && velocity.x != 0)
	{
		canWallJump_ = true;
	}
}
void Megaman::rightCollision(double wallX, double wallWidth)
{
	spriteData.x = wallX + wallWidth + 1; //+1;			// position at the right of the wall
	//velocity.x = 0;
	//if (velocity.y > 0 && !input->isKeyDown(UP_KEY) && !input->getGamepadA(0))
	//{
	//		canWallJump_ = true;
	//}
	if (velocity.y > 0 && velocity.x != 0)
	{
		canWallJump_ = true;
	}
}
void Megaman::bottomCollision(double wallY, double wallHeight)
{
	spriteData.y = wallY + wallHeight + 1;				// position underneath the wall
	velocity.y = 1;										// set velocity to make megaman fall
	standingOnSurface_ = false;			
	floorCollision_ = false;
	canWallJump_ = false;
	canJump_ = false;
}

//=============================================================================
// draw
// Draws Mega Man according to his current state
//============================================================================
void Megaman::draw()
{
	edge.top = -megamanNS::HEIGHT / 2;			// reset top collision edge

	if (isInvincible_)
	{
		flicker++;
	}
	if (!isInvincible_ || (isInvincible_ && flicker % 3 == 0))
	{
		if (spriteData.state == WALKING)
		{
			//if (spriteData.shotType == NONE)
			//{
				megamanWalking.draw(spriteData);
			//}
			//else
			//{
			//	megamanShootingWalking.draw(spriteData);
			//}
		}
		else if (spriteData.state == WALL_SLIDING)
		{
			megamanWallSliding.draw(spriteData);
		}
		else if (spriteData.state == DASHING)
		{
			edge.top = 0;							// make top collision edge lower if dashing
			if (spriteData.shotType == NONE)
			{
				megamanDashing.draw(spriteData);
			}
			else
			{
				megamanShootingDashing.draw(spriteData);
			}
		}
		else if (spriteData.state == JUMPING)
		{
			if (spriteData.shotType == NONE)
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
		else if (spriteData.state == DAMAGED)
		{
			megamanDamaged.draw(spriteData);
		}
		else if (spriteData.shotType != NONE)
		{
			megamanShooting.draw(spriteData);
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
}

