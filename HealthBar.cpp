#include "HealthBar.h"

//=============================================================================
// Initialize the Bar 
// Pre: *graphics = pointer to Graphics object
//      *textureM = pointer to TextureManager object
//      left, top = screen location
//      scale = scaling (zoom) amount
//      color = color of bar
// Post: returns true on success, false on error
//=============================================================================
bool HealthBar::initialize(Graphics *graphics, TextureManager *textureM, int left,
	int top, float scale, COLOR_ARGB color)
{
	try {
		Image::initialize(graphics, healthBarNS::IMAGE_SIZE, healthBarNS::IMAGE_SIZE,
			healthBarNS::TEXTURE_COLS, textureM);
		setCurrentFrame(healthBarNS::BAR_FRAME);
		spriteData.x = (float)left;
		spriteData.y = (float)top;
		spriteData.scale = scale;
		colorFilter = color;
	}
	catch (...)
	{
		return false;
	}
	//return okay
	return true;
}

//=============================================================================
// set the Bar size
//=============================================================================
void HealthBar::set(float p)
{
	if (p < 0)
		p = 0;
	else if (p > 100)
		p = 100;
	spriteData.rect.right = spriteData.rect.left + (LONG)(spriteData.width*p / 100);
}

