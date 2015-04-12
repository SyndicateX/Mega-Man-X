#ifndef bullet_h               // Prevent multiple definitions if this 
#define bullet_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace bulletNS
{
	const int WIDTH = 12;                   // image width
	const int HEIGHT = 12;                  // image height
	const int X = GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 1000;                // pixels per second
	const float ENERGY_LOSS = 100;          // energy loss on bounce (pixels/sec)
	const float MIN_VY = 1;                 // minumum Y velocity
	const float MASS = 1.0f;
}

// inherits from Entity class
class Bullet : public Entity
{
public:
	// constructor
	Bullet();

	// inherited member functions
	void update(float frameTime);
};
#endif

