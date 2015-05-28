#include "enemy.h"

//=============================================================================
// default constructor
//=============================================================================
Enemy::Enemy() : Entity()
{
	spriteData.width = enemyNS::WIDTH;          // size of enemy
	spriteData.height = enemyNS::HEIGHT;
	spriteData.x = enemyNS::X;                  // location on screen
	spriteData.y = enemyNS::Y;
	spriteData.rect.bottom = enemyNS::HEIGHT;   // rectangle to select parts of an image
	spriteData.rect.right = enemyNS::WIDTH;
	spriteData.direction = RIGHT;				  // enemy always faces right at the start of any level
	spriteData.state = STANDING;					
	velocity.x = 0;						        // velocity X
	velocity.y = -20;							    // velocity Y
	frameDelay = 1.0;// 0.07;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	edge.top = -enemyNS::HEIGHT / 2;              // set collision edges
	edge.bottom = enemyNS::HEIGHT / 2;
	edge.left = -enemyNS::WIDTH / 2;
	edge.right = enemyNS::WIDTH / 2;
	collisionType = entityNS::BOX;
	mass = enemyNS::MASS;
	visible = true;
	active = true;
	dx = 0;
	dy = 0;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Enemy::update(float frameTime)
{
	Entity::update(frameTime);
	dy += frameTime * velocity.y * 2.5;
	velocity.y += frameTime * GRAVITY;              // gravity
	if (velocity.y >= TERMINAL_VELOCITY)
	{
		velocity.y = TERMINAL_VELOCITY;
	}
	enemyIdle.update(frameTime);
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

void Enemy::stop(int wallX, int wallY, int wallWidth, int wallHeight)
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

//=============================================================================
// Collision Handlers
// Helper functions for stop() functions. Resolve Megaman's collisions
// for each direction when Mega Man collides with a wall or floor.
//=============================================================================
void Enemy::topCollision(int wallY)
{
	spriteData.y = wallY - spriteData.height + 1;			// position at the top of the wall
	velocity.y = 0; 										// stop y acceleration
}
void Enemy::leftCollision(int wallX)
{
	spriteData.x = wallX - spriteData.width;			// position at the left of the wall
	//velocity.x = 0;
	//if (velocity.y > 0 && !input->isKeyDown(UP_KEY) && !input->getGamepadA(0))
	//{
	//		canWallJump_ = true;
	//}
}
void Enemy::rightCollision(int wallX, int wallWidth)
{
	spriteData.x = wallX + wallWidth + 1; //+1;			// position at the right of the wall
	//velocity.x = 0;
	//if (velocity.y > 0 && !input->isKeyDown(UP_KEY) && !input->getGamepadA(0))
	//{
	//		canWallJump_ = true;
	//}
}
void Enemy::bottomCollision(int wallY, int wallHeight)
{
	spriteData.y = wallY + wallHeight + 1;				// position underneath the wall
	velocity.y = 1;										// set velocity to make megaman fall
}






