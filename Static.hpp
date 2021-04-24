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
	
    // OBJECT INTERACTION
    // In future these functions might need to be expanded to return multiple possibilities, for example
    // "Stab target" and "Slash target". Or "Chop down door" and "Pry open door".
    // The good thing about this approach is that it can list out all possible interactions between objects,
    // which I think is better than making the player guess what everything does.
    // There are different types of objects, such as Items, WorldObjects and LocalTiles.
  virtual void interact (WorldObject* obj, int interactionType=0)
  {
    std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  }
  // virtual void interact (LocalTile* obj, int interactionType=0)
  // {
    // std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  // }
  // virtual void interact (Item* obj, int interactionType=0)
  // {
    // std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  // }
  // virtual void interact (Character* obj, int interactionType=0)
  // {
    // std::cout<<"Char interact\n";
    // //std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  // }
  // virtual void interact (Creature* obj, int interactionType=0)
  // {
    // std::cout<<"Creature interact\n";
    // //std::cout<<"The "<<getName()<<" interacts with the "<<obj->getName()<<".\n";
  // }
   
	/* HASTEXTURE */
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
