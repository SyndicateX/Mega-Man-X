#pragma once
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
//                  Constants
//=============================================================================
// graphic images
const char TILE_TEXTURES[]				= "pictures\\brick001.png"; // tiles
const char BACKDROP_IMAGE_1[]			= "pictures\\CastleBackground.png";
const char BACKDROP_IMAGE_2[]			= "pictures\\GreenBackground.png";
const char BULLET_IMAGE[]				= "pictures\\BusterBusterEdit.png";
const char MEGAMAN_IMAGE[]				= "pictures\\mmx_x4_x_sheet.png";
const char CHARGING_SPRITES_IMAGE[]		= "pictures\\BusterBuster.png";
const char MECHA_SONIC[]				= "pictures\\Mecha_Sonic.Forte.32.png";
const char EXPLOSION[]					= "pictures\\explosion.png";
const char BEE[]						= "pictures\\baddies.png";
const char BOWSER[]						= "pictures\\bowser.png"; 
const char TEXTURES_IMAGE[]				= "pictures\\textures.png";  // game textures

// window
const char CLASS_NAME[] = "Mega Man X";
const char GAME_TITLE[] = "Mega Man X";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1024;              // width of game in pixels
const UINT GAME_HEIGHT = 720;               // height of game in pixels
 
// game
const bool VSYNC = true;                    // true locks display to vertical sync rate
const double PI = 3.14159265;
const float FRAME_RATE = 60.0f;             // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;         // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 500.0f;                   // acceleration of gravity pixels/sec
//const UINT MAP_WIDTH = 2000;
const UINT MAP_HEIGHT = 1500;
const int JUMP_VELOCITY = -180;
const int TERMINAL_VELOCITY = -JUMP_VELOCITY;
const double MAP_X_SCROLL_RATE = 0.15;
const double MAP_Y_SCROLL_RATE = 0.2;

const double WALK_SPEED = 1;
const double DASH_SPEED = 2.2;
const float DAMAGE_TIME = 0.4f;
const float INVINCIBILITY_TIME = 1.6f;
const int MAX_BULLETS = 3;

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[] = "audio\\Win\\Wave Bank.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";

// audio cues
const char EXPLODE[] = "Explode";
const char FIRE[] = "Fire";
const char LOAD[] = "Load";
const char MAX[] = "Max";
const char MAXFIRE[] = "MaxFire";

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR CONSOLE_KEY  = '`';         // ` key
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
const UCHAR LEFT_KEY     = VK_LEFT;     // left arrow
const UCHAR RIGHT_KEY    = VK_RIGHT;    // right arrow
const UCHAR DOWN_KEY	 = VK_DOWN;		// down arrow
const UCHAR UP_KEY		 = VK_UP;		// up arrow
const UCHAR SPACE_KEY	 = VK_SPACE;	// space key

enum State
{
	STANDING,
	WALKING,
	CROUCHING,
	JUMPING,
	SHOOTING,
	DASHING,
	WALL_SLIDING,
	DEAD,
	DAMAGED,
	ATTACKING,
	FIRE_BREATH
};

enum ShotType
{
	NONE,
	REGULAR_SHOT,
	SMALL_CHARGE,
	MEDIUM_CHARGE,
	FIREBALL
};

enum Direction
{
	LEFT,
	RIGHT
};

// weapon types
enum WEAPON 
{ 
	ENEMY_COLLISION,
	ENEMY_PROJECTILE,
	BOSS_COLLISION,
	BOSS_PROJECTILE,
	UNCHARGED_BULLET,
	SMALL_CHARGE_BULLET,
	MEDIUM_CHARGE_BULLET
};

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

#endif
