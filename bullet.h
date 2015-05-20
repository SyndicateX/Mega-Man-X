#ifndef bullet_h               // Prevent multiple definitions if this 
#define bullet_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace bulletNS
{
	const int WIDTH = 26;                   // image width
	const int HEIGHT = 65;                  // image height
	const int X = 0;						// GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = 0;						// GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 1200;               // pixels per second
	const float MASS = 1.0f;

	const int NO_CHARGE_START_FRAME = 8;
	const int NO_CHARGE_END_FRAME = 11;
	const float NO_CHARGE_ANIMATION_DELAY = 0.17f;

	const int SMALL_CHARGE_START_FRAME = 17;
	const int SMALL_CHARGE_END_FRAME = 18;
	const float SMALL_CHARGE_ANIMATION_DELAY = 0.12f;

	const int LARGE_CHARGE_START_FRAME = 23;
	const int LARGE_CHARGE_END_FRAME = 25;
	const float LARGE_CHARGE_ANIMATION_DELAY = 0.07f;

}

// inherits from Entity class
class Bullet : public Entity
{
public:
	// constructor
	Bullet();
	bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	// inherited member functions
	void update(float frameTime);
	virtual void draw();

	// set functions
	void setInitialY(double initialY)
	{ initialY_ = initialY; }

	// get functions
	double getInitialY()
	{ return initialY_; }

	SpriteCoordinates bulletSpriteCoordinates;
	Image regularBullet;
	Image bulletSmall;
	Image bulletLarge;

private:
	double initialY_;
};
#endif

