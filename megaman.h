
#ifndef megaman_h             // Prevent multiple definitions if this 
#define megaman_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace megamanNS
{
    const int WIDTH = 75;                  // image width
    const int HEIGHT = 100;                  // image height
    const int X = GAME_WIDTH/4 - WIDTH;     // location on screen
    const int Y = GAME_HEIGHT - HEIGHT * 2;
    const float SPEED = 300;                // pixels per second
	const float MASS = 1.0e6f;
}

// inherits from Entity class
class Megaman : public Entity
{
public:
    // constructor
    Megaman();

    // inherited member functions
    void update(float frameTime);

	//In case megaman collides with a wall
	void Megaman::stop(int wallX, int wallY, int wallLength, int wallHeight);
private:
	bool standingOnSurface_ = true;
	bool floorCollision_ = true;
};
#endif

