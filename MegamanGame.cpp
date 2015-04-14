#include "MegamanGame.h"
#include "sprites.h"
#include "Timer.h"

//=============================================================================
// Constructor
//=============================================================================
MegamanGame::MegamanGame()
{
	mapX = 0;
	mapY = 0;
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

	// paddle texture
	if (!paddleTexture.initialize(graphics, PADDLE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

    // backdrop
    if (!backdrop.initialize(graphics,0,0,0,&backdropTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// paddle
	if (!paddle.initialize(this, paddleNS::WIDTH, paddleNS::HEIGHT, 0, &paddleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle"));

    // megaman
	if (!megaman.initialize(this, megamanNS::WIDTH, megamanNS::HEIGHT, 0, &megamanTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));

	// megaman charging sprites
	if (!chargingSprites.initialize(this, chargingSpritesNS::WIDTH, chargingSpritesNS::HEIGHT, 0, &chargingSpritesTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing charging sprites"));
	
	// bullet charged small
	if (!bulletChargedSmall.initialize(this, bulletChargedSmallNS::WIDTH, bulletChargedSmallNS::HEIGHT, 0, &bulletChargedSmallTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));

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
	static bool isDashing = false;
	static bool directionChange = false;

	//******************************** PRESSED LEFT *******************************
	if (input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0))
	{
		if (megaman.getDirection() == RIGHT)
		{
			directionChange = true;
			isDashing = false;
		}
		else
		{
			directionChange = false;
		}
		if (!isDashing)
		{
			moveMegaman(WALK_SPEED);
		}
		if (isDashing && megaman.getState() == JUMPING)
		{
			moveMegaman(DASH_SPEED);
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
			isDashing = false;
		}
		else
		{
			directionChange = false;
		}
		if (!isDashing)
		{
			moveMegaman(WALK_SPEED);
		}
		if (isDashing && megaman.getState() == JUMPING)
		{
			moveMegaman(DASH_SPEED);
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
						bullet[bullet.size() - 1].setX(megaman.getX() + megaman.getWidth());
						bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 10);
					}
					else
					{
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
				bulletChargedSmall.setDirection(RIGHT);
				bulletChargedSmall.setX(megaman.getX() + megaman.getWidth());
				bulletChargedSmall.setY(megaman.getY() + megaman.getHeight() / 3 - 20);
			}
			else
			{
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
		isDashing = true;
		megaman.setState(DASHING);
		moveMegaman(2.5);

		if ((currentTime.QuadPart - dashTime.QuadPart) / (double)(frequency.QuadPart) < 0.42)
		{}
		else
		{		
			megaman.setCanDash(false);		// Megaman cannot dash again until this is reset after user releases dash key
			dashTime = currentTime;
			isDashing = false;
		}
	}

	if ((!(input->isKeyDown(SPACE_KEY)) && !(input->getGamepadB(0))))
	{
		isDashing = false;
		megaman.setCanDash(false);
		dashTime = currentTime; // Reset dash timer if megaman is not dashing
	}
	if (directionChange)
	{
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
	paddle.update(frameTime);						
    megaman.update(frameTime);
}

//=============================================================================
// Handle Mega Man and map movements
//=============================================================================
void MegamanGame::moveMegaman(double moveRate)
{
	if (megaman.getDirection() == LEFT)
	{
		if (mapX > 0 && megaman.getX() < GAME_WIDTH / 2)
			mapX -= megamanNS::SPEED * frameTime * moveRate;
		else
			megaman.setX(megaman.getX() - megamanNS::SPEED * frameTime * moveRate);
	}
	else
	{
		if (mapX < MAP_WIDTH && megaman.getX() >= GAME_WIDTH / 2)
			mapX += megamanNS::SPEED * frameTime * moveRate;
		else if (mapX >= MAP_WIDTH && megaman.getX() <= GAME_WIDTH)
			megaman.setX(megaman.getX() + megamanNS::SPEED * frameTime * moveRate);
		else
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
	for (int i = 0; i < bullet.size(); i++)
	{
		if (bullet[i].collidesWith(paddle, cv))
			bullet.erase(bullet.begin() + i);
	}

	//if (bulletChargedSmall.collidesWith(paddle, cv))
	//	Destroy? Move out of bounds? Reset parameters?

	if (megaman.collidesWith(paddle, cv)) // 
		megaman.stop(paddle.getX(), paddle.getY(), paddleNS::WIDTH, paddleNS::HEIGHT);
}

//=============================================================================
// Render game items
//=============================================================================
void MegamanGame::render()
{
    graphics->spriteBegin();                // begin drawing sprites

	if (mapX > 0 && mapX < MAP_WIDTH)
	{
		paddle.setX(paddleNS::X - mapX);
		//paddle.setY(paddleNS::Y - mapY);
		backdrop.setX(-mapX);
		//backdrop.setY(-mapY);
	}
	else
	{
		if (mapX <= 0)
		{
			mapX = 0;
		}
		else
		{
			mapX = MAP_WIDTH;
		}
	}

    backdrop.draw();                        // add the backdrop to the scene
	paddle.draw();							// add the paddle to the scene
    megaman.draw();							// add megaman to the scene
	chargingSprites.draw();
	bulletChargedSmall.draw();
	for (int i = 0; i < bullet.size(); i++)
	{
		bullet[i].draw();
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
	paddleTexture.onLostDevice();
	chargingSpritesTexture.onLostDevice();
    backdropTexture.onLostDevice();         // backdrop texture

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
	paddleTexture.onResetDevice();
    megamanTexture.onResetDevice();
	chargingSpritesTexture.onResetDevice();
	bulletTexture.onResetDevice();
	bulletChargedSmallTexture.onResetDevice();

    Game::resetAll();
    return;
}
