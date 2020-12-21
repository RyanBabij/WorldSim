#pragma once
#ifndef WORLDSIM_CREATURE_MANAGER_HPP
#define WORLDSIM_CREATURE_MANAGER_HPP

/* WorldSim: Creature_Manager
  #include "Creature_Manager.hpp"
  
  Container for Creature_Species, stored per-biome.
*/

#include "Creature_Generator.hpp"

class Creature_Manager
{
	public:
	
	int totalSpawnWeight;
	
	// All flora types in the biome (max 255 types)
	Vector <Creature_Species*> vSpecies;
	
	RandomLehmer rng;
	
	// All abstracted flora in the map
	//Vector <FloraAbstract*> vFloraAbstract;
	
	Creature_Manager()
	{
		totalSpawnWeight=0;
		rng.seed(SEEDER);
	}
	
	void generate (const int amount=1)
	{
		int currentPoints=1000;
		for (int i=0;i<amount;++i)
		{			
			vSpecies.push(creatureGenerator.get(currentPoints));
			totalSpawnWeight+=currentPoints;
			currentPoints/=2;
		}
	}
	
	// return a random flora from the weighted lists
	Creature_Species* get()
	{
		if (vSpecies.size() == 0 )
		{
			return 0;
		}
		if ( vSpecies.size() == 1 )
		{
			return vSpecies(0);
		}
		
		int floraSlot = rng.rand32(totalSpawnWeight-1);
		
		int currentWeighting = 0;
		for (int i=0;i<vSpecies.size();++i)
		{
			currentWeighting+=vSpecies(i)->spawnWeight;
			
			if ( currentWeighting >= floraSlot )
			{
				return vSpecies(i);
			}
		}
		
		return 0;
		
		
	}
};

#endif
