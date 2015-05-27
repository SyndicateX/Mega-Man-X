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
	void stop(std::vector<VECTOR2> collisionVector, std::vector<RECT> tileCoordinates);

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

private:
	Image enemyIdle;
	SpriteCoordinates mechaSonicSpriteCoordinates;

	double startX_;
	double startY_;
	double dx;
	double dy;

	void topCollision(int wallY);
	void leftCollision(int wallX);
	void rightCollision(int wallX, int wallWidth);
	void bottomCollision(int wallY, int wallHeight);
	
};

#endif

