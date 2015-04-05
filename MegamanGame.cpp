
#include "MegamanGame.h"
#include "sprites.h"

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

    // ball texture
    if (!ballTexture.initialize(graphics,BALL_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball texture"));

    // megaman texture
    if (!megamanTexture.initialize(graphics,MEGAMAN_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// megaman texture
	if (!megamanTexture.initialize(graphics, MEGAMAN_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// paddle texture
	if (!paddleTexture.initialize(graphics, PADDLE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle texture"));

    // backdrop
    if (!backdrop.initialize(graphics,0,0,0,&backdropTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

    // ball
    if (!ball.initialize(this,ballNS::WIDTH,ballNS::HEIGHT,0,&ballTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ball"));
    ball.setX(GAME_WIDTH/4 - ballNS::WIDTH);
    ball.setY(GAME_HEIGHT/2 - ballNS::HEIGHT);
    ball.setVelocity(VECTOR2(ballNS::SPEED,-ballNS::SPEED)); // VECTOR2(X, Y)

	// paddle
	if (!paddle.initialize(this, paddleNS::WIDTH, paddleNS::HEIGHT, 0, &paddleTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing paddle"));

    // megaman
	if (!megaman.initialize(this, megamanNS::WIDTH, megamanNS::HEIGHT, 0, &megamanTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));

	// megaman sprite initialize
	SpriteCoordinates megamanSpriteCoordinates("xcoords.txt");
	if (!megaman.initialize(megamanSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman"));

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void MegamanGame::update()
{
	//Both state and direction must be initialized
	State state = STANDING;
	static Direction direction = RIGHT; //Static because direction persists

	if ((input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0)) && input->getGamepadX(0))			//Running Left
	{
		megaman.setX(megaman.getX() - megamanNS::SPEED*frameTime * 1.4);
		state = WALKING;
		direction = LEFT;
		megaman.setFrames(56, 68);
	}
	else if ((input->isKeyDown(RIGHT_KEY) || input->getGamepadDPadRight(0)) && input->getGamepadX(0))	//Running Right
	{
		megaman.setX(megaman.getX() + megamanNS::SPEED*frameTime * 1.4);
		state = WALKING;
		direction = RIGHT;
		megaman.setFrames(56, 68);
	}
	else if (input->isKeyDown(LEFT_KEY) || input->getGamepadDPadLeft(0))
	{
		megaman.setX(megaman.getX() - megamanNS::SPEED*frameTime);
		state = WALKING;
		direction = LEFT;
		megaman.setFrames(54, 68);
	}
	else if (input->isKeyDown(RIGHT_KEY) || input->getGamepadDPadRight(0))
	{
		megaman.setX(megaman.getX() + megamanNS::SPEED*frameTime);
		state = WALKING;
		direction = RIGHT;
		megaman.setFrames(54, 67);
	}
	//else if (input->isKeyDown(DOWN_KEY) || input->getGamepadDPadDown(0))
	//{
	//	state = CROUCHING;
	//	megaman.setCurrentFrame(6);
	//	megaman.setFrames(6, 6);
	//}
	else
	{
		megaman.setCurrentFrame(17);
		megaman.setFrames(17, 17);
	}
	
	if (input->isKeyDown(UP_KEY) || input->getGamepadA(0))
	{
		megaman.setCurrentFrame(27);
		state = JUMPING;
	}

    ball.update(frameTime);
	paddle.update(frameTime);
    megaman.update(frameTime, state, direction);
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
    if(ball.collidesWith(megaman, cv))
        ball.bounce(cv, megaman);

	if (ball.collidesWith(paddle, cv))
		ball.bounce(cv, paddle);

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
    ball.draw();                            // add the ball to the scene
	paddle.draw();							// add the paddle to the scene
    megaman.draw();   // add megaman to the scene
    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void MegamanGame::releaseAll()
{
    megamanTexture.onLostDevice();            // megaman texture
	paddleTexture.onLostDevice();
    ballTexture.onLostDevice();             // ball texture
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
    ballTexture.onResetDevice();
	paddleTexture.onResetDevice();
    megamanTexture.onResetDevice();

    Game::resetAll();
    return;
}
