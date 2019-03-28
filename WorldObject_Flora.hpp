#pragma once
#ifndef WORLDSIM_WORLDOBJECT_FLORA_HPP
#define WORLDSIM_WORLDOBJECT_FLORA_HPP

/* WorldObject_Flora.hpp
	#include"WorldObject_Flora.hpp"

  All flora worldobjects. Basically trees and plants. Generally stuck where they spawn, mostly just good for either
  harvesting or growing.
  Flora is a special case of WorldObject, as there can be no more than 1 on any tile.
  
*/

class WorldObject_Flora: public WorldObject
{
	public:
	// int x, y;
	
	// bool stackable; /* True if multiple objects of this class can be merged together into a single stack, sharing their data. */
	
	// double weight; /* in grams */
	// double bulk; /* In cm^3 , and adjusted higher for cumbersome items.*/
  
    // Determines the stage of growth the plant is at. Can determine harvest yield.
  int growth;
	
	WorldObject_Flora();
	virtual ~WorldObject_Flora() {}
  
  
	virtual std::string getName() override;
	// virtual std::string getExtendedInfo() { return "N/A"; }
	
	
	// int distanceTo(WorldObject*); /* Chebyshev (this one is distances where diagonal movement is allowed.) */
		// int distanceFrom(WorldObject*); /* Alias. */

	virtual Texture* currentTexture() override;
};


class Flora_Blackweed: public WorldObject_Flora
{
  Flora_Blackweed();
	virtual ~Flora_Blackweed() {}

	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};

class Flora_Redweed: public WorldObject_Flora
{
  Flora_Redweed();
	virtual ~Flora_Redweed() {}

	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};

class Flora_Blueweed: public WorldObject_Flora
{
  Flora_Blueweed();
	virtual ~Flora_Blueweed() {}

	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};

#endif
