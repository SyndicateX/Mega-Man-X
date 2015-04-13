#include "chargingSprites.h"

//=============================================================================
// default constructor
//=============================================================================
chargingSprites::chargingSprites() : Entity()
{
	spriteData.width = chargingSpritesNS::WIDTH;          // size of Ship1
	spriteData.height = chargingSpritesNS::HEIGHT;
	spriteData.x = chargingSpritesNS::X;                  // location on screen
	spriteData.y = chargingSpritesNS::Y;
	spriteData.rect.bottom = chargingSpritesNS::HEIGHT;   // rectangle to select parts of an image
	spriteData.rect.right = chargingSpritesNS::WIDTH;
	spriteData.direction = RIGHT;				  // chargingSprites always faces right at the start of any level
	spriteData.state = STANDING;
	velocity.x = 0;						        // velocity X
	velocity.y = 0;							    // velocity Y
	frameDelay = 1.0;// 0.07;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	edge.top = -chargingSpritesNS::HEIGHT / 2;              // set collision edges
	edge.bottom = chargingSpritesNS::HEIGHT / 2;
	edge.left = -chargingSpritesNS::WIDTH / 2;
	edge.right = chargingSpritesNS::WIDTH / 2;
	collisionType = entityNS::ROTATED_BOX;
	mass = chargingSpritesNS::MASS;

}

//=============================================================================
// Initialize Mega Man.
// Post: returns true if successful, false if failed
//=============================================================================
bool chargingSprites::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{
	// chargingSprites shooting sprite initialize
	megamanShootingSpriteCoordinates.populateVector("xshootcoords.txt");
	if (!initializeCoords(megamanShootingSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing chargingSprites"));

	megamanCharge1.initialize(gamePtr->getGraphics(), chargingSpritesNS::WIDTH,
		chargingSpritesNS::HEIGHT, 0, textureM);
	if (!megamanCharge1.initialize(megamanShootingSpriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing chargingSprites"));
	megamanCharge1.setFrames(chargingSpritesNS::CHARGE1_MEGAMAN_START_FRAME, chargingSpritesNS::CHARGE1_MEGAMAN_END_FRAME);
	megamanCharge1.setCurrentFrame(chargingSpritesNS::CHARGE1_MEGAMAN_START_FRAME);
	megamanCharge1.setFrameDelay(chargingSpritesNS::CHARGE1_MEGAMAN_ANIMATION_DELAY);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}
//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void chargingSprites::update(float frameTime)
{
	Entity::update(frameTime);

	megamanCharge1.update(frameTime);
}

void chargingSprites::draw()
{
	if (charge1_ == true)
	{
		megamanCharge1.draw(spriteData);
	}
}

