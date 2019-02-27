#pragma once
#ifndef WORLDSIM_LOCALTILE_CPP
#define WORLDSIM_LOCALTILE_CPP

#include "LocalTile.hpp"

#include <WorldGenerator/Biome.hpp>

/*
	Local tile data gets stored here. Render stuff can't go here because that depends on neighboring tiles.
*/

LocalTile::LocalTile()
{
  hasFloor = false;
  nFish=-1;
}

LocalTile::~LocalTile()
{
}

void LocalTile::addObject(WorldObject* obj)
{
  vObject.push(obj);
}
void LocalTile::removeObject(WorldObject* obj)
{
  vObject.erase(obj);
}

void LocalTile::clearObjects()
{
  for ( int i=0; i<vObject.size();++i)
  {
    delete vObject(i);
  }
  vObject.clear();
}



    // Returns true is this tile has an object that can block line of sight.
bool LocalTile::hasViewBlocker()
{
  for ( int i=0; i<vObject.size();++i)
  {
    if (vObject(i)->blocksView)
    {
      return true;
    }
  }
  return false;
}

    // Returns true is this tile has an object that can block movement.
bool LocalTile::hasMovementBlocker()
{
  for ( int i=0; i<vObject.size();++i)
  {
    if (vObject(i)->blocksMovement)
    {
      return true;
    }
  }
  return false;
}







  std::string LocalTile::getName()
  {
    if ( baseTerrain == NOTHING )
    {
      return "Nothing";
    }
    else if ( baseTerrain == OCEAN )
    {
      return "Ocean";
    }
    else if (baseTerrain == FOREST)
    {
      return "Forest";
    }
    else if (baseTerrain == DESERT)
    {
      return "Desert";
    }
    else if (baseTerrain == UNDERGROUND)
    {
      return "Underground";
    }
    
    else if ( baseTerrain == GRASSLAND )
    {
      return "Grass";
    }
    else
    {
      return "Ground";
    }
  }


    /* INHERITED FUNCTIONS */

Texture* LocalTile::currentTexture()
{
  if ( hasFloor )
  {
    return &TEX_LOCAL_FLOOR;
  }
  
	//enum enumBiome { NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};
	if ( baseTerrain == NOTHING )
	{
		return &TEX_WORLD_TEST_00;
	}
	else if ( baseTerrain == OCEAN )
	{
		return &TEX_WORLD_TERRAIN_OCEAN_00;
	}
	else if (baseTerrain == FOREST)
	{
		return &TEX_WORLD_TERRAIN_FOREST_TREE;
	}
	else if (baseTerrain == DESERT)
	{
		return &TEX_WORLD_TERRAIN_DESERT_00;
	}
	else if (baseTerrain == UNDERGROUND)
	{
		return &TEX_WORLD_TERRAIN_UNDERGROUND_00;
	}
	
	//else if ( biome == GRASSLAND )
	else
	{

		if ( seed %4 == 0 )
		{
			return &TEX_WORLD_TERRAIN_GRASS_00;
		}
		if ( seed %4 == 1 )
		{
			return &TEX_WORLD_TERRAIN_GRASS_01;
		}
		if ( seed %4 == 2 )
		{
			return &TEX_WORLD_TERRAIN_GRASS_02;
		}
		return &TEX_WORLD_TERRAIN_GRASS_03;

	}

	return &TEX_WORLD_TEST_00;
}

Vector <HasTexture*> * LocalTile::currentTextures()
{
  return 0;
}


#endif
