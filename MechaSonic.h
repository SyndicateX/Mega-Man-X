#pragma once
#include "enemy.h"
class MechaSonic :
	public Enemy
{
public:
	MechaSonic();
	~MechaSonic();
	bool initialize(Game * gamePtr, int width, int height, int ncols,
		TextureManager * textureM);
private:

};

