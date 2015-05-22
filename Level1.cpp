#include "Level1.h"
using namespace level1NS;

Level1::Level1()
{
	mapX = 0;
	mapY = megaman.getY();
	oldX_ = 0;
	oldY_ = 0;
	directionChange_ = false;
}

Level1::~Level1()
{
	releaseAll();           // call onLostDevice() for every graphics item
}

void Level1::initializeAdditional(HWND& hwnd, Graphics* graphics, Input* input, Game* game)
{
	// backdrop texture
	if (!backdropTexture.initialize(graphics, BACKDROP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// enemy texture
	if (!mechaSonicTexture.initialize(graphics, ENEMY001))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// bee enemy texture
	if (!beeTexture.initialize(graphics, BEE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee texture"));

	// map textures
	if (!tileTextures.initialize(graphics, TILE_TEXTURES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

	// backdrop
	if (!backdrop.initialize(graphics, 0, 0, 0, &backdropTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// tile image
	if (!tile.initialize(graphics, TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COLS, &tileTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
	tile.setFrames(0, 0);
	tile.setCurrentFrame(0);

	// enemy
	if (!mechaSonic.initialize(game, enemyNS::WIDTH, enemyNS::HEIGHT, 0, &mechaSonicTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

	// bee enemy
	if (!bee.initialize(game, enemyNS::WIDTH, enemyNS::HEIGHT, 0, &beeTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee"));

	// floors
	for (int i = 0; i < TILE_ROWS; i++)
	{
		for (int j = 0; j < TILE_COLUMNS; j++)
		{
			if (tileMap[i][j] >= 0)
			{
				floor.push_back(Entity());
				if (!floor[floor.size() - 1].initialize(game, TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COLS, &tileTextures))
					throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing floor"));
				floor[floor.size() - 1].setCollisionType(entityNS::BOX);
				floor[floor.size() - 1].setEdge(MEGAMAN_EDGE);
				// Set floor position
				floor[floor.size() - 1].setX(j*TEXTURE_SIZE);
				floor[floor.size() - 1].setY(i*TEXTURE_SIZE);
			}
		}
	}
}

void Level1::update(float frameTime, Input* input, Game* game)
{
	// Handles megaman's input and actions
	updateMegaman(MAP_WIDTH, MAP_HEIGHT, frameTime, input, game);
	bee.update(frameTime);

	// Level specific code goes here
}

void Level1::ai()
{

}

void Level1::collisions(float frameTime)
{
	VECTOR2 cv;
	std::vector<VECTOR2> collisionVector;		// Centers of collision between a wall and Mega Man
	std::vector<RECT> tileCoordinates;			// Coordinates of all tiles that collided with Mega Man
	bool megamanCollided = false;				// True if Mega Man collides with any tile
	bool beeCollided = false;

	for (int i = 0; i < floor.size(); i++)
	{
		for (int j = 0; j < bullet.size(); j++)
		{
			if (bullet[j].collidesWith(floor[i], cv))
			{
				bullet[j].setVisible(false);
				bullet[j].setActive(false);
			}
		}
		if (megaman.collidesWith(floor[i], cv)) // 
		{
			collisionVector.push_back(cv);
			tileCoordinates.push_back(RECT());
			tileCoordinates[tileCoordinates.size() - 1].top = floor[i].getY();
			tileCoordinates[tileCoordinates.size() - 1].left = floor[i].getX();
			tileCoordinates[tileCoordinates.size() - 1].bottom = floor[i].getY() + floor[i].getHeight();
			tileCoordinates[tileCoordinates.size() - 1].right = floor[i].getX() + floor[i].getWidth();
			megamanCollided = true;
			//megaman.stop(floor[i].getX(), floor[i].getY(), floor[i].getWidth(), floor[i].getHeight());
		}
		if (bee.collidesWith(floor[i], cv))
		{
			if (!beeCollided)
			{
				bee.stop(floor[i].getX(), floor[i].getWidth());
				beeCollided = true;
			}

		}
	}

	for (int j = 0; j < bullet.size(); j++)
	{
		if (bullet[j].collidesWith(bee, cv))
		{
			// bee takes damage
			bee.setActive(false);
			bee.setState(DEAD);
		}
	}

	if (megamanCollided)
	{
		megaman.stop(collisionVector, tileCoordinates);		// Sets Mega Man's position and status after a collision
	}
}

void Level1::render(Graphics* graphics)
{
	graphics->spriteBegin();                // begin drawing sprites

	mapY += megaman.getY() - oldY_;			// update map coordinates
	mapX += megaman.getX() - oldX_;			//

	megaman.setY(oldY_);					// reset Mega Man's y-coordinate to his previous y-coordinate (keeps him centered on the screen)
	backdrop.setY(-mapY + megamanNS::Y);
	bee.setY(beeNS::Y - mapY + megamanNS::Y);

	if (mapX >= 0 && mapX <= TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)	// if Mega Man is not near and edge of the map on either end
	{
		backdrop.setX(-mapX);
		bee.setX(beeNS::X + bee.getDx() - mapX);
		megaman.setX(oldX_);				// reset Mega Man's x-coordinate to his previous x-coordinate (keeps him centered on the screen)
	}
	else if (mapX < 0)													// if Mega Man is near the left edge of the map
	{
		bee.setX(beeNS::X + bee.getDx());
	}
	else																// if Mega Man is near the right edge of the map
	{
		bee.setX(beeNS::X + bee.getDx() - (TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH));
	}

	int counter = 0;
	for (int i = 0; i < TILE_ROWS; i++)					// Moving tiles based on the map's movements
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
					floor[counter].setX(j*TEXTURE_SIZE - (TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)); // + TEXTURE_SIZE * TILE_ROWS + GAME_WIDTH);
				}
				floor[counter].setY(i*TEXTURE_SIZE - mapY + megamanNS::Y);
				counter++;
			}
		}
	}

	backdrop.draw();                        // add the backdrop to the scene

	for (int i = 0; i < bullet.size(); i++)			// Moving bullets based on the map's movements
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
	megaman.draw();							// add Mega Man to the scene
	chargingSprites.draw();					// add Mega Man's charging sprites to the scene

	if (bee.getVisible())
	{
		bee.draw();								// add bee enemy to the scene
	}
	for (int i = 0; i < bullet.size(); i++)
	{
		if (bullet[i].getVisible() && bullet[i].getActive())
			bullet[i].draw();					// add bullets to the scene
	}
	graphics->spriteEnd();                  // end drawing sprites
}

void Level1::releaseAll()
{
	mechaSonicTexture.onLostDevice();

	megamanTexture.onLostDevice();          // megaman texture
	bulletTexture.onLostDevice();			// bullet texture
	bulletTexture.onLostDevice();			// bullet texture
	chargingSpritesTexture.onLostDevice();
	backdropTexture.onLostDevice();         // backdrop texture
	tileTextures.onLostDevice();
	beeTexture.onLostDevice();
	explosionTexture.onLostDevice();

	//Game::releaseAll();
	return;
}

void Level1::resetAll()
{
	mechaSonicTexture.onResetDevice();

	backdropTexture.onResetDevice();
	tileTextures.onResetDevice();
	megamanTexture.onResetDevice();
	chargingSpritesTexture.onResetDevice();
	bulletTexture.onResetDevice();
	bulletTexture.onResetDevice();
	beeTexture.onResetDevice();
	explosionTexture.onResetDevice();

	//Game::resetAll();
	return;
}