#ifndef megaman_h             // Prevent multiple definitions if this 
#define megaman_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace megamanNS
{
    const int WIDTH = 75;						// image width
    const int HEIGHT = 100;						// image height
	const int X = GAME_WIDTH / 2;			    // location on screen
    const int Y = GAME_HEIGHT - 4 * HEIGHT;
    const float SPEED = 125;					// pixels per second
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

	//const int SHOOTING_WALKING_MEGAMAN_START_FRAME = 100;
	//const int SHOOTING_WALKING_MEGAMAN_END_FRAME = 128;
	//const float SHOOTING_WALKING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int DASHING_MEGAMAN_START_FRAME = 71;
	const int DASHING_MEGAMAN_END_FRAME = 71;
	const float DASHING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int SHOOTING_DASHING_MEGAMAN_START_FRAME = 136;
	const int SHOOTING_DASHING_MEGAMAN_END_FRAME = 136;
	const float SHOOTING_DASHING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int WALL_SLIDING_MEGAMAN_START_FRAME = 158;
	const int WALL_SLIDING_MEGAMAN_END_FRAME = 158;
	const float WALL_SLIDING_MEGAMAN_ANIMATION_DELAY = 0.07f;

	const int DAMAGED_MEGAMAN_START_FRAME = 179;
	const int DAMAGED_MEGAMAN_END_FRAME = 182;
	const float DAMAGED_MEGAMAN_ANIMATION_DELAY = 0.1f;
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

	// Determines where to place megaman if he collides with several blocks at once. 
	// Calls the other stop() function to deal with single block collision scenarios
	void stop(std::vector<VECTOR2> collisionVector, std::vector<RECT> tileCoordinates);
	// Determines where to place megaman if he collides with a single block
	void stop(double wallX, double wallY, double wallWidth, double wallHeight);
	void topCollision(double wallY);
	void leftCollision(double wallX);
	void rightCollision(double wallX, double wallWidth);
	void bottomCollision(double wallY, double wallHeight);

	// Set Functions
	void setDoWallJump(bool doWallJump) { doWallJump_ = doWallJump; }
	void setCanDash(bool dashStatus) { canDash_ = dashStatus; }
	void setIsDashing(bool dashStatus) { isDashing_ = dashStatus; }
	void setCanJump(bool jumpStatus) { canJump_ = jumpStatus; }
	void setCanWallJump(bool wallJumpStatus) { canWallJump_ = wallJumpStatus; }
	void setCanShoot(bool shootStatus) { canShoot_ = shootStatus; }
	void setIsDashJumping(bool dashJumpStatus) { isDashJumping_ = dashJumpStatus; }
	void setFloorCollision(bool floorCollisionStatus) { floorCollision_ = floorCollisionStatus; }
	void setDamageTimer(float damageTime) { damageTimer = damageTime; }
	void setInvincible(bool invincibleStatus) { isInvincible_ = invincibleStatus; }

	// Get Functions
	bool canDash() const { return canDash_; }
	bool isDashing() const { return isDashing_; }
	bool canJump() const { return canJump_; }
	bool canWallJump() const { return canWallJump_; }
	bool canShoot() const { return canShoot_; }
	bool isDashJumping() const { return isDashJumping_; }
	bool isInvincible() const { return isInvincible_; }

private:
	SpriteCoordinates megamanSpriteCoordinates;
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
	Image megamanShootingDashing;
	Image megamanWallSliding;
	Image megamanDamaged;
	//Image megamanShootingWalking;		// Needs to modify sprite sheet for this to work
										// Could possible work by modifying currentFrame when updating frames

	bool standingOnSurface_ = true;
	bool floorCollision_ = false;
	bool wallJump_ = false;
	bool doWallJump_ = false;
	bool canDash_ = false;
	bool isDashing_ = false;
	bool canJump_ = false;
	bool canWallJump_ = false;
	bool canShoot_ = true;
	bool isDashJumping_ = false;
	bool isInvincible_ = false;

	float damageTimer;                  // time remaining until user can control Mega Man
	float invincibleTimer;                  // time remaining until Mega Man can take damage again

	int flicker;
};
#endif

