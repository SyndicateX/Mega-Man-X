#include "enemy.h"

//=============================================================================
// default constructor
//=============================================================================
Enemy::Enemy() : Entity()
{
	spriteData.x = startX_;                  // location on screen
	spriteData.y = startY_;
	spriteData.direction = RIGHT;				  // enemy always faces right at the start of any level
	spriteData.state = STANDING;					
	velocity.x = 0;						        // velocity X
	velocity.y = -20;							    // velocity Y
	frameDelay = 1.0;// 0.07;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	collisionType = entityNS::BOX;
	visible = true;
	active = true;
	health = 100;
	dx = 0;
	dy = 0;
	boss = false;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Enemy::gravity(float frameTime)
{
	Entity::update(frameTime);
	dy += frameTime * velocity.y * 2.5;
	velocity.y += frameTime * GRAVITY;              // gravity
	if (velocity.y >= TERMINAL_VELOCITY)
	{
		velocity.y = TERMINAL_VELOCITY;
	}
}

//=============================================================================
// damage
//=============================================================================
void Enemy::damage(WEAPON weapon)
{
	spriteData.state = DAMAGED;
	//damageTimer = DAMAGE_TIME;
	velocity.y = 0;

	switch (weapon)
	{
	case UNCHARGED_BULLET:
		audio->playCue(EXPLODE);	//play sound
		health -= enemyNS::REGULAR_SHOT_DAMAGE;
		break;
	case SMALL_CHARGE_BULLET:
		audio->playCue(EXPLODE);    // play sound
		health -= enemyNS::SMALL_CHARGE_DAMAGE;
		break;
	case MEDIUM_CHARGE_BULLET:
		audio->playCue(EXPLODE);    // play sound
		health -= enemyNS::MEDIUM_CHARGE_DAMAGE;
		break;
	}
	if (health <= 0)
	{
		spriteData.state = DEAD;
		active = false;
	}
}

//=============================================================================
// stop
// Collision detection between an Enemy and solid surfaces
//=============================================================================
void Enemy::stop(std::vector<VECTOR2> collisionVector, std::vector<RECT> tileCoordinates)
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

void Enemy::stop(double wallX, double wallY, double wallWidth, double wallHeight)
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

void Enemy::stop(double wallX, double wallWidth)
{
	if (spriteData.x < wallX)
	{
		spriteData.direction = LEFT;
		flipHorizontal(true);
		dx -= spriteData.x +spriteData.width - wallX + 5;
	}
	else if (wallX + wallWidth > spriteData.x)
	{
		spriteData.direction = RIGHT;
		flipHorizontal(false);
		dx += wallX + wallWidth - spriteData.x + 5;
	}
}

//=============================================================================
// Collision Handlers
// Helper functions for stop() functions. Resolve Megaman's collisions
// for each direction when Mega Man collides with a wall or floor.
//=============================================================================
void Enemy::topCollision(double wallY)
{
	spriteData.y = wallY - spriteData.height + 1;			// position at the top of the wall
	velocity.y = 0; 										// stop y acceleration
	floorCollision_ = true;
}
void Enemy::leftCollision(double wallX)
{
	spriteData.x = wallX - spriteData.width;			// position at the left of the wall
}
void Enemy::rightCollision(double wallX, double wallWidth)
{
	spriteData.x = wallX + wallWidth + 1; //+1;			// position at the right of the wall
}
void Enemy::bottomCollision(double wallY, double wallHeight)
{
	spriteData.y = wallY + wallHeight + 1;				// position underneath the wall
	velocity.y = 1;										// set velocity to make megaman fall
}






