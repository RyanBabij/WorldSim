#pragma once
#ifndef WORLDSIM_STATIC_HPP
#define WORLDSIM_STATIC_HPP

/* WorldSim: Static
   #include "Static.hpp" */
	
/**
   Statics are objects which can't move on their own, and therefore
   they don't need to know their coordinates.
   They also only tick once per day.

   Static subclasses:
      Flora
      Tree
      Building
      Furniture

   Statics may block movement and/or LOS
	
	Statics probably won't be able to stack, to save memory.
*/

class Static: public HasTexture
{
   public:
   std::string name;
   unsigned char blockMove; // bitfield blocks movement in direction starting NW and going clockwise
   unsigned char blockLOS; // bitfield blocks LOS in direction starting NW and going clockwise
   
   unsigned char id; // local id from 1-255 for loading/saving/abstraction
   
   // HP
   // DAMAGE RESIST
   
   Static()
   {
      blockMove=0;
      blockLOS=0;
      id=0;
   }
	
	virtual std::string getName()
	{
		return name;
	}
   
   virtual Texture* currentTexture()
   {
      return 0;
   }
};

// object capable of holding Ingredients
class Harvestable
{
   public:
};

#endif
