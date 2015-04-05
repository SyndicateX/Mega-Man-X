#pragma once
#include <vector>
#include <string>

class SpriteCoordinates		//Built to handle sprite sheets of non-uniform sprites by handling coordinates directly
{
public:
	////////////////////////////////////////
	//           Constructors             //
	////////////////////////////////////////
	SpriteCoordinates();
	SpriteCoordinates(std::string fileName);		//Saves sprite coordinates to coords_ (private variable)

	////////////////////////////////////////
	//           Get functions            //
	////////////////////////////////////////
	std::vector<std::vector<int>> getCoords();		//Returns all coordinates of a sprite

private:
	std::vector<std::vector<int>> coords_;			//Stores the coordinates of all the sprites in a sprite sheet
};

