#ifndef enemy_h             // Prevent multiple definitions if this 
#define enemy_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace enemyNS
{
	const int REGULAR_SHOT_DAMAGE = 3;
	const int SMALL_CHARGE_DAMAGE = 6;
	const int MEDIUM_CHARGE_DAMAGE = 8;
	const float SPEED_BOWSER = 250.0F;
}

// inherits from Entity class
class Enemy : public Entity
{
public:
	// constructor
	Enemy();

	//inherited member functions
	virtual bool initialize(Game * gamePtr, int width, int height, int ncols,
		TextureManager * textureM) = 0;
	//std::string spriteCoordinatesFileName);
	virtual void update(float frameTime) = 0;
	void gravity(float frameTime);
	virtual void draw() = 0;

	// damage Mega Man with WEAPON
	void damage(WEAPON);

	void stop(double wallX, double wallY, double wallWidth, double wallHeight);
	void stop(double wallX, double wallWidth);
	void stop(std::vector<VECTOR2> collisionVector, std::vector<RECT> tileCoordinates);
	virtual void handleCollisions(double wallX, double wallY, double wallWidth, double wallHeight) = 0;

	virtual void setStartX(double startX) = 0;
	virtual void setStartY(double startY) = 0;

	double getStartX()
	{
		return startX_;
	}
	double getStartY()
	{
		return startY_;
	}
	double getDx()
	{
		return dx;
	}
	double getDy()
	{
		return dy;
	}
	bool isBoss()
	{
		return boss;
	}
	bool attackReady()
	{
		return attackTimer <= 0;
	}
	void setAttackDelay(float delayTime)
	{
		attackTimer = delayTime;
	}
	bool isInvincible() const { return isInvincible_; }

	void setFloorCollision(bool floorCollisionStatus) { floorCollision_ = floorCollisionStatus; }
	bool getFloorCollision() { return floorCollision_; }

protected:
	SpriteCoordinates spriteCoordinates;

	double startX_;
	double startY_;
	double dx;
	double dy;
	bool boss;

	bool gravity_;

	bool floorCollision_ = false;
	bool isInvincible_ = false;
	bool canAttack_ = true;

	float invincibleTimer;                  // time remaining until Mega Man can take damage again
	float attackTimer;

	int flicker;

	void topCollision(double wallY);
	void leftCollision(double wallX);
	void rightCollision(double wallX, double wallWidth);
	void bottomCollision(double wallY, double wallHeight);
	
};

#endif

