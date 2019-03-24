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
  hasFloor = 0;
  bWall=0;
  nFish=-1;
  footprint=0;
  height=0;
  isLand=false;
  seed=0;
  
  hasGems=false;
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
  //return &TEX_LOCAL_FLOOR;
  
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
	else if (baseTerrain == SNOW)
	{
		return &TEX_WORLD_TERRAIN_SNOW;
	}
	else if (baseTerrain == ICE)
	{
		return &TEX_WORLD_TERRAIN_SNOW;
	}
	else if (baseTerrain == STEPPES)
	{
		return &TEX_WORLD_TERRAIN_STEPPE;
	}
  
  

  
	
	//else if ( biome == GRASSLAND )
	else
	{

		if ( seed==0 || seed %4 == 0 )
		{
			return &TEX_WORLD_TERRAIN_GRASS_00;
		}
		else if ( seed %4 == 1 )
		{
			return &TEX_WORLD_TERRAIN_GRASS_01;
		}
		else if ( seed %4 == 2 )
		{
			return &TEX_WORLD_TERRAIN_GRASS_02;
		}
		return &TEX_WORLD_TERRAIN_GRASS_03;

	}

	return &TEX_WORLD_TEST_00;
}

Vector <Texture*> * LocalTile::currentTextures()
{
  auto vTexture = new Vector <Texture*>;
  //vTexture->push(&TEX_WORLD_TERRAIN_OCEAN_00);
  //vTexture->push(currentTexture());
  //return vTexture;
  
  //return &TEX_LOCAL_FLOOR;
  
  if ( hasFloor==1 )
  {
    vTexture->push(&TEX_LOCAL_FLOOR);
  }
  else if (hasFloor==2)
  {
    vTexture->push(&TEX_FLOOR_WOOD);
  }
  
	//enum enumBiome { NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};
	if ( baseTerrain == NOTHING )
	{
    vTexture->push(&TEX_WORLD_TEST_00);
	}
	else if ( baseTerrain == OCEAN )
	{
    vTexture->push(&TEX_WORLD_TERRAIN_OCEAN_00);
	}
	else if (baseTerrain == FOREST)
	{
    vTexture->push(&TEX_WORLD_TERRAIN_FOREST_TREE);
	}
	else if (baseTerrain == DESERT)
	{
    vTexture->push(&TEX_WORLD_TERRAIN_DESERT_00);
	}
	else if (baseTerrain == UNDERGROUND)
	{
    vTexture->push(&TEX_WORLD_TERRAIN_UNDERGROUND_00);
	}
	else if (baseTerrain == SNOW)
	{
    vTexture->push(&TEX_WORLD_TERRAIN_SNOW);
	}
	else if (baseTerrain == ICE)
	{
    vTexture->push(&TEX_WORLD_TERRAIN_SNOW);
	}
	else if (baseTerrain == STEPPES)
	{
    vTexture->push(&TEX_WORLD_TERRAIN_STEPPE);
	}
  

	//else if ( biome == GRASSLAND )
	else
	{

		if ( seed==0 || seed %4 == 0 )
		{
      vTexture->push(&TEX_WORLD_TERRAIN_GRASS_00);
		}
		else if ( seed %4 == 1 )
		{
      vTexture->push(&TEX_WORLD_TERRAIN_GRASS_01);
		}
		else if ( seed %4 == 2 )
		{
      vTexture->push(&TEX_WORLD_TERRAIN_GRASS_02);
		}
    vTexture->push(&TEX_WORLD_TERRAIN_GRASS_03);
	}
  
  if (hasGems)
  {
  vTexture->push(&TEX_WORLD_ARTIFACT_GEMS);
  }

  
  return vTexture;
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
  { saveData+="O"; }
  else if ( baseTerrain == FOREST )
  { saveData+="F"; }
  else if ( baseTerrain == DESERT )
  { saveData+="D"; }
  else if ( baseTerrain == MOUNTAIN )
  { saveData+="M"; }
  else if ( baseTerrain == SNOW )
  { saveData+="S"; }
  else if ( baseTerrain == HILLY )
  { saveData+="H"; }
  else if ( baseTerrain == JUNGLE )
  { saveData+="J"; }
  else if ( baseTerrain == WETLAND )
  { saveData+="W"; }
  else if ( baseTerrain == STEPPES )
  { saveData+="P"; }
  else if ( baseTerrain == ICE )
  { saveData+="I"; }
  else
  { saveData+="G"; }
  
  saveData+=(char)height+48;
  
  saveData+=" ";
  saveData+=DataTools::toString(seed);
  
  //saveData+=DataTools::toString(height);
  return saveData;
}
void LocalTile::loadData(std::string _saveData)
{
  
  if ( _saveData[0] == 'O'  )
  { baseTerrain=OCEAN; }
  else if ( _saveData[0] == 'F' )
  { baseTerrain=FOREST; }
  else if ( _saveData[0] == 'D' )
  { baseTerrain=DESERT; }
  else if ( _saveData[0] == 'M' )
  { baseTerrain=MOUNTAIN; }
  else if ( _saveData[0] == 'S' )
  { baseTerrain=SNOW; }
  else if ( _saveData[0] == 'H' )
  { baseTerrain=HILLY; }
  else if ( _saveData[0] == 'J' )
  { baseTerrain=JUNGLE; }
  else if ( _saveData[0] == 'W' )
  { baseTerrain=WETLAND; }
  else if ( _saveData[0] == 'P' )
  { baseTerrain=STEPPES; }
  else if ( _saveData[0] == 'I' )
  { baseTerrain=ICE; }
  else
  { baseTerrain = GRASSLAND; }
  
  height = _saveData[1] - 48;
  
  Vector <std::string> * vToke = DataTools::tokenize(_saveData, " \t\n\r");
  
  if (vToke && vToke->size()>1)
  {
    seed = DataTools::toUnsignedShort((*vToke)(1));
    delete vToke;   
  }
  
}


#endif
