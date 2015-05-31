#include "Level2.h"
using namespace level2NS;

Level2::Level2()
{
	mapX = 0;
	mapY = megaman.getY();
	oldX_ = 0;
	oldY_ = 0;
	directionChange_ = false;
	fightingBoss = false;
}

Level2::~Level2()
{
	releaseAll();           // call onLostDevice() for every graphics item
}

void Level2::initializeAdditional(HWND& hwnd, Graphics* graphics, Input* input, Game* game)
{
	// backdrop texture
	if (!backdropTexture.initialize(graphics, BACKDROP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background texture"));

	// mecha sonic texture
	if (!mechaSonicTexture.initialize(graphics, MECHA_SONIC))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic texture"));

	// bee enemy texture
	if (!beeTexture.initialize(graphics, BEE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee texture"));

	// bowser enemy texture
	if (!bowserTexture.initialize(graphics, BOWSER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser texture"));

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

	// floors
	for (int i = 0; i < TILE_ROWS; i++)
	{
		for (int j = 0; j < TILE_COLUMNS; j++)
		{
			if (tileMap[i][j] >= 0 && tileMap[i][j] < 100)
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
			else if (tileMap[i][j] >= 100)
			{
				if (tileMap[i][j] == E1)
				{
					enemy.push_back(new MechaSonic());
					if (!enemy[enemy.size() - 1]->initialize(game, mechaSonicNS::WIDTH, mechaSonicNS::HEIGHT, 0, &mechaSonicTexture))
						throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));
				}
				else if (tileMap[i][j] == E2)
				{
					enemy.push_back(new Bee());
					if (!enemy[enemy.size() - 1]->initialize(game, beeNS::WIDTH, beeNS::HEIGHT, 0, &beeTexture))
						throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bee"));
				}
				else if (tileMap[i][j] == B1)
				{
					enemy.push_back(new Bowser());
					if (!enemy[enemy.size() - 1]->initialize(game, bowserNS::WIDTH, bowserNS::HEIGHT, 0, &bowserTexture))
						throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bowser"));
				}
				enemy[enemy.size() - 1]->setStartX(j*TEXTURE_SIZE);
				enemy[enemy.size() - 1]->setStartY(i*TEXTURE_SIZE);
				bossIndex = enemy.size() - 1;
			}
		}
	}
}

void Level2::update(float frameTime, Input* input, Game* game)
{
	oldY_ = megaman.getY();
	oldX_ = megaman.getX();

	// Handles megaman's input and actions
	updateMegaman(MAP_WIDTH, MAP_HEIGHT, frameTime, input, game);
	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy[i]->getVisible())
		{
			enemy[i]->update(frameTime);
		}
	}

	if (mapX > MAP_WIDTH - 5 * TEXTURE_SIZE && mapY > TEXTURE_SIZE * TILE_ROWS - 2 * TEXTURE_SIZE && !fightingBoss)
	{
		fightingBoss = true;
		/*if (mapX < MAP_WIDTH - TEXTURE_SIZE / 2 || mapY < TEXTURE_SIZE * TILE_ROWS - GAME_HEIGHT / 2 - TEXTURE_SIZE)
		{
		mapX++;
		mapY++;
		}*/
		for (int i = 0; i < enemy.size(); i++)
		{
			if (enemy[i]->isBoss())
			{
				enemy[i]->setActive(true);
				enemy[i]->setVisible(true);
			}
			else
			{
				enemy[i]->setActive(false);
				enemy[i]->setVisible(false);
			}
		}
	}
	if (fightingBoss && enemy[bossIndex]->getState() == DEAD)
	{
		levelComplete_ = true;
	}
	// Level specific code goes here
}

void Level2::ai()
{

}

void Level2::collisions(float frameTime)
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
		}

		for (int j = 0; j < enemy.size(); j++)
		{
			if (enemy[j]->collidesWith(floor[i], cv))
			{
				enemy[j]->handleCollisions(floor[i].getX(), floor[i].getY(), floor[i].getWidth(), floor[i].getHeight());
			}
			if (enemy[j]->collidesWith(megaman, cv) && !megaman.isInvincible() && megaman.getState() != DAMAGED)
			{
				megaman.setState(DAMAGED);
				megaman.setDamageTimer(DAMAGE_TIME);
				megaman.setVelocity(VECTOR2(megaman.getVelocity().x, 0));
			}
		}
	}

	for (int i = 0; i < bullet.size(); i++)
	{
		for (int j = 0; j < enemy.size(); j++)
		{
			if (bullet[i].collidesWith(*enemy[j], cv))
			{
				enemy[j]->setActive(false);
				enemy[j]->setState(DEAD);
				bullet[i].setActive(false);
				bullet[i].setVisible(false);
			}
		}
	}

	if (megamanCollided)
	{
		megaman.stop(collisionVector, tileCoordinates);		// Sets Mega Man's position and status after a collision
	}
}

