#include "Levels.h"
#include "sprites.h"
//using namespace levelsNS;

//=============================================================================
// Constructor
//=============================================================================
Levels::Levels()
{
	mapX = 0;
	mapY = megaman.getY();
	oldX_ = 0;
	oldY_ = 0;
	directionChange_ = false;
	levelComplete_ = false;
	levelFailed_ = false;
	bulletNumber_ = 0;
}

//=============================================================================
// Destructor
//=============================================================================
Levels::~Levels()
{
	releaseAll();           // call onLostDevice() for every graphics item
	for (int i = 0; i < enemy.size(); i++)
	{
		delete enemy[i];
	}
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Levels::initialize(HWND& hwnd, Graphics* graphics, Input* input, Game* game)
{
	audio = game->getAudio();                // the audio system

	// megaman texture
	if (!megamanTexture.initialize(graphics, MEGAMAN_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// megaman charging sprites texture
	if (!chargingSpritesTexture.initialize(graphics, CHARGING_SPRITES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// bullet texture
	if (!bulletTexture.initialize(graphics, BULLET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet texture"));

	// megaman
	if (!megaman.initialize(game, megamanNS::WIDTH, megamanNS::HEIGHT, 0, &megamanTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));

	// main game textures
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

	// health bar
	healthBar.initialize(graphics, &gameTextures, 0, levelsNS::HEALTHBAR_Y, 5.0f, graphicsNS::WHITE);

	// megaman charging sprites
	if (!chargingSprites.initialize(game, chargingSpritesNS::WIDTH, chargingSpritesNS::HEIGHT, 0, &chargingSpritesTexture))
	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing charging sprites"));

	//bullet
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bullet.push_back(Bullet());
		if (!bullet[i].initialize(game, bulletNS::WIDTH, bulletNS::HEIGHT, 0, &bulletTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet"));
	}

return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Levels::updateMegaman(double MAP_WIDTH, double MAP_HEIGHT, float frameTime, Input* input, Game* game)
{
	//oldY_ = megaman.getY();
	//oldX_ = megaman.getX();
	static LARGE_INTEGER lastShootTime;
	LARGE_INTEGER frequency;
	LARGE_INTEGER currentTime;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&currentTime);
	static LARGE_INTEGER dashTime = currentTime;
	static int chargeTime = 0;

	if (megaman.getX() >= GAME_WIDTH - megaman.getWidth())
	{
		levelComplete_ = true;
	}

	if (megaman.isDead())
	{
		levelFailed_ = true;
	}

	if (megaman.isDashing() && megaman.getState() == JUMPING)
	{
		megaman.setIsDashJumping(true);
	}

	if (megaman.getState() != DAMAGED && megaman.getState() != DEAD)	//Grab user input if megaman is not currently taking damage or dead
	{

		//******************************** PRESSED LEFT *******************************
		if (input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0))
		{
			leftRightInput(LEFT, MAP_WIDTH, MAP_HEIGHT);
		}
		//******************************* PRESSED RIGHT *******************************
		else if (input->isKeyDown(RIGHT_KEY) || input->getGamepadDPadRight(0))
		{
			leftRightInput(RIGHT, MAP_WIDTH, MAP_HEIGHT);
		}
		//********************************** IDLE *************************************
		else
		{
			// Holds megaman's shooting animation for a few frames after shooting
			megaman.setVelocity(VECTOR2(0, megaman.getVelocity().y));
			if (megaman.getShotType() != NONE && (currentTime.QuadPart - lastShootTime.QuadPart) / (double)(frequency.QuadPart) < .35)
			{
				megaman.setState(SHOOTING);
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
				{
				}
				else
				{
					bool shotWasFired = false;
					for (int i = 0; i < bullet.size() && !shotWasFired; i++)
					{
						if (!bullet[i].getVisible() && !bullet[i].getActive())
						{
							megaman.setState(SHOOTING);
							megaman.setShotType(REGULAR_SHOT);
							bullet[i].setVisible(true);
							bullet[i].setActive(true);
							bullet[i].setShotType(megaman.getShotType());
							shoot(i);
							lastShootTime = currentTime;
							justShot = true;
							shotWasFired = true;
						}
					}
				}
			}
			else							// If button is held down more than a frame, charge timer increments
			{
				if (chargeTime == 40)
				{
					audio->playCue(LOAD);
				}
				if (chargeTime > 80)
				{
					audio->playCue(MAX);
				}
				if (chargeTime <= 80)
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
				bool shotWasFired = false;
				for (int i = 0; i < bullet.size() && !shotWasFired; i++)
				{
					if (!bullet[i].getActive() && !bullet[i].getVisible())
					{
						bullet[i].setVisible(true);
						bullet[i].setActive(true);
						bullet[i].setShotType(megaman.getShotType());
						chargingSprites.setCharge1(false);
						shoot(i);
						lastShootTime = currentTime;
						shotWasFired = true;
					}
				}
			}
			chargeTime = 0;			// reset charge time						++ Both are reset even if no charged shot
			justShot = false;		// allows another small shot to be fired	++ was fired
		}

		//************************************* DASHING *************************************
		if (megaman.canDash() && (input->isKeyDown(SPACE_KEY) || input->getGamepadB(0)))// && !megaman.getState() == JUMPING)
		{
			megaman.setIsDashing(true);
			megaman.setState(DASHING);
			moveMegaman(DASH_SPEED, MAP_WIDTH, MAP_HEIGHT);

			if ((currentTime.QuadPart - dashTime.QuadPart) / (double)(frequency.QuadPart) < 0.42)
			{
			}
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
	}
	//*********************************** UPDATE DATA ************************************
	for (int i = 0; i < bullet.size(); i++)			// Update bullet data -- is this the best place for this?
	{
		if (bullet[i].getVisible() && bullet[i].getActive())
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
				bullet[i].setVisible(false);
				bullet[i].setActive(false);
			}
		}
	}

	chargingSprites.update(frameTime);
	megaman.update(frameTime);
}


//=============================================================================
// Handle Mega Man's bullets initialization
//=============================================================================
void Levels::shoot(int index)
{
	chargingSprites.setCharge1(false);
	megaman.setState(SHOOTING);
	bullet[index].setDirection(megaman.getDirection());
	if (megaman.canWallJump())
	{
		if (bullet[index].getDirection() == RIGHT)
			bullet[index].setDirection(LEFT);
		else
			bullet[index].setDirection(RIGHT);
	}
	if (bullet[index].getDirection() == RIGHT)
	{
		bullet[index].flipHorizontal(false);
		bullet[index].setDirection(RIGHT);
		bullet[index].setX(megaman.getX() + megaman.getWidth());
		bullet[index].setY(megaman.getY() + megaman.getHeight() / 3 - 20);
	}
	else
	{
		bullet[index].flipHorizontal(true);
		bullet[index].setDirection(LEFT);
		bullet[index].setX(megaman.getX());
		bullet[index].setY(megaman.getY() + megaman.getHeight() / 3 - 20);
	}
	bullet[index].setInitialY(mapY, megaman.canWallJump(), megaman.isDashing());
}

//=============================================================================
// Handles left and right keyboard/gamepad input
//=============================================================================
void Levels::leftRightInput(Direction direction, double MAP_WIDTH, double MAP_HEIGHT)
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
		moveMegaman(DASH_SPEED, MAP_WIDTH, MAP_HEIGHT);
	}
	else if (!megaman.isDashing())
	{
		moveMegaman(WALK_SPEED, MAP_WIDTH, MAP_HEIGHT);
	}

	megaman.setDirection(direction);
	megaman.setState(WALKING);
}

//=============================================================================
// Handle Mega Man and map movements on the x-coordinate
//=============================================================================
void Levels::moveMegaman(double moveRate, double MAP_WIDTH, double MAP_HEIGHT)
{
	if (megaman.getDirection() == LEFT)
	{
		if (mapX > 0 && megaman.getX() < GAME_WIDTH / 2)
		{
			megaman.setVelocity(VECTOR2(-moveRate * megamanNS::SPEED, megaman.getVelocity().y));
		}
		else
		{
			megaman.setVelocity(VECTOR2(-moveRate * megamanNS::SPEED, megaman.getVelocity().y));
		}
	}
	else
	{
		if (mapX < MAP_WIDTH && megaman.getX() >= GAME_WIDTH / 2)
		{
			megaman.setVelocity(VECTOR2(moveRate * megamanNS::SPEED, megaman.getVelocity().y));
		}
		else if (mapX >= MAP_WIDTH && megaman.getX() <= GAME_WIDTH)
		{
			megaman.setVelocity(VECTOR2(moveRate * megamanNS::SPEED, megaman.getVelocity().y));
		}
		else
		{
			megaman.setVelocity(VECTOR2(moveRate * megamanNS::SPEED, megaman.getVelocity().y));
		}
	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Levels::releaseAll()
{
	mechaSonicTexture.onLostDevice();
	megamanTexture.onLostDevice();          // megaman texture
	bulletTexture.onLostDevice();			// bullet texture
	bulletTexture.onLostDevice();			// bullet texture
	chargingSpritesTexture.onLostDevice();
	backdropTexture.onLostDevice();         // backdrop texture
	tileTextures.onLostDevice();

	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Levels::resetAll()
{
	mechaSonicTexture.onResetDevice();
	backdropTexture.onResetDevice();
	tileTextures.onResetDevice();
	megamanTexture.onResetDevice();
	chargingSpritesTexture.onResetDevice();
	bulletTexture.onResetDevice();
	bulletTexture.onResetDevice();

	return;
}

bool Levels::isLevelComplete()
{
	return levelComplete_;
}

bool Levels::levelFailed()
{
	return levelFailed_;
}
