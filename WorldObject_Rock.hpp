#pragma once
#ifndef GUILD_WORLDOBJECT_ROCK_HPP
#define GUILD_WORLDOBJECT_ROCK_HPP

/* WorldObject_Rock.hpp
	#include"WorldObject_Rock.hpp"
	<DOCUMENTATION>
*/

class WorldObject_Rock: public WorldObject
{
	public:
	// int x, y;
	int nGold;
	// bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	// double weight; /* in grams */
	// double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
	
	WorldObject_Rock();
	virtual ~WorldObject_Rock() {}
  
  
	virtual std::string getName();
	// virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	// int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		// int distanceFrom(WorldObject*); /* Alias. */

	virtual Texture* currentTexture();
};

#endif
