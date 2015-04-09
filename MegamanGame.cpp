
#include "MegamanGame.h"
#include "sprites.h"
#include "Timer.h"

//=============================================================================
// Constructor
//=============================================================================
MegamanGame::MegamanGame()
{}

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

    //// ball texture
    //if (!ballTexture.initialize(graphics,BALL_IMAGE))
    //    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball texture"));

    // megaman texture
    if (!megamanTexture.initialize(graphics,MEGAMAN_IMAGE))
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

    // ball
    //if (!ball.initialize(this,ballNS::WIDTH,ballNS::HEIGHT,0,&ballTexture))
    //    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball"));
    //ball.setX(GAME_WIDTH/4 - ballNS::WIDTH);
    //ball.setY(GAME_HEIGHT/2 - ballNS::HEIGHT);
    //ball.setVelocity(VECTOR2(ballNS::SPEED,-ballNS::SPEED)); // VECTOR2(X, Y)

	// paddle
	if (!paddle.initialize(this, paddleNS::WIDTH, paddleNS::HEIGHT, 0, &paddleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle"));

    // megaman
	if (!megaman.initialize(this, megamanNS::WIDTH, megamanNS::HEIGHT, 0, &megamanTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));

	// megaman sprite initialize
	//SpriteCoordinates megamanSpriteCoordinates;
	//megamanSpriteCoordinates.populateVector("xcoords.txt");
	//if (!megaman.initializeCoords(megamanSpriteCoordinates))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));
	
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

	static bool dashComplete = false;

	//******************************** WALKING LEFT ********************************
	if (input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0))
	{
		megaman.setX(megaman.getX() - megamanNS::SPEED*frameTime);
		megaman.setDirection(LEFT);
		megaman.setState(WALKING);
	}
	//******************************* WALKING RIGHT *******************************
	else if (input->isKeyDown(RIGHT_KEY) || input->getGamepadDPadRight(0))
	{
		megaman.setX(megaman.getX() + megamanNS::SPEED*frameTime);
		megaman.setState(WALKING);
		megaman.setDirection(RIGHT);
	}
	//********************************** IDLE ************************************
	else
	{
		if (megaman.getState() == SHOOTING && (currentTime.QuadPart - lastShootTime.QuadPart) / (double)(frequency.QuadPart) < .35)
		//if (megaman.getShotState() != NONE && (currentTime.QuadPart - lastShootTime.QuadPart) / (double)(frequency.QuadPart) < .35)
		{
			// This holds the shooting sprite momentarily after a shot is fired (when megaman is standing idle)
		}
		else if (megaman.getState() == DASHING && (currentTime.QuadPart - dashTime.QuadPart) / (double)(frequency.QuadPart) <= 0.65 && megaman.getCurrentFrame() != 73)
		{
			if (megaman.getDirection() == RIGHT)
			{
				megaman.setX(megaman.getX() + megamanNS::SPEED*frameTime);
			}
			else
			{
				megaman.setX(megaman.getX() - megamanNS::SPEED*frameTime);
			}
			megaman.setFrames(72, 73);
		}
		else
		{
			megaman.setShotState(NONE);
			megaman.setState(STANDING);
		}
	}
	
	//**************************** Shooting Animation + Bullet Creation ****************************

	//...................................................
	//.................... SINGLE SHOT ..................
	//...................................................
	if (input->isEnterKeyUp() && input->wasKeyPressed(ENTER_KEY)) 
	//if (input->isKeyDown(ENTER_KEY) || input->getGamepadX(0))			
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

				if (megaman.getDirection() == RIGHT)
				{
					bullet[bullet.size() - 1].setDirection(RIGHT);
					bullet[bullet.size() - 1].setX(megaman.getX() + megaman.getWidth());
					bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 10);
				}
				else
				{
					bullet[bullet.size() - 1].setDirection(LEFT);
					bullet[bullet.size() - 1].setX(megaman.getX());
					bullet[bullet.size() - 1].setY(megaman.getY() + megaman.getHeight() / 3 - 10);
				}
			}
			lastShootTime = currentTime;
		}
		chargeTime = 0;
	}
	//......................................................
	//................... CHARGED SHOT .....................
	//......................................................
	if (input->isKeyDown(ENTER_KEY) || input->getGamepadX(0))
	{
		if (chargeTime < 40)
			chargeTime++;
	}
	if (chargeTime >= 40 && input->isEnterKeyUp())
	{
		megaman.setState(SHOOTING);
		megaman.setShotState(SHOOT);
		if (megaman.getDirection() == RIGHT)
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
		chargeTime = 0;
	}

	//************************************* DASHING *************************************
	if (input->isSpaceKeyUp    ())
		dashTime = currentTime;
	if (input->isKeyDown(SPACE_KEY) || input->getGamepadB(0))// && !megaman.getState() == JUMPING)
	{
		dashComplete = false;
		megaman.setState(DASHING);
		if (megaman.getDirection() == LEFT)
		{
			megaman.setX(megaman.getX() - megamanNS::SPEED*frameTime);
		}
		else
		{
			megaman.setX(megaman.getX() + megamanNS::SPEED*frameTime);
		}

		if ((currentTime.QuadPart - dashTime.QuadPart) / (double)(frequency.QuadPart) < 0.65)
		{
			//if (megaman.getCurrentFrame() == 71)	// Freeze on last frame while dashing
			//{
			//	//megaman.setCurrentFrame(71);
			//	//megaman.setFrames(71, 71);
			//}
			//else
			//{
			//	//megaman.setFrames(68, 71);
			//}
		}
		else
		{
            dashComplete = true; 
			input->clearKeyPress(SPACE_KEY);
			dashTime = currentTime;
		}
	}

	//************************************ JUMPING ***************************************
	if (input->canJump() && input->isKeyDown(UP_KEY) || input->getGamepadA(0))
	{
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

	bulletChargedSmall.update(frameTime);			//Update the rest
	paddle.update(frameTime);						
    megaman.update(frameTime);
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
    //if(ball.collidesWith(megaman, cv))
    //    ball.bounce(cv, megaman);
	for (int i = 0; i < bullet.size(); i++)
	{
		if (bullet[i].collidesWith(paddle, cv))
			bullet.erase(bullet.begin() + i);
	}

	//if (ball.collidesWith(paddle, cv))
	//	ball.bounce(cv, paddle);

	VECTOR2 cs;
	if (megaman.collidesWith(paddle, cs))
		megaman.stop(paddleNS::X, paddleNS::Y, paddleNS::WIDTH, paddleNS::HEIGHT);
}

//=============================================================================
// Render game items
//=============================================================================
void MegamanGame::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    backdrop.draw();                        // add the backdrop to the scene
    //ball.draw();                            // add the ball to the scene
	paddle.draw();							// add the paddle to the scene
    megaman.draw();							// add megaman to the scene
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
   // ballTexture.onLostDevice();             // ball texture
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
   // ballTexture.onResetDevice();
	paddleTexture.onResetDevice();
    megamanTexture.onResetDevice();
	bulletTexture.onResetDevice();
	bulletChargedSmallTexture.onResetDevice();

    Game::resetAll();
    return;
}
