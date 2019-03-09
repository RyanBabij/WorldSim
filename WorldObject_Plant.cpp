#pragma once
#ifndef GUILD_WORLDOBJECT_PLANT_CPP
#define GUILD_WORLDOBJECT_PLANT_CPP

#include"WorldObject_Plant.hpp"

/* WorldObject_Plant.cpp
	#include"WorldObject_Plant.cpp"

  Implementation of WorldObject_Plant.hpp.
*/

WorldObject_Plant::WorldObject_Plant()
{
	x=0; y=0;
	weight=0;
	bulk=0;
  
  growth=0;
  
  blocksView = false;
  blocksMovement = false;
}

std::string WorldObject_Plant::getName()
{
	return "Plant";
}


Texture* WorldObject_Plant::currentTexture()
{
  if ( growth==0 )
  {
    return &TEX_FLORA_PLANT;
  }
	return &TEX_FLORA_PLANT;
}

#endif
