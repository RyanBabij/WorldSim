#pragma once
#ifndef WORLDSIM_CREATURE_SPECIES_HPP
#define WORLDSIM_CREATURE_SPECIES_HPP

/* WorldSim: Creature_Species
  #include "Creature_Species.hpp"
  
  Meta information about all Creatures of a certain type. Creature instances should be generated from here.
*/

class Creature_Species
{
	public:
	Creature_Species(std::string _name, int _spawnWeight)
	{
		name = _name;
		spawnWeight = _spawnWeight;
	}
	
	std::string name;
	int spawnWeight;
	
};

#endif
