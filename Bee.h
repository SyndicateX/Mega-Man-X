#ifndef bee_h               // Prevent multiple definitions if this 
#define bee_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "enemy.h"
#include "constants.h"
#include "sprites.h"

namespace beeNS
{
	const int WIDTH = 87;                   // image width
	const int HEIGHT = 120;                  // image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 250;                // pixels per second

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
	void handleCollisions(double wallX, double wallY, double wallWidth, double wallHeight);
	void draw();

	void setStartX(double startX)
	{
		startX_ = startX;
	}
	void setStartY(double startY)
	{
		startY_ = startY;
	}

private:
	SpriteCoordinates beeSpriteCoordinates;
	Image beeFlying;
	Image beeDying;
};
#endif