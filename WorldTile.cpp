#pragma once
#ifndef GUILD_WORLDTILE_CPP
#define GUILD_WORLDTILE_CPP

#include "WorldTile.hpp"

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
	
	return &TEX_WORLD_TERRAIN_GRASS_00;
	
	return &TEX_WORLD_TEST_00;
}

Texture* WorldTile::getImprovementTexture()
{
	return 0;
}

#endif
