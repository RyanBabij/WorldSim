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
  
  isUnderground=false;
  
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

std::string WorldObject::getBaseData()
{
  std::string saveData = getName();
  saveData+=" ";
  saveData+=DataTools::toString(x);
  saveData+=" ";
  saveData+=DataTools::toString(y);
  saveData+=" ";
  
  if ( blocksView ) { saveData+= "[BLOCKS_VIEW] "; }
  if ( blocksMovement ) { saveData+= "[BLOCKS_MOVEMENT] "; }
  if ( canCook ) { saveData+= "[CAN_COOK] "; }
  if ( canHarvest ) { saveData+= "[CAN_HARVEST] "; }

  saveData+="\n";
  return saveData;
}

std::string WorldObject::getSaveData()
{
  std::string saveData = getName();
  

  return saveData;
}
void WorldObject::loadData(std::string _saveData)
{
}


#endif
