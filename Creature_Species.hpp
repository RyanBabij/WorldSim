#pragma once
#ifndef WORLDSIM_CREATURE_SPECIES_HPP
#define WORLDSIM_CREATURE_SPECIES_HPP

/* WorldSim: Creature_Species
  #include "Creature_Species.hpp"
  
  Meta information about all Creatures of a certain type.
  
  Creature instances should be generated from here.
  
  This is necessary because unlike something like Flora, Creatures may have individually different attributes.
*/

#include <Container/Table/TableInterface.hpp>

class World_Biome;

class Creature_Species: public TableInterface
{
	public:
	
	World_Biome* biome;
	
	Creature_Species(std::string _name, int _spawnWeight)
	{
		name = _name;
		spawnWeight = _spawnWeight;
	}
	
	std::string name;
	int spawnWeight;
	
	// TABLE INTERFACE
	std::string getColumn(std::string _column)
	{
		if ( _column=="name" )
		{
			return name;
		}
		return "?";
	}
	std::string getColumnType(std::string _column)
	{
		return "string";
	}
	
};

#endif
