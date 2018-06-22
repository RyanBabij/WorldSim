#pragma once
#ifndef GUILD_WORLDOBJECT_HPP
#define GUILD_WORLDOBJECT_HPP

/* WorldObject.hpp
	#include"WorldObject.hpp"
	<DOCUMENTATION>
*/

class WorldObject: public HasTexture
{
	public:
	int x, y;
	
	bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	double weight; /* in grams */
	double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
	
	WorldObject();
	virtual std::string getName();
	virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		int distanceFrom(WorldObject*); /* Alias. */
		
	virtual ~WorldObject()
	{}
};

#endif
