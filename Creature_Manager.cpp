#pragma once
#ifndef WORLDSIM_CREATURE_MANAGER_CPP
#define WORLDSIM_CREATURE_MANAGER_CPP

/* WorldSim: Creature_Manager
  #include "Creature_Manager.cpp"

	Implementation of Creature_Manager.hpp
*/

#include "Creature_Species.hpp"

#include "Creature_Manager.hpp"


Creature_Manager::Creature_Manager()
{
	totalSpawnWeight=0;
	rng.seed(SEEDER);
}

void Creature_Manager::generate (const int amount /*=1*/)
{
	int currentPoints=1000;
	for (int i=0; i<amount; ++i)
	{
		vSpecies.push(creatureGenerator.get(currentPoints));
		totalSpawnWeight+=currentPoints;
		currentPoints/=2;
	}
}

// return a random flora from the weighted lists
Creature_Species* Creature_Manager::get()
{
	return 0;
	vSpecies.clear();
	std::cout<<"vspecies size: "<<vSpecies.size()<<"\n";
	return 0;
	if (vSpecies.size() == 0 )
	{
		return 0;
	}
	if ( vSpecies.size() == 1 )
	{
		return vSpecies(0);
	}
	int floraSlot=0;
	if (totalSpawnWeight>0)
	{
		floraSlot = rng.rand32(totalSpawnWeight-1);
	}


	int currentWeighting = 0;
	for (int i=0; i<vSpecies.size(); ++i)
	{
		currentWeighting+=vSpecies(i)->spawnWeight;

		if ( currentWeighting >= floraSlot )
		{
			return vSpecies(i);
		}
	}

	return 0;
}

#endif
