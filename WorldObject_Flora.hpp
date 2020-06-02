#pragma once
#ifndef WORLDSIM_WORLDOBJECT_FLORA_HPP
#define WORLDSIM_WORLDOBJECT_FLORA_HPP

/* WorldObject_Flora.hpp
	#include"WorldObject_Flora.hpp"
   
   Flora is plants or trees. They generally don't do anything except grow
   and provide resources for Creatures and NPCs. They can also be planted.
   
   Trees generally block movement and LOS, and plants generally don't.
   
   There can only be 1 flora per tile.
   
   Flora growth can easily be abstracted into X ticks per day.

*/

class WorldObject_Flora: public WorldObject
{
	public:

    // Determines the stage of growth the plant is at. Can determine harvest yield.
  int growth;
	
	WorldObject_Flora();
	virtual ~WorldObject_Flora() {}
  
	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};


class Flora_Blackweed: public WorldObject_Flora
{
  public:
  Flora_Blackweed();
	virtual ~Flora_Blackweed() {}

	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};

class Flora_Redweed: public WorldObject_Flora
{
  public:
  Flora_Redweed();
	virtual ~Flora_Redweed() {}

	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};

class Flora_Blueweed: public WorldObject_Flora
{
  public:
  Flora_Blueweed();
	virtual ~Flora_Blueweed() {}

	virtual std::string getName() override;
	virtual Texture* currentTexture() override;
};

#endif
