#pragma once
#ifndef WORLDSIM_WORLDOBJECT_CPP
#define WORLDSIM_WORLDOBJECT_CPP

#include"WorldObject.hpp"

/* WorldObject.cpp
	#include"WorldObject.cpp"

  Implementation of WorldObject.hpp.
*/

WorldObject::WorldObject()
{
	x=0; y=0;
  fullX = 0; fullY = 0;
	weight=0;
	bulk=0;
  
  blocksView=false;
  blocksMovement=false;
  canCook = false;
  canHarvest = false;
  
  worldX=-1; worldY=-1;
  
  chopAmount = -1;
}

std::string WorldObject::getName()
{
	return "WorldObject";
}

int WorldObject::distanceTo (WorldObject* _obj) 
{
	return std::max(abs(x-_obj->x),abs(y-_obj->y));
}

int WorldObject::distanceFrom(WorldObject* _obj) { return distanceTo(_obj); } /* Alias. */

int WorldObject::distanceTo (int _x, int _y) 
{
	return std::max(abs(x-_x),abs(y-_y));
}

Texture* WorldObject::currentTexture()
{
	return &TEX_TEST_404;
}

#endif
