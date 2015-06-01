#ifndef chargingSprites_h             // Prevent multiple definitions if this 
#define chargingSprites_h             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "sprites.h"

namespace chargingSpritesNS
{
	const int WIDTH = 100;                  // image width
	const int HEIGHT = 100;                  // image height

	const int CHARGE1_MEGAMAN_START_FRAME = 0;
	const int CHARGE1_MEGAMAN_END_FRAME = 7;
	const float CHARGE1_MEGAMAN_ANIMATION_DELAY = 0.04f;
}

// inherits from Entity class
class chargingSprites : public Entity
{
public:
	// constructor
	chargingSprites();

	// inherited member functions
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	void update(float frameTime);
	virtual void draw();

	bool isCharged1() { return charge1_; }
	void setCharge1(bool chargeStatus) { charge1_ = chargeStatus; }
private:
	Image megamanCharge1;
	SpriteCoordinates megamanShootingSpriteCoordinates;
	bool charge1_ = false;
};
#endif

