#pragma once
#ifndef WORLDSIM_WORLD_BIOME_HPP
#define WORLDSIM_WORLD_BIOME_HPP

/* World_Biome.hpp
	#include "World_Biome.hpp"

	Project: WorldSim
	Created: 2/12/2017, 0272554803.
	Updated: 2/12/2017, 0272554803.

	Description:
	

	Notes:


*/


#include <WorldGenerator/WorldGenerator2.hpp>
#include <Container/Table/TableInterface.hpp>

class World_Biome: public TableInterface
{
	public:
	
	std::string name;
	int size; /* size in tiles */
	int type; /* Biome type */
	
	World_Biome()
	{
		name="";
		size=0;
	}
	
	virtual ~World_Biome()
	{
	}
	
	//  NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13
	std::string getColumn(std::string _column)
	{
		if ( _column=="name" )
		{
			return name;
		}
		else if ( _column=="size" )
		{
			return DataTools::toString(size);
		}
		else if ( _column=="type" )
		{
			return WorldGenerator2::biomeName[type];
		}

		
		return "?";
	}
	std::string getColumnType(std::string _column)
	{
		if ( _column == "size" )
		{
			return "number";
		}
		return "string";
	}
};

#endif