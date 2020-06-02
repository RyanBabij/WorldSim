#pragma once
#ifndef GUILD_WORLDOBJECT_ROCK_CPP
#define GUILD_WORLDOBJECT_ROCK_CPP

#include"WorldObject_Rock.hpp"

/* WorldObject_Rock.cpp
   #include"WorldObject_Rock.cpp"
   <DOCUMENTATION>
*/

WorldObject_Rock::WorldObject_Rock()
{
   x=0; y=0;
   weight=0;
   bulk=0;
  
  nGold = 0;
}

std::string WorldObject_Rock::getName()
{
   return "Rock";
}


Texture* WorldObject_Rock::currentTexture()
{
  if ( nGold > 0 )
  {
    return &TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI_GOLD;
  }
  
   return &TEX_WORLD_TERRAIN_MOUNTAIN_ROCKYBOI;
}

#endif
