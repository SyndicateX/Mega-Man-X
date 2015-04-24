#include "MegamanGame.h"
#include "sprites.h"
#include "Timer.h"
using namespace megamanGameNS;

//=============================================================================
// Constructor
//=============================================================================
MegamanGame::MegamanGame()
{
	mapX = 0;
	mapY = megaman.getY();
	oldX_ = 0;
	oldY_ = 0;
	directionChange_ = false;
}

//=============================================================================
// Destructor
//=============================================================================
MegamanGame::~MegamanGame()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void MegamanGame::initialize(HWND hwnd)
{
    Game::initialize(hwnd);
    // backdrop texture
    if (!backdropTexture.initialize(graphics,BACKDROP_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// enemy texture
	if (!mechaSonicTexture.initialize(graphics, ENEMY001))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// map textures
	if (!tileTextures.initialize(graphics, TILE_TEXTURES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

    // megaman texture
    if (!megamanTexture.initialize(graphics,MEGAMAN_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// megaman charging sprites texture
	if (!chargingSpritesTexture.initialize(graphics, CHARGING_SPRITES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// bullet texture
	if (!bulletTexture.initialize(graphics, BULLET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet texture"));

    // backdrop
    if (!backdrop.initialize(graphics,0,0,0,&backdropTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// tile image
	if (!tile.initialize(graphics, TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COLS, &tileTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
	tile.setFrames(0, 0);
	tile.setCurrentFrame(0);

    // megaman
	if (!megaman.initialize(this, megamanNS::WIDTH, megamanNS::HEIGHT, 0, &megamanTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));

	// enemy
	if (!mechaSonic.initialize(this, enemyNS::WIDTH, enemyNS::HEIGHT, 0, &mechaSonicTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

	// megaman charging sprites
	if (!chargingSprites.initialize(this, chargingSpritesNS::WIDTH, chargingSpritesNS::HEIGHT, 0, &chargingSpritesTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing charging sprites"));

	// floors
	for (int i = 0; i < TILE_ROWS; i++)
	{
		for (int j = 0; j < TILE_COLUMNS; j++)
		{
			if (tileMap[i][j] >= 0)
			{
				floor.push_back(Entity());
				if (!floor[floor.size() - 1].initialize(this, TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COLS, &tileTextures))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing floor"));
				floor[floor.size() - 1].setCollisionType(entityNS::BOX);
				floor[floor.size() - 1].setEdge(MEGAMAN_EDGE);
				// Set floor position
				floor[floor.size() - 1].setX(j*TEXTURE_SIZE);
				floor[floor.size() - 1].setY(i*TEXTURE_SIZE);
			}
		}
	}

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void MegamanGame::update()
{
	oldY_ = megaman.getY();
	oldX_ = megaman.getX();
	static LARGE_INTEGER lastShootTime;
	LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&currentTime);
	static LARGE_INTEGER dashTime = currentTime;
	static int chargeTime = 0;

	if (megaman.isDashing() && megaman.getState() == JUMPING)
	{
		megaman.setIsDashJumping(true);
	}


	//******************************** PRESSED LEFT *******************************
	if (input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0))
	{
		leftRightInput(LEFT);
	}
	//******************************* PRESSED RIGHT *******************************
	else if (input->isKeyDown(RIGHT_KEY) || input->getGamepadDPadRight(0))
	{ 
		leftRightInput(RIGHT);
	}	
	//********************************** IDLE *************************************
	else
	{
		megaman.setVelocity(VECTOR2(0, megaman.getVelocity().y));
		if (megaman.getShotType() != NONE && (currentTime.QuadPart - lastShootTime.QuadPart) / (double)(frequency.QuadPart) < .35)
		{
			megaman.setState(SHOOTING);		// Holds megamn shooting animation for a few frames after shooting
		}
		else
		{
			megaman.setShotType(NONE);
			megaman.setState(STANDING);
		}
	}
	//**************************** Shooting Animation + Bullet Creation ****************************
	static bool justShot = false;
	if (megaman.canShoot() && (input->isKeyDown(ENTER_KEY) || input->getGamepadX(0))) 
	{
		if (!justShot)						// Single shot scenario. Activated when the shooting button is first pressed 
		{
			if ((currentTime.QuadPart - lastShootTime.QuadPart) / (double)(frequency.QuadPart) < .05)	
			{}
			else
			{
				if (bullet.size() < MAX_BULLETS)
				{
					megaman.setState(SHOOTING);
					megaman.setShotType(REGULAR_SHOT);
					bullet.push_back(Bullet());
					if (!bullet[bullet.size() - 1].initialize(this, bulletNS::WIDTH, bulletNS::HEIGHT, 0, &bulletTexture))
						throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));
					bullet[bullet.size() - 1].setShotType(REGULAR_SHOT);

					shoot();
					lastShootTime = currentTime;
					justShot = true;
				}
			}
		}
		else							// If button is held down more than a frame, charge timer increments
		{
			if (chargeTime < 80)
				chargeTime++;
			if (chargeTime >= 40)
			{
				chargingSprites.setCharge1(true);
				chargingSprites.setX(megaman.getX() - 65);
				chargingSprites.setY(megaman.getY() - 55);
			}
		}
	}

	if ((!(input->isKeyDown(ENTER_KEY)) && (!(input->getGamepadX(0))) && justShot))
	{
		megaman.setCanShoot(true);

		 if (chargeTime >= 40 && !(input->isKeyDown(ENTER_KEY)))	// If the charge timer reaches its target, mega man fires a small charged shot
		{
			 megaman.setState(SHOOTING);
			 megaman.setShotType(SMALL_CHARGE);
			 if (chargeTime >= 80) // If the charge timer reaches its target, mega man fires a medium charged shot
			 {
				 megaman.setShotType(MEDIUM_CHARGE);
			 }
			bullet.push_back(Bullet());
			if (!bullet[bullet.size() - 1].initialize(this, bulletNS::WIDTH, bulletNS::HEIGHT, 0, &bulletTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));
			bullet[bullet.size() - 1].setShotType(megaman.getShotType());
			chargingSprites.setCharge1(false);
			shoot();
			lastShootTime = currentTime;
		}
		chargeTime = 0;			// reset charge time						// Both are reset even if no charged shot
		justShot = false;		// allows another small shot to be fired	// was fired
	}

	//************************************* DASHING *************************************
	if (megaman.canDash() && (input->isKeyDown(SPACE_KEY) || input->getGamepadB(0)))// && !megaman.getState() == JUMPING)
	{
		megaman.setIsDashing(true);
		megaman.setState(DASHING);
		moveMegaman(DASH_SPEED);

		if ((currentTime.QuadPart - dashTime.QuadPart) / (double)(frequency.QuadPart) < 0.42)
		{ }
		else
		{		
			megaman.setCanDash(false);		// Megaman cannot dash again until this is reset after user releases dash key
			dashTime = currentTime;
			megaman.setIsDashing(false);
		}
	}

	if ((!(input->isKeyDown(SPACE_KEY)) && !(input->getGamepadB(0))))
	{
		megaman.setIsDashing(false);
		megaman.setCanDash(false);
		dashTime = currentTime; // Reset dash timer if megaman is not dashing
	}
	if (directionChange_)
	{
		megaman.setIsDashing(false);
		megaman.setCanDash(false);
	}

	//************************************ JUMPING ***************************************
	if ((megaman.canJump() || (megaman.canJump() && megaman.canWallJump())) && (input->isKeyDown(UP_KEY) || input->getGamepadA(0)))
	{
		if (megaman.canWallJump())
		{
			megaman.setDoWallJump(true);
			if (input->isKeyDown(SPACE_KEY) || input->getGamepadB(0))
			{
				megaman.setIsDashJumping(true);
			}
		}
		megaman.setState(JUMPING);
	}

	//*********************************** UPDATE DATA ************************************
	for (int i = 0; i < bullet.size(); i++)			// Update bullet data -- is this the best place for this?
	{
		if (bullet[i].getDirection() == RIGHT)
		{
			bullet[i].setX(bullet[i].getX() + bulletNS::SPEED*frameTime);
		}
		else
		{
			bullet[i].setX(bullet[i].getX() - bulletNS::SPEED*frameTime);
		}
		bullet[i].update(frameTime);
		if (bullet[i].getX() > GAME_WIDTH || bullet[i].getX() < 0)
		{
			bullet.erase(bullet.begin() + i);
		}
	}

	chargingSprites.update(frameTime);		
    megaman.update(frameTime);
}

//=============================================================================
// Handles left and right keyboard/gamepad input
//=============================================================================
void MegamanGame::leftRightInput(Direction direction)
{
	if (megaman.getDirection() != direction)
	{
		directionChange_ = true;
		megaman.setVelocity(VECTOR2(0, megaman.getVelocity().y));
		megaman.setIsDashing(false);
	}
	else
	{
		directionChange_ = false;
	}

	if (megaman.isDashJumping())
	{
		moveMegaman(DASH_SPEED);
	}
	else if (!megaman.isDashing())
	{
		moveMegaman(WALK_SPEED);
	}

	megaman.setDirection(direction);
	megaman.setState(WALKING);
}

//=============================================================================
// Handle Mega Man and map movements on the x-coordinate
//=============================================================================
void MegamanGame::moveMegaman(double moveRate)
{
	//if (megaman.getVelocity().x == 0)
	//{
		if (megaman.getDirection() == LEFT)
		{
			if (mapX > 0 && megaman.getX() < GAME_WIDTH / 2)
			{ 
				//mapX -= megamanNS::SPEED * frameTime * moveRate;
				megaman.setVelocity(VECTOR2(-moveRate * megamanNS::SPEED, megaman.getVelocity().y));
			}
			else
			{
				//megaman.setX(megaman.getX() - megamanNS::SPEED * frameTime * moveRate);
				megaman.setVelocity(VECTOR2(-moveRate * megamanNS::SPEED, megaman.getVelocity().y));
			}
		}
		else
		{
			if (mapX < MAP_WIDTH && megaman.getX() >= GAME_WIDTH / 2)
			{
				megaman.setVelocity(VECTOR2(moveRate * megamanNS::SPEED, megaman.getVelocity().y));
				//mapX += megamanNS::SPEED * frameTime * moveRate;
			}
			else if (mapX >= MAP_WIDTH && megaman.getX() <= GAME_WIDTH)
			{
				megaman.setVelocity(VECTOR2(moveRate * megamanNS::SPEED, megaman.getVelocity().y));
				//megaman.setX(megaman.getX() + megamanNS::SPEED * frameTime * moveRate);
			}
			else
			{
				megaman.setVelocity(VECTOR2(moveRate * megamanNS::SPEED, megaman.getVelocity().y));
				//megaman.setX(megaman.getX() + megamanNS::SPEED * frameTime * moveRate);
			}
		}
	//}
}

//=============================================================================
// Handle Mega Man's bullets initialization
//=============================================================================
void MegamanGame::shoot()
{
	chargingSprites.setCharge1(false);
	megaman.setState(SHOOTING);
	bullet[bullet.size() - 1].setDirection(megaman.getDirection());
	if (megaman.canWallJump())
	{
		if (bullet[bullet.size() - 1].getDirection() == RIGHT)
			bullet[bullet.size() - 1].setDirection(LEFT);
		else
			bullet[bullet.size() - 1].setDirection(RIGHT);
	}
	if (bullet[bullet.size() - 1].getDirection() == RIGHT)
	{
		bullet[bullet.size() - 1].flipHorizontal(false);
		bullet[bullet.size() - 1].setDirection(RIGHT);
		bullet[bullet.size() - 1].setX(megaman.getX() + megaman.getWidth());
		bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 20);
	}
	else
	{
		bullet[bullet.size() - 1].flipHorizontal(true);
		bullet[bullet.size() - 1].setDirection(LEFT);
		bullet[bullet.size() - 1].setX(megaman.getX());
		bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 20);
	}
	bullet[bullet.size() - 1].setInitialY(mapY);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void MegamanGame::ai()
{
	mechaSonic.update(frameTime);
}

//=============================================================================
// Handle collisions
//=============================================================================
void MegamanGame::collisions()
{
	VECTOR2 cv;
	for (int i = 0; i < floor.size(); i++)
	{
		for (int j = 0; j < bullet.size(); j++)
		{
			if (bullet[j].collidesWith(floor[i], cv))

				bullet.erase(bullet.begin() + j);
		}
		if (megaman.collidesWith(floor[i], cv)) // 
		{
			megaman.stop(floor[i].getX(), floor[i].getY(), floor[i].getWidth(), floor[i].getHeight());
		}
	}
	//if (megaman.collidesWith(mechaSonic, cv))
	//	megaman.stop(mechaSonic.getX(), mechaSonic.getY(),mechaSonic.getWidth(), mechaSonic.getHeight());
}

//=============================================================================
// Render game items
//=============================================================================
void MegamanGame::render()
{
    graphics->spriteBegin();                // begin drawing sprites
	mapY += megaman.getY() - oldY_;
	mapX += megaman.getX() - oldX_;

	megaman.setY(oldY_);

	if (mapX >= 0 && mapX <= TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)
	{
		backdrop.setX(-mapX);
		megaman.setX(oldX_);
	}

	backdrop.setY(-mapY + megamanNS::Y);

	int counter = 0;
	for (int i = 0; i < TILE_ROWS; i++)
	{
		for (int j = 0; j < TILE_COLUMNS; j++)
		{
			if (tileMap[i][j] >= 0)
			{
				if (mapX >= 0 && mapX <= TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)
				{
					floor[counter].setX(j*TEXTURE_SIZE - mapX);
				}
				else if (mapX < 0)
				{
					floor[counter].setX(j*TEXTURE_SIZE);
				}
				else if (mapX > TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)
				{
					floor[counter].setX(j*TEXTURE_SIZE - (TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH));// + TEXTURE_SIZE * TILE_ROWS + GAME_WIDTH);
				}
				floor[counter].setY(i*TEXTURE_SIZE - mapY + megamanNS::Y);
				counter++;
			}
		}
	}
	
	backdrop.draw();                        // add the backdrop to the scene

	for (int i = 0; i < bullet.size(); i++)
	{
		bullet[i].setY(bullet[i].getInitialY() - mapY + megamanNS::Y);
	}

	for (int row = 0; row<TILE_ROWS; row++)       // for each row of map
	{
		tile.setY((float)(row*TEXTURE_SIZE - mapY + megamanNS::Y)); // set tile Y
		for (int col = 0; col<TILE_COLUMNS; col++)    // for each column of map
		{
			if (tileMap[row][col] >= 0)          // if tile present
			{
				//tile.setCurrentFrame(tileMap[row][col]);			// set tile texture
				if (mapX >= 0 && mapX <= TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)
				{
					tile.setX((float)(col*TEXTURE_SIZE) - mapX);	// set tile X
				}
				else if (mapX < 0)
				{
					tile.setX((float)(col*TEXTURE_SIZE));	// set tile X
				}
				else if (mapX > TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)
				{
					tile.setX((float)(col*TEXTURE_SIZE) - (TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH));// +TEXTURE_SIZE * TILE_ROWS + GAME_WIDTH));	// set tile X
				}

				tile.setY((float)(row*TEXTURE_SIZE) - mapY + megamanNS::Y);	// set tile Y
				// if tile on screen
				if ((tile.getX() > -TEXTURE_SIZE && tile.getX() < GAME_WIDTH) &&
					(tile.getY() > -TEXTURE_SIZE && tile.getY() < GAME_HEIGHT))
					tile.draw();                // draw tile
			}
		}
	}
	mechaSonic.draw();						// add enemy to the scene
    megaman.draw();							// add megaman to the scene
	chargingSprites.draw();					// add megaman charging sprites to the scene
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet[i].draw();					// add regular uncharged bullet to the scene
	}
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void MegamanGame::releaseAll()
{
	mechaSonicTexture.onLostDevice();

    megamanTexture.onLostDevice();          // megaman texture
	bulletTexture.onLostDevice();			// bullet texture
	bulletTexture.onLostDevice();			// bullet texture
	chargingSpritesTexture.onLostDevice();
    backdropTexture.onLostDevice();         // backdrop texture
	tileTextures.onLostDevice();

    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void MegamanGame::resetAll()
{
	mechaSonicTexture.onResetDevice();

    backdropTexture.onResetDevice();
	tileTextures.onResetDevice();
    megamanTexture.onResetDevice();
	chargingSpritesTexture.onResetDevice();
	bulletTexture.onResetDevice();
	bulletTexture.onResetDevice();

    Game::resetAll();
    return;
}
