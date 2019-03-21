#pragma once
#ifndef WORLDSIM_LOCALTILE_CPP
#define WORLDSIM_LOCALTILE_CPP

#include "LocalTile.hpp"

#include <WorldGenerator/Biome.hpp>

#include "Creature.hpp"

/*
	Local tile data gets stored here. Render stuff can't go here because that depends on neighboring tiles.
*/

LocalTile::LocalTile()
{
  hasFloor = false;
  bWall=0;
  nFish=-1;
  footprint=0;
  height=0;
  isLand=false;
  seed=0;
}

LocalTile::~LocalTile()
{
}

  // Push object to master list and generic object list.
void LocalTile::add(WorldObject* _object)
{
  vObject.push(_object);
  vObjectGeneric.push(_object);
}
  // Push object to Item list and master list.
void LocalTile::add(Item* _item)
{
  vItem.push(_item);
  vObject.push(_item);
}

void LocalTile::add(Character* _character)
{
  vCharacter.push(_character);
  vObject.push(_character);
}
void LocalTile::add(Creature* _creature)
{
  vCreature.push(_creature);
  vObject.push(_creature);
}


  // Remove object from master list and generic object list.
void LocalTile::remove(WorldObject* _object)
{
  vObject.erase(_object);
  vObjectGeneric.erase(_object);
}
  // Remove object from Item list and master list.
void LocalTile::remove(Item* _item)
{
  vItem.erase(_item);
  vObject.erase(_item);
}
  // Remove object from Character list and master list.
void LocalTile::remove(Character* _character)
{
  vCharacter.erase(_character);
  vObject.erase(_character);
}
  // Remove object from Creature list and master list.
void LocalTile::remove(Creature* _creature)
{
  vCreature.erase(_creature);
  vObject.erase(_creature);
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
  if (bWall != 0)
  {
    return true;
  }
  for ( int i=0; i<vObject.size();++i)
  {
    if (vObject(i)->blocksView )
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
  if ( hasFloor==1 )
  {
    return &TEX_LOCAL_FLOOR;
  }
  else if (hasFloor==2)
  { return &TEX_FLOOR_WOOD;
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

bool LocalTile::canTravelNorth()
{
  if ((bWall & 0b10000000) == 0)
  {
    return true;
  }
  return false;
}
bool LocalTile::canTravelEast()
{
  if ((bWall & 0b01000000) == 0)
  {
    return true;
  }
  return false;
}
bool LocalTile::canTravelSouth()
{
  if ((bWall & 0b00100000) == 0)
  {
    return true;
  }
  return false;
}
bool LocalTile::canTravelWest()
{
  if ((bWall & 0b00010000) == 0)
  {
    return true;
  }
  return false;
}

std::string LocalTile::getSaveData()
{
  std::string saveData = "";
  
  if ( baseTerrain == OCEAN )
  {
    saveData+="O";
  }
  else
  {
    saveData+="G";
  }
  return saveData;
}
void LocalTile::loadData(std::string _saveData)
{
  if(_saveData[0] == 'O' )
  {
    baseTerrain=OCEAN;
  }
  else
  {
    baseTerrain = GRASSLAND;
  }
  
}


#endif
