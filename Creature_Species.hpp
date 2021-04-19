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

class Creature;
class World_Biome;

class Creature_Species: public TableInterface
{
	public:
	
	World_Biome* biome;
		
	std::string name;
	int spawnWeight;
		
	Creature_Species(std::string _name, int _spawnWeight);
	
	// return an instance of this species
	Creature* spawn();
	
	// TABLE INTERFACE
	std::string getColumn(std::string _column);
	std::string getColumnType(std::string _column);
	
};

#endif
