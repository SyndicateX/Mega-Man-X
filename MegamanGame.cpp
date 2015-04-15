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
	mapY = 0;
	tileMapX = 0;
	tileMapY = 0;
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

	// bullet charged small texture
	if (!bulletChargedSmallTexture.initialize(graphics, BULLET_CHARGED_SMALL_IMAGE))
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

	// megaman charging sprites
	if (!chargingSprites.initialize(this, chargingSpritesNS::WIDTH, chargingSpritesNS::HEIGHT, 0, &chargingSpritesTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing charging sprites"));
	
	// bullet charged small
	if (!bulletChargedSmall.initialize(this, bulletChargedSmallNS::WIDTH, bulletChargedSmallNS::HEIGHT, 0, &bulletChargedSmallTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));

	// floors
	for (int i = 0; i < MH; i++)
	{
		for (int j = 0; j < MW; j++)
		{
			if (tileMap[i][j] >= 0)
			{
				floor.push_back(Entity());
				RECT rect = { -64, -64, 64, 64 };
				if (!floor[floor.size() - 1].initialize(this, TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COLS, &tileTextures))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing floor"));
				floor[floor.size() - 1].setFrames(0, 0);
				floor[floor.size() - 1].setCurrentFrame(0);
				floor[floor.size() - 1].setCollisionType(entityNS::ROTATED_BOX);
				floor[floor.size() - 1].setEdge(rect);
				// Set post position
				floor[floor.size() - 1].setX(j*TEXTURE_SIZE);
				floor[floor.size() - 1].setY(i*TEXTURE_SIZE);
			}
		}
	}

	//// bullet
	//for (int i = 0; i < 3; i++)
	//{
	//	if (!bullet[i].initialize(this, bulletNS::WIDTH, bulletNS::HEIGHT, 0, &bulletTexture))
	//		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	//}

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void MegamanGame::update()
{
	static LARGE_INTEGER lastShootTime;
	LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&currentTime);
	static LARGE_INTEGER dashTime = currentTime;
	static int chargeTime = 0;
	static bool directionChange = false;

	if (megaman.isDashing() && megaman.getState() == JUMPING)
	{
		megaman.setIsDashJumping(true);
	}

	//******************************** PRESSED LEFT *******************************
	if (input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0))
	{
		if (megaman.getDirection() == RIGHT)
		{
			directionChange = true;
			megaman.setIsDashing(false);
		}
		else
		{
			directionChange = false;
		}

		if (megaman.isDashJumping())
		{
			moveMegaman(DASH_SPEED);
		}
		else if (!megaman.isDashing())
		{
			moveMegaman(WALK_SPEED);
		}

		megaman.setDirection(LEFT);
		megaman.setState(WALKING);
	}
	//******************************* PRESSED RIGHT *******************************
	else if (input->isKeyDown(RIGHT_KEY) || input->getGamepadDPadRight(0))
	{ 
		if (megaman.getDirection() == LEFT)
		{
			directionChange = true;
			megaman.setIsDashing(false);
		}
		else
		{
			directionChange = false;
		}

		if (megaman.isDashJumping())
		{
			moveMegaman(DASH_SPEED);
		}
		else if (!megaman.isDashing())
		{
			moveMegaman(WALK_SPEED);
		}

		megaman.setState(WALKING);
		megaman.setDirection(RIGHT);
	}	
	//********************************** IDLE *************************************
	else
	{
		if (megaman.getShotState() != NONE && (currentTime.QuadPart - lastShootTime.QuadPart) / (double)(frequency.QuadPart) < .35)
		{
			megaman.setState(SHOOTING);		// Holds megamn shooting animation for a few frames after shooting
		}
		else
		{
			megaman.setShotState(NONE);
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
				megaman.setState(SHOOTING);
				megaman.setShotState(SHOOT);
				if (bullet.size() < MAX_BULLETS)				// 4 bullets on screen max (maybe more?)
				{
					bullet.push_back(Bullet());

					if (!bullet[bullet.size() - 1].initialize(this, bulletNS::WIDTH, bulletNS::HEIGHT, 0, &bulletTexture))
						throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));

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
						bullet[bullet.size() - 1].setX(megaman.getX() + megaman.getWidth());
						bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 10);
					}
					else
					{
						bullet[bullet.size() - 1].flipHorizontal(true);
						bullet[bullet.size() - 1].setX(megaman.getX());
						bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 10);
					}
				}
				lastShootTime = currentTime;
				justShot = true;
			}
		}
		else							// If button is held down more than a frame, charge timer increments
		{
			if (chargeTime < 40)
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

		if (chargeTime >= 40 && !(input->isKeyDown(ENTER_KEY)))				// If the charge timer reaches its target, mega man fires a charged shot
		{
			chargingSprites.setCharge1(false);
			megaman.setState(SHOOTING);
			megaman.setShotState(SHOOT);
			bulletChargedSmall.setDirection(megaman.getDirection());
			if (megaman.canWallJump())
			{
				if (bulletChargedSmall.getDirection() == RIGHT)
					bulletChargedSmall.setDirection(LEFT);
				else
					bulletChargedSmall.setDirection(RIGHT);
			}
			if (bulletChargedSmall.getDirection() == RIGHT)
			{
				bulletChargedSmall.flipHorizontal(false);
				bulletChargedSmall.setDirection(RIGHT);
				bulletChargedSmall.setX(megaman.getX() + megaman.getWidth());
				bulletChargedSmall.setY(megaman.getY() + megaman.getHeight() / 3 - 20);
			}
			else
			{
				bulletChargedSmall.flipHorizontal(true);
				bulletChargedSmall.setDirection(LEFT);
				bulletChargedSmall.setX(megaman.getX());
				bulletChargedSmall.setY(megaman.getY() + megaman.getHeight() / 3 - 20);
			}
			lastShootTime = currentTime;
		}
		chargeTime = 0;		// reset charge time
		justShot = false;	// allows another small shot to be fired
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
	if (directionChange)
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

	if (bulletChargedSmall.getDirection() == RIGHT)
	{
		bulletChargedSmall.setX(bulletChargedSmall.getX() + bulletChargedSmallNS::SPEED*frameTime);
	}
	else
	{
		bulletChargedSmall.setX(bulletChargedSmall.getX() - bulletChargedSmallNS::SPEED*frameTime);
	}


	chargingSprites.update(frameTime);
	bulletChargedSmall.update(frameTime);			//Update the rest					
    megaman.update(frameTime);
}

