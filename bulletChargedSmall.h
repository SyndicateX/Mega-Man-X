
#ifndef bulletChargedSmall_h               // Prevent multiple definitions if this 
#define bulletChargedSmall_h               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"

namespace bulletChargedSmallNS
{
	const int WIDTH = 30;                   // image width
	const int HEIGHT = 30;                  // image height
	const int X = 0;						// GAME_WIDTH / 2 - WIDTH / 2;   // location on screen
	const int Y = 0;						// GAME_HEIGHT / 2 - HEIGHT / 2;
	const float SPEED = 800;                // pixels per second
	const float MASS = 1.0f;
}

// inherits from Entity class
class BulletChargedSmall : public Entity
{
public:
	// constructor
	BulletChargedSmall();

	// inherited member functions
	void update(float frameTime);
};
#endif

