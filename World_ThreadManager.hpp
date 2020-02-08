#pragma once
#ifndef WORLDSIM_WORLD_THREADMANAGER_HPP
#define WORLDSIM_WORLD_THREADMANAGER_HPP

/* WorldSim: World_ThreadManager.hpp
	#include "World_ThreadManager.hpp"

	Manages the threads being run in World class.
   In future will control background world generation,
   unloading, and simulation.
   
   Currently I'm just working on generation.

*/

class World;

class World_ThreadManager
{
   int maxWorldsToGenerate; // max number of worlds to generate at once.
   // Vector <World_Local*> vGenningMaps; // vector of worlds currently generating.
   // Vector <World_Local*> vGeneratedMaps; // vector of worlds generated
   // Vector <World_Local*> vMapsToUnload; // vector of worlds to cache to disk
   
   ArrayS2 <World_Local> *aWorldTile; // pointer to World->Local Map array.
   
   public:
   World* world;
   
   World_ThreadManager(World* _world);
   void main();
};

#endif