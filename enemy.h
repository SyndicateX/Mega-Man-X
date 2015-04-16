#ifndef enemy_h             // Prevent multiple definitions if this 
#define enemy_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace enemyNS
{
    const int WIDTH = 88;                  // image width
    const int HEIGHT = 121;                  // image height
    const int X = GAME_WIDTH/2 + WIDTH * 3;     // location on screen
    const int Y = GAME_HEIGHT / 2;
    const float SPEED = 300;                // pixels per second
	const float MASS = 1.0e6f;

	const int IDLE_ENEMY_START_FRAME = 0;
	const int IDLE_ENEMY_END_FRAME = 9;
	const float IDLE_ENEMY_ANIMATION_DELAY = 0.55f;

}

// inherits from Entity class
class Enemy : public Entity
{
public:
	// constructor
	Enemy();

	//inherited member functions
	virtual bool initialize(Game * gamePtr, int width, int height, int ncols,
		TextureManager * textureM);
	void update(float framTime);
	virtual void draw();
	void stop(int wallX, int wallY, int wallLength, int wallHeight);

private:
	Image enemyIdle;

};

//// inherits from Entity class
//class Enemy : public Entity
//{
//public:
//    // constructor
//    Enemy();
//
//    // inherited member functions
//	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
//		TextureManager *textureM);
//	void update(float frameTime);
//	virtual void draw();
//
//	// In case enemy collides with a wall
//	void Enemy::stop(int wallX, int wallY, int wallLength, int wallHeight);
//
//	// Set Functions
//	void setDoWallJump(bool doWallJump) { doWallJump_ = doWallJump; }
//	void setCanDash(bool dashStatus) { canDash_ = dashStatus; }
//	void setIsDashing(bool dashStatus) { isDashing_ = dashStatus; }
//	void setCanJump(bool jumpStatus) { canJump_ = jumpStatus; }
//	void setCanWallJump(bool wallJumpStatus) { canWallJump_ = wallJumpStatus; }
//	void setCanShoot(bool shootStatus) { canShoot_ = shootStatus; }
//	void setIsDashJumping(bool dashJumpStatus) { isDashJumping_ = dashJumpStatus; }
//
//	// Get Functions
//	bool canDash() { return canDash_; }
//	bool isDashing() { return isDashing_; }
//	bool canJump() { return canJump_; }
//	bool canWallJump() { return canWallJump_; }
//	bool canShoot() { return canShoot_; }
//	bool isDashJumping() { return isDashJumping_; }
//
//
//private:
//	Image enemyIdle;
//	Image enemyWalking;
//	Image enemyJumping;
//	Image enemyJumpPeak;
//	Image enemyFalling;
//	Image enemyShooting;
//	Image enemyShootingJump;
//	Image enemyShootingJumpPeak;
//	Image enemyShootingFalling;
//	Image enemyDashing;
//	Image enemyWallSliding;
//	SpriteCoordinates enemySpriteCoordinates;
//	bool standingOnSurface_ = true;
//	bool floorCollision_ = true;
//	bool wallJump_ = false;
//	bool doWallJump_ = false;
//	bool canDash_ = false;
//	bool isDashing_ = false;
//	bool canJump_ = false;
//	bool canWallJump_ = false;
//	bool canShoot_ = true;
//	bool isDashJumping_ = false;
//
//	//bool charge1_ = false;
//};
#endif