//=============================================================================
// Handle Mega Man and map movements
//=============================================================================
void MegamanGame::moveMegaman(double moveRate)
{
	//if (megaman.getDirection() == LEFT)
	//{
	//	if (mapX > 0 && megaman.getX() < GAME_WIDTH / 2)
	//		mapX -= megamanNS::SPEED * frameTime * moveRate;
	//	else
	//		megaman.setX(megaman.getX() - megamanNS::SPEED * frameTime * moveRate);
	//}
	//else
	//{
	//	if (mapX < MAP_WIDTH && megaman.getX() >= GAME_WIDTH / 2)
	//		mapX += megamanNS::SPEED * frameTime * moveRate;
	//	else if (mapX >= MAP_WIDTH && megaman.getX() <= GAME_WIDTH)
	//		megaman.setX(megaman.getX() + megamanNS::SPEED * frameTime * moveRate);
	//	else
	//		megaman.setX(megaman.getX() + megamanNS::SPEED * frameTime * moveRate);
	//}
	if (megaman.getDirection() == LEFT)
	{
			megaman.setX(megaman.getX() - megamanNS::SPEED * frameTime * moveRate);
	}
	else
	{
			megaman.setX(megaman.getX() + megamanNS::SPEED * frameTime * moveRate);
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void MegamanGame::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void MegamanGame::collisions()
{
    VECTOR2 cv;
	//if (bulletChargedSmall.collidesWith(paddle, cv))
	//	Destroy? Move out of bounds? Reset parameters?

	for (int i = 0; i < floor.size(); i++)
	{
		for (int j = 0; j < bullet.size(); j++)
		{
			if (bullet[j].collidesWith(floor[i], cv))
				bullet.erase(bullet.begin() + j);
		}
		if (megaman.collidesWith(floor[i], cv)) // 
			megaman.stop(floor[i].getX(), floor[i].getY(), floor[i].getWidth(), floor[i].getHeight());
	}
}

//=============================================================================
// Render game items
//=============================================================================
void MegamanGame::render()
{
    graphics->spriteBegin();                // begin drawing sprites

	//if (mapX > 0 && mapX < MAP_WIDTH && megaman.getX())
	//{
	//	//backdrop.setX(-mapX);
	//	//backdrop.setY(-mapY);
	//}
	//else
	//{
	//	if (mapX <= 0)
	//	{
	//		mapX = 0;
	//	}
	//	else
	//	{
	//		mapX = MAP_WIDTH;
	//	}
	//}
	
	backdrop.draw();                        // add the backdrop to the scene

	for (int row = 0; row<MH; row++)       // for each row of map
	{
		tile.setY((float)(row*TEXTURE_SIZE)); // set tile Y
		for (int col = 0; col<MAP_WIDTH; col++)    // for each column of map
		{
			if (tileMap[row][col] >= 0)          // if tile present
			{
				tile.setCurrentFrame(tileMap[row][col]);    // set tile texture
				tile.setX((float)(col*TEXTURE_SIZE) + tileMapX);  // set tile X
				tile.setY((float)(row*TEXTURE_SIZE) + tileMapY);  // set tile Y
				// if tile on screen
				if ((tile.getX() > -TEXTURE_SIZE && tile.getX() < GAME_WIDTH) &&
					(tile.getY() > -TEXTURE_SIZE && tile.getY() < GAME_HEIGHT))
					tile.draw();                // draw tile
			}
		}
	}

    megaman.draw();							// add megaman to the scene
	chargingSprites.draw();					// add megaman charging sprites to the scene
	bulletChargedSmall.draw();				// add small charged bullet to the scene
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
    megamanTexture.onLostDevice();            // megaman texture
	bulletTexture.onLostDevice();			// bullet texture
	bulletChargedSmallTexture.onLostDevice();			// bullet texture
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
    backdropTexture.onResetDevice();
	tileTextures.onResetDevice();
    megamanTexture.onResetDevice();
	chargingSpritesTexture.onResetDevice();
	bulletTexture.onResetDevice();
	bulletChargedSmallTexture.onResetDevice();

    Game::resetAll();
    return;
}
