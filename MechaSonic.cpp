#include "MechaSonic.h"


MechaSonic::MechaSonic()
{
	spriteData.width = mechaSonicNS::WIDTH;          // size of mechaSonic
	spriteData.height = mechaSonicNS::HEIGHT;
	spriteData.x = startX_;                  // location on screen
	spriteData.y = startY_;
	spriteData.rect.bottom = mechaSonicNS::HEIGHT;   // rectangle to select parts of an image
	spriteData.rect.right = mechaSonicNS::WIDTH;
	spriteData.direction = RIGHT;				  // mechaSonic always faces right at the start of any level
	spriteData.state = STANDING;
	velocity.x = 0;						        // velocity X
	velocity.y = -20;							    // velocity Y
	frameDelay = 1.0;// 0.07;
	startFrame = 0;                             // first frame of animation
	endFrame = 0;                           // last frame of animation
	currentFrame = startFrame;
	edge.top = -mechaSonicNS::HEIGHT / 2;              // set collision edges
	edge.bottom = mechaSonicNS::HEIGHT / 2;
	edge.left = -mechaSonicNS::WIDTH / 2;
	edge.right = mechaSonicNS::WIDTH / 2;
	collisionType = entityNS::BOX;
	mass = mechaSonicNS::MASS;
	visible = true;
	active = true;
	dx = 0;
	dy = 0;
}

MechaSonic::~MechaSonic()
{

}

//=============================================================================
// initialize
// Initialization for MechaSonic
//=============================================================================
bool MechaSonic::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{	
	std::string spriteCoordinatesFileName = "sonicsprites.txt";

	spriteCoordinates.populateVector(spriteCoordinatesFileName);
	if (!initializeCoords(spriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

	//Idle
	if (!mechaSonicIdle.initialize(spriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

	mechaSonicIdle.initialize(gamePtr->getGraphics(), mechaSonicNS::WIDTH,
		mechaSonicNS::HEIGHT, 0, textureM);
	mechaSonicIdle.setFrames(mechaSonicNS::IDLE_START_FRAME, mechaSonicNS::IDLE_END_FRAME);
	mechaSonicIdle.setCurrentFrame(mechaSonicNS::IDLE_START_FRAME);
	mechaSonicIdle.setFrameDelay(mechaSonicNS::IDLE_ANIMATION_DELAY);

	//Dying
	if (!mechaSonicDying.initialize(spriteCoordinates))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing mecha sonic"));

	mechaSonicDying.initialize(gamePtr->getGraphics(), mechaSonicNS::WIDTH,
		mechaSonicNS::HEIGHT, 0, textureM);
	mechaSonicDying.setFrames(mechaSonicNS::DYING_START_FRAME, mechaSonicNS::DYING_END_FRAME);
	mechaSonicDying.setCurrentFrame(mechaSonicNS::DYING_START_FRAME);
	mechaSonicDying.setFrameDelay(mechaSonicNS::DYING_ANIMATION_DELAY);

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void MechaSonic::update(float frameTime)
{
	Entity::update(frameTime);

	if (spriteData.state != DEAD)
	{
		gravity(frameTime);
		mechaSonicIdle.update(frameTime);
	}
	else
	{
		if (mechaSonicDying.getCurrentFrame() == mechaSonicNS::DYING_END_FRAME)
		{
			visible = false;
		}
		mechaSonicDying.update(frameTime);
	}
}

void MechaSonic::handleCollisions(int wallX, int wallY, int wallLength, int wallHeight)
{
	stop(wallX, wallY, wallLength, wallHeight);
}

//=============================================================================
// draw
// Drawing all of Enemy's different states
//=============================================================================
void MechaSonic::draw()
{
	if (spriteData.state != DEAD)
	{
		mechaSonicIdle.draw(spriteData);
	}
	else
	{
		mechaSonicDying.draw(spriteData);
		audio->playCue(EXPLODE);
	}
}




