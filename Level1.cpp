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
	if (!backdropTexture.initialize(graphics,BACKDROP_IMAGE))
	    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// enemy texture
	if (!mechaSonicTexture.initialize(graphics, ENEMY001))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing megaman texture"));

	// map textures
	if (!tileTextures.initialize(graphics, TILE_TEXTURES))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile textures"));

	// backdrop
	if (!backdrop.initialize(graphics,0,0,0,&backdropTexture))
	    throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// tile image
	if (!tile.initialize(graphics, TEXTURE_SIZE, TEXTURE_SIZE, TEXTURE_COLS, &tileTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tile"));
	tile.setFrames(0, 0);
	tile.setCurrentFrame(0);

	// enemy
	if (!mechaSonic.initialize(game, enemyNS::WIDTH, enemyNS::HEIGHT, 0, &mechaSonicTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

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

	// Level specific code goes here
}

void Level1::ai()
{

}

void Level1::collisions()
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
}

void Level1::render(Graphics* graphics)
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

void Level1::releaseAll()
{
	mechaSonicTexture.onLostDevice();

	megamanTexture.onLostDevice();          // megaman texture
	bulletTexture.onLostDevice();			// bullet texture
	bulletTexture.onLostDevice();			// bullet texture
	chargingSpritesTexture.onLostDevice();
	backdropTexture.onLostDevice();         // backdrop texture
	tileTextures.onLostDevice();

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

	//Game::resetAll();
	return;
}