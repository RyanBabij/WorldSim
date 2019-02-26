#pragma once
#ifndef WORLDSIM_WORLDOBJECT_PLANT_HPP
#define WORLDSIM_WORLDOBJECT_PLANT_HPP

/* WorldObject_Plant.hpp
	#include"WorldObject_Plant.hpp"

  Plants are mostly useful for harvesting alchemy ingredients.
*/

class WorldObject_Plant: public WorldObject
{
	public:
	// int x, y;
	
	// bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	// double weight; /* in grams */
	// double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
  
    // Determines the stage of growth the plant is at. Can determine harvest yield.
  int growth;
	
	WorldObject_Plant();
	virtual ~WorldObject_Plant() {}
  
  
	virtual std::string getName();
	// virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	// int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		// int distanceFrom(WorldObject*); /* Alias. */

	virtual Texture* currentTexture();
};

#endif
