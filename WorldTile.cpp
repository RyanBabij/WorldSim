#pragma once
#ifndef GUILD_WORLDTILE_CPP
#define GUILD_WORLDTILE_CPP

#include "WorldTile.hpp"

#include <WorldGenerator/Biome.hpp>

/*
	Local tile data gets stored here. Render stuff can't go here because that depends on neighboring tiles.
*/

WorldTile::WorldTile()
{
	seed = 0;
	biome = NOTHING;
	baseMoveCost = 0;
	canHaveSettlement = false;
	baseFertility = 0;
	canMove = 0;
	baseLogisticsCost = 0;
	defensiveBonus = 0;
  baseMetal=0;
}

WorldTile::~WorldTile()
{
}

void WorldTile::addInfluence (Tribe* tribe, int amount)
{
  if ( tribe == 0 ) { return; }
  
  // Note that std::map will automatically initialise new entries.
  mInfluence[tribe] += amount;
  
  
  //Prevent influence from being excessively high.
  if ( mInfluence[tribe] > 200 )
  {
    mInfluence[tribe] = 200;
  }
  
  // std::cout<<"Adding influence.\n";
  
  // for(auto elem : mInfluence)
  // {
     // std::cout << elem.first->name   << " " << elem.second <<".\n";
  // }
}

void WorldTile::removeInfluence (Tribe* tribe, int amount)
{
  if ( tribe == 0 ) { return; }
  
  // Note that std::map will automatically initialise new entries.
  mInfluence[tribe] -= amount;
  
  if ( mInfluence[tribe] < 0 ) { mInfluence[tribe] = 0; }
  
  // std::cout<<"Removing influence.\n";
  
  // for(auto elem : mInfluence)
  // {
     // std::cout << elem.first->name   << " " << elem.second <<".\n";
  // }
}

void WorldTile::degradeInfluence (int amount /* =1 */)
{
  //std::cout<<"Degrading influence.\n";
  
  for(auto &elem : mInfluence)
  {
    elem.second -= amount;
    if (elem.second < 0) { elem.second = 0; }
  }
}

void WorldTile::destroyInfluence (Tribe* tribe)
{
  if ( tribe == 0 ) { return; }

  mInfluence.erase(tribe);
}

Tribe* WorldTile::getDominantInfluence ()
{
  if ( mInfluence.empty() || (--mInfluence.end())->second<=0 ) { return 0; }
  // Maps are sorted from smallest to largest, so we simply need to return the last key.
  
  //std::cout<<"Val is :"<<(--mInfluence.end())->second<<".\n";
  //return 0;
  
  return (--mInfluence.end())->first;
}

int WorldTile::getDominantInfluenceValue ()
{
  if ( mInfluence.empty() ) { return 0; }
  // Maps are sorted from smallest to largest, so we simply need to return the last key.
  return (--mInfluence.end())->second;
}


void WorldTile::init(enumBiome _biomeID, int _seed = 0)
{
  biome = _biomeID;
  seed = _seed;
    
  switch(_biomeID) {
    case NOTHING:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 0;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case OCEAN:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case GRASSLAND:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 4;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case FOREST:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case DESERT:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 1;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case MOUNTAIN:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 1;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case SNOW:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 1;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case HILLY:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case JUNGLE:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case WETLAND:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case STEPPES:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    case RIVER:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 2;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    default:
    {
      baseMoveCost = 0;
      canHaveSettlement = false;
      baseFertility = 0;
      canMove = 0;
      baseLogisticsCost = 0;
      defensiveBonus = 0;
      baseMetal = 0;
      break;
    }
    break;
}
    
    if ( biome == MOUNTAIN )
    {
      //std::cout<<"Adding resources to mountain.\n";
      baseMetal=1000;
    }
    else
    {
      baseMetal=0;
    }

}

Texture* WorldTile::currentTexture()
{
	//enum enumBiome { NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13};
	if ( biome == NOTHING )
	{
		return &TEX_WORLD_TEST_00;
	}
	else if ( biome == OCEAN )
	{
		return &TEX_WORLD_TERRAIN_OCEAN_00;
	}
	else if (biome == FOREST)
	{
		return &TEX_WORLD_TERRAIN_FOREST_TREE;
	}
	else if (biome == DESERT)
	{
		return &TEX_WORLD_TERRAIN_DESERT_00;
	}
	
	//else if ( biome == GRASSLAND )
	else
	{
		//std::cout<<"Returning grassyboi\n";
		if ( seed %4 == 0 )
		{
			//std::cout<<"0\n";
			return &TEX_WORLD_TERRAIN_GRASS_00;
		}
		if ( seed %4 == 1 )
		{
			//std::cout<<"1\n";
			return &TEX_WORLD_TERRAIN_GRASS_01;
		}
		if ( seed %4 == 2 )
		{
			//std::cout<<"2\n";
			return &TEX_WORLD_TERRAIN_GRASS_02;
		}
		//std::cout<<"3\n";
		return &TEX_WORLD_TERRAIN_GRASS_03;

	}

	return &TEX_WORLD_TEST_00;
}

Texture* WorldTile::getImprovementTexture()
{
	return 0;
}

#endif
