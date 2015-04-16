#ifndef chargedBullet_h               // Prevent multiple definitions if this 
#define chargedBullet_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace chargedBulletNS
{
	const int WIDTH = 58;                   // image width
	const int HEIGHT = 35;                  // image height
	const int X = 0;						// GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = 0;						// GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 1200;               // pixels per second
	const float MASS = 1.0f;

	const int SMALL_CHARGE_START_FRAME = 0;
	const int SMALL_CHARGE_END_FRAME = 0;
	const float SMALL_CHARGE_ANIMATION_DELAY = 1.0f;

	const int LARGE_CHARGE_START_FRAME = 0;
	const int LARGE_CHARGE_END_FRAME = 0;
	const float LARGE_CHARGE_ANIMATION_DELAY = 1.0f;

}

// inherits from Entity class
class ChargedBullet : public Entity
{
public:
	// constructor
	ChargedBullet();
	bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);

	// inherited member functions
	void update(float frameTime);
	virtual void draw();

	Image chargedBulletSmall;
	Image chargedBulletLarge;
};
#endif

