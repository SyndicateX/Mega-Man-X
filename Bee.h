#ifndef bee_h               // Prevent multiple definitions if this 
#define bee_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "enemy.h"
#include "constants.h"
#include "sprites.h"

namespace beeNS
{
	const int WIDTH = 87;                   // image width
	const int HEIGHT = 114;                  // image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 250;                // pixels per second
	const float ENERGY_LOSS = 100;          // energy loss on bounce (pixels/sec)
	const float MIN_VY = 1;                 // minumum Y velocity
	const float MASS = 1.0f;

	const int FLYING_BEE_START_FRAME = 49;
	const int FLYING_BEE_END_FRAME = 50;
	const float FLYING_BEE_ANIMATION_DELAY = 0.05f;

	const int DYING_BEE_START_FRAME = 152;
	const int DYING_BEE_END_FRAME = 155;
	const float DYING_BEE_ANIMATION_DELAY = 0.10f;
}

// inherits from Entity class
class Bee : public Enemy
{
public:
	// constructor
	Bee();

	// inherited member functions
	void update(float frameTime);
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	virtual void draw();

	void stop(double wallX, double wallWidth);

	double getDx()
	{ 
		return dx; 
	}
	double getDy() 
	{ 
		return dy; 
	}

private:
	SpriteCoordinates beeSpriteCoordinates;
	Image beeFlying;
	Image beeDying;

	double dx;
	double dy;
};
#endif