#ifndef enemy_h             // Prevent multiple definitions if this 
#define enemy_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace enemyNS
{

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
	void stop(int wallX, int wallY, int wallLength, int wallHeight);
	void stop(double wallX, double wallWidth);
	void stop(std::vector<VECTOR2> collisionVector, std::vector<RECT> tileCoordinates);
	virtual void handleCollisions(int wallX, int wallY, int wallLength, int wallHeight) = 0;

	void setStartX(double startX)
	{
		startX_ = startX;
	}
	void setStartY(double startY)
	{
		startY_ = startY;
	}

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

protected:
	SpriteCoordinates spriteCoordinates;

	double startX_;
	double startY_;
	double dx;
	double dy;

	bool gravity_;

	void topCollision(int wallY);
	void leftCollision(int wallX);
	void rightCollision(int wallX, int wallWidth);
	void bottomCollision(int wallY, int wallHeight);
	
};

#endif

