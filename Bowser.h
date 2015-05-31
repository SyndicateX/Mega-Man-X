#ifndef bowser_h               // Prevent multiple definitions if this 
#define bowser_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "enemy.h"
#include "constants.h"
#include "sprites.h"

namespace bowserNS
{
	const int WIDTH = 200;                   // image width
	const int HEIGHT = 200;                  // image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 250;                // pixels per second
	const float ENERGY_LOSS = 100;          // energy loss on bounce (pixels/sec)
	const float MIN_VY = 1;                 // minumum Y velocity
	const float MASS = 1.0f;

	const int IDLE_START_FRAME = 40;
	const int IDLE_END_FRAME = 45;
	const float IDLE_ANIMATION_DELAY = 0.15f;

	const int DYING_START_FRAME = 54;
	const int DYING_END_FRAME = 55;
	const float DYING_ANIMATION_DELAY = 0.10f;
}

// inherits from Entity class
class Bowser : public Enemy
{
public:
	// constructor
	Bowser();

	// inherited member functions
	void update(float frameTime);
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	void handleCollisions(double wallX, double wallY, double wallWidth, double wallHeight);
	void draw();

	void setStartX(double startX)
	{
		startX_ = startX - 100;
	}
	void setStartY(double startY)
	{
		startY_ = startY - 200;
	}

private:
	SpriteCoordinates bowserSpriteCoordinates;
	Image bowserIdle;
	Image bowserDying;
};
#endif