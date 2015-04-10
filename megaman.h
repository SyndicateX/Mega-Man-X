
#ifndef megaman_h             // Prevent multiple definitions if this 
#define megaman_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace megamanNS
{
    const int WIDTH = 75;                  // image width
    const int HEIGHT = 100;                  // image height
    const int X = GAME_WIDTH/4 - WIDTH;     // location on screen
    const int Y = GAME_HEIGHT - HEIGHT * 2;
    const float SPEED = 300;                // pixels per second
	const float MASS = 1.0e6f;

	const int IDLE_MEGAMAN_START_FRAME = 17;
	const int IDLE_MEGAMAN_END_FRAME = 17;
	const float IDLE_MEGAMAN_ANIMATION_DELAY = 0.55f;

	const int WALKING_MEGAMAN_START_FRAME = 54;
	const int WALKING_MEGAMAN_END_FRAME = 67;
	const float WALKING_MEGAMAN_ANIMATION_DELAY = 0.06f;

	const int JUMPING_MEGAMAN_START_FRAME = 26;
	const int JUMPING_MEGAMAN_END_FRAME = 26;
	const float JUMPING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int JUMP_PEAK_MEGAMAN_START_FRAME = 27;
	const int JUMP_PEAK_MEGAMAN_END_FRAME = 27;
	const float JUMP_PEAK_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int FALLING_MEGAMAN_START_FRAME = 28;
	const int FALLING_MEGAMAN_END_FRAME = 28;
	const float FALLING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int SHOOTING_MEGAMAN_START_FRAME = 39;
	const int SHOOTING_MEGAMAN_END_FRAME = 40;
	const float SHOOTING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int SHOOTING_JUMP_MEGAMAN_START_FRAME = 84;
	const int SHOOTING_JUMP_MEGAMAN_END_FRAME = 84;
	const float SHOOTING_JUMP_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int SHOOTING_JUMP_PEAK_MEGAMAN_START_FRAME = 86;
	const int SHOOTING_JUMP_PEAK_MEGAMAN_END_FRAME = 86;
	const float SHOOTING_JUMP_PEAK_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int SHOOTING_FALLING_MEGAMAN_START_FRAME = 88;
	const int SHOOTING_FALLING_MEGAMAN_END_FRAME = 88;
	const float SHOOTING_FALLING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int DASHING_MEGAMAN_START_FRAME = 71;
	const int DASHING_MEGAMAN_END_FRAME = 71;
	const float DASHING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int WALL_SLIDING_MEGAMAN_START_FRAME = 158;
	const int WALL_SLIDING_MEGAMAN_END_FRAME = 158;
	const float WALL_SLIDING_MEGAMAN_ANIMATION_DELAY = 0.07f;
}

// inherits from Entity class
class Megaman : public Entity
{
public:
    // constructor
    Megaman();

    // inherited member functions
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	void update(float frameTime);
	virtual void draw();

	//In case megaman collides with a wall
	void Megaman::stop(int wallX, int wallY, int wallLength, int wallHeight);

	//bool canWallJump() { return wallJump_; }
	void setDoWallJump(bool doWallJump) { doWallJump_ = doWallJump; }
	bool canDash() { return canDash_; }
	void setCanDash(bool dashStatus) { canDash_ = dashStatus; }
	bool canJump() { return canJump_; }
	void setCanJump(bool jumpStatus) { canJump_ = jumpStatus; }
	bool canWallJump() { return canWallJump_; }
	void setCanWallJump(bool wallJumpStatus) { canWallJump_ = wallJumpStatus; }
	bool canShoot() { return canShoot_; }
	void setCanShoot(bool shootStatus) { canShoot_ = shootStatus; }
private:
	Image megamanIdle;
	Image megamanWalking;
	Image megamanJumping;
	Image megamanJumpPeak;
	Image megamanFalling;
	Image megamanShooting;
	Image megamanShootingJump;
	Image megamanShootingJumpPeak;
	Image megamanShootingFalling;
	Image megamanDashing;
	Image megamanWallSliding;
	SpriteCoordinates megamanSpriteCoordinates;// ("xcoords.txt");
	bool standingOnSurface_ = true;
	bool floorCollision_ = true;
	bool wallJump_ = false;
	bool doWallJump_ = false;
	bool canDash_ = false;
	bool canJump_ = false;
	bool canWallJump_ = false;
	bool canShoot_ = true;
};
#endif