void Level2::updateMap()
{
	if (fightingBoss)
	{
		mapX = MAP_WIDTH - TEXTURE_SIZE / 2;
		mapY = TEXTURE_SIZE * TILE_ROWS - GAME_HEIGHT / 2 - TEXTURE_SIZE;
	}
	else
	{
		mapY += megaman.getY() - oldY_;			// update map coordinates
		mapX += megaman.getX() - oldX_;			//
		megaman.setY(oldY_);					// reset Mega Man's y-coordinate to his previous y-coordinate (keeps him centered on the screen)

		for (int i = 0; i < bullet.size(); i++)			// Moving bullets based on the map's movements
		{
			bullet[i].setY(bullet[i].getInitialY() - mapY + megamanNS::Y);
		}

		for (int i = 0; i < enemy.size(); i++)
		{
			enemy[i]->setY(enemy[i]->getStartY() + megamanNS::Y - mapY + enemy[i]->getDy());
		}

		if (mapX >= 0 && mapX <= TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)	// if Mega Man is not near an edge of the map on either end
		{
			backdrop.setX(-mapX / 20);
			megaman.setX(oldX_);				// reset Mega Man's x-coordinate to his previous x-coordinate (keeps him centered on the screen)
		}
	}

	backdrop.setY(-mapY / 10 + megamanNS::Y - 200);

	for (int i = 0; i < enemy.size(); i++)
	{
		enemy[i]->setY(enemy[i]->getStartY() + megamanNS::Y - mapY + enemy[i]->getDy());
	}

	if (mapX >= 0 && mapX <= TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH)	// if Mega Man is not near an edge of the map on either end
	{
		for (int i = 0; i < enemy.size(); i++)
		{
			enemy[i]->setX(enemy[i]->getStartX() + enemy[i]->getDx() - mapX);
		}
	}
	else if (mapX < 0)													// if Mega Man is near the left edge of the map
	{
		for (int i = 0; i < enemy.size(); i++)
		{
			enemy[i]->setX(enemy[i]->getStartX() + enemy[i]->getDx());
		}
	}
	else																// if Mega Man is near the right edge of the map
	{
		for (int i = 0; i < enemy.size(); i++)
		{
			enemy[i]->setX(enemy[i]->getStartX() - (TEXTURE_SIZE * TILE_COLUMNS - GAME_WIDTH));
		}
	}

	int counter = 0;
	for (int i = 0; i < TILE_ROWS; i++)					// Moving tiles based on the map's movements
	{
		for (int j = 0; j < TILE_COLUMNS; j++)
		{
			if (tileMap[i][j] >= 0 && tileMap[i][j] < 100)
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
}

void Level2::render(Graphics* graphics)
{
	graphics->spriteBegin();                // begin drawing sprites

	backdrop.draw();                        // add the backdrop to the scene

	for (int row = 0; row<TILE_ROWS; row++)       // for each row of map
	{
		tile.setY((float)(row*TEXTURE_SIZE - mapY + megamanNS::Y)); // set tile Y
		for (int col = 0; col<TILE_COLUMNS; col++)    // for each column of map
		{
			if (tileMap[row][col] >= 0 && tileMap[row][col] < 100)          // if tile present
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
	for (int i = 0; i < enemy.size(); i++)
	{
		if (enemy[i]->getVisible())
		{
			enemy[i]->draw();
		}
	}

	megaman.draw();							// add Mega Man to the scene
	chargingSprites.draw();					// add Mega Man's charging sprites to the scene

	for (int i = 0; i < bullet.size(); i++)
	{
		if (bullet[i].getVisible() && bullet[i].getActive())
			bullet[i].draw();					// add bullets to the scene
	}

	graphics->spriteEnd();                  // end drawing sprites
}

void Level2::releaseAll()
{
	mechaSonicTexture.onLostDevice();
	megamanTexture.onLostDevice();          // megaman texture
	bulletTexture.onLostDevice();			// bullet texture
	mechaSonicTexture.onLostDevice();		// bullet texture
	chargingSpritesTexture.onLostDevice();
	backdropTexture.onLostDevice();         // backdrop texture
	tileTextures.onLostDevice();
	beeTexture.onLostDevice();
	bowserTexture.onLostDevice();

	//Game::releaseAll();
	return;
}

void Level2::resetAll()
{
	mechaSonicTexture.onResetDevice();
	backdropTexture.onResetDevice();
	tileTextures.onResetDevice();
	megamanTexture.onResetDevice();
	chargingSpritesTexture.onResetDevice();
	bulletTexture.onResetDevice();
	mechaSonicTexture.onResetDevice();
	beeTexture.onResetDevice();
	bowserTexture.onResetDevice();

	//Game::resetAll();
	return;
}