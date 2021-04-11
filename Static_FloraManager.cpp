#pragma once
#ifndef WORLDSIM_STATIC_FLORA_MANAGER_CPP
#define WORLDSIM_STATIC_FLORA_MANAGER_CPP

/* WorldSim: Static_FloraManager
   #include "Static_FloraManager.cpp"

	Implementation of Static_FloraManager.cpp.
	
	Note that FloraManager is currently depending on a global instance of FloraGenerator.
	This should probably be fixed at some point.
	
*/

#include "Static_Flora.hpp"

#include "Static_FloraGenerator.cpp"
#include "Static_FloraManager.hpp"



FloraManager::FloraManager()
{
	totalSpawnWeight=0;
	rng.seed(SEEDER);
}

void FloraManager::generate (const int amount /* =1 */)
{
	//std::cout<<"FloraManager generating "<<amount<<" flora.\n";
	int currentPoints=1000;
	for (int i=0;i<amount;++i)
	{			
		vFlora.push(floraGenerator.get(currentPoints));
		totalSpawnWeight+=currentPoints;
		currentPoints/=2;
	}
}

// return a random flora from the weighted lists
Flora* FloraManager::get()
{
	if (vFlora.size() == 0 )
	{
		return 0;
	}
	if ( vFlora.size() == 1 )
	{
		return vFlora(0);
	}
	
	int floraSlot = rng.rand32(totalSpawnWeight-1);
	
	int currentWeighting = 0;
	for (int i=0;i<vFlora.size();++i)
	{
		currentWeighting+=vFlora(i)->spawnWeight;
		
		if ( currentWeighting >= floraSlot )
		{
			//std::cout<<"Spawning: "<<vFlora(i)->name<<"\n";
			return vFlora(i);
		}
	}
	
	return 0;
	
	
}

#endif
