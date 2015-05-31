#pragma once
#include "enemy.h"

namespace mechaSonicNS
{
	const int WIDTH = 88;                  // image width
	const int HEIGHT = 121;                  // image height
	const int X = GAME_WIDTH / 2 + WIDTH * 3;     // location on screen
	const int Y = GAME_HEIGHT / 2;
	const float SPEED = 300;                // pixels per second
	const float MASS = 1.0e6f;

	const int IDLE_START_FRAME = 0;
	const int IDLE_END_FRAME = 8;
	const float IDLE_ANIMATION_DELAY = 0.55f;

	const int DYING_START_FRAME = 29;
	const int DYING_END_FRAME = 30;
	const float DYING_ANIMATION_DELAY = 0.12f;
}

class MechaSonic : public Enemy
{
public:
	MechaSonic();
	~MechaSonic();
	bool initialize(Game * gamePtr, int width, int height, int ncols,
		TextureManager * textureM);
	void update(float frameTime);
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
	Image mechaSonicIdle;
	Image mechaSonicDying;
};

