#include "sprites.h"
#include <fstream>
#include <cctype>

//=============================================================================
// default constructor
//=============================================================================
SpriteCoordinates::SpriteCoordinates()
{
	//Not necessary. SpriteCoordinates should only be constructed with a file name.
}

//=============================================================================
// constructor
//=============================================================================
//SpriteCoordinates::SpriteCoordinates(std::string fileName)
void SpriteCoordinates::populateVector(std::string fileName)
{
	//Finds <X>, <Y>, <Width>, and <Length> in an XML file converted into a txt file containing all the sprite information
	std::ifstream in;
	std::string line;
	std::string nums;
	
	in.open(fileName);

	while (!in.eof())
	{
		in >> line;
		if (line.substr(0, 3) == "<X>")
		{
			coords_.push_back({ 0, 0, 0, 0 });
			for (int i = 3; isdigit(line[i]); i++)
			{
				nums += line[i];
			}
			coords_[coords_.size() - 1][0] = stoi(nums);
			nums = "";
		}
		if (line.substr(0, 3) == "<Y>")
		{
			for (int i = 3; isdigit(line[i]); i++)
			{
				nums += line[i];
			}
			coords_[coords_.size() - 1][1] = stoi(nums);
			nums = "";
		}
		if (line.substr(0, 7) == "<Width>")
		{
			for (int i = 7; isdigit(line[i]); i++)
			{
				nums += line[i];
			}
			coords_[coords_.size() - 1][2] = stoi(nums);
			nums = "";
		}
		if (line.substr(0, 8) == "<Height>")
		{
			for (int i = 8; isdigit(line[i]); i++)
			{
				nums += line[i];
			}
			coords_[coords_.size() - 1][3] = stoi(nums);
			nums = "";
		}
	}
}

//=============================================================================
// Get Functions
//=============================================================================
std::vector<std::vector<int>> SpriteCoordinates::getCoords()
{
	return coords_;
}
