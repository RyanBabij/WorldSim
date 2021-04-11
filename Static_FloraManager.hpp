#pragma once
#ifndef WORLDSIM_STATIC_FLORA_MANAGER_HPP
#define WORLDSIM_STATIC_FLORA_MANAGER_HPP

/* WorldSim: Static_FloraManager
   #include "Static_FloraManager.hpp"

	Container class for Flora objects.
	
*/

class Flora;

class FloraManager
{
	public:
	
	int totalSpawnWeight;
	
	// All flora types in the biome (max 255 types)
	Vector <Flora*> vFlora;
	
	RandomLehmer rng;
	
	// All abstracted flora in the map
	//Vector <FloraAbstract*> vFloraAbstract;
	
	FloraManager();
	
	void generate (const int amount=1);

	// return a random flora from the weighted lists
	Flora* get();
};

#endif
