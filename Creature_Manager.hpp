#pragma once
#ifndef WORLDSIM_CREATURE_MANAGER_HPP
#define WORLDSIM_CREATURE_MANAGER_HPP

/* WorldSim: Creature_Manager
  #include "Creature_Manager.hpp"
  
  Container for Creature_Species, stored per-biome.
*/

class Creature_Species;

class Creature_Manager
{
	public:
	
	int totalSpawnWeight;
	
	// All flora types in the biome (max 255 types)
	Vector <Creature_Species*> vSpecies;
	
	RandomLehmer rng;
	
	// All abstracted flora in the map
	//Vector <FloraAbstract*> vFloraAbstract;
	
	Creature_Manager();
	
	void generate (const int amount =1);
	
	// return a random flora from the weighted lists
	Creature_Species* get();
};

#endif
