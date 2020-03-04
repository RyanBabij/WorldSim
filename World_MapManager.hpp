#pragma once
#ifndef WORLDSIM_WORLD_MAPMANAGER_HPP
#define WORLDSIM_WORLD_MAPMANAGER_HPP

/* WorldSim: World_MapManager.hpp
	#include "World_MapManager.hpp"

	Manages the local maps, generating and caching them as required.
   
   Currently I'm just working on generation.

   This is basically a container for Map_Local objects, with multithreading support.

*/

class World;

#if defined WILDCAT_THREADING
   #include <thread>
   #include <mutex>
   #include <shared_mutex>
   #include <atomic>
#endif


// Keeping track of caching order:
// using a list is maybe not ideal because every time we push to the
// list we need to search the entire list anyway.
// Instead it's probably best if each map carries an accessID.
// When the accessID reaches overflow we can quickly rebuilt it from 0.


class World_MapManager
{
   int maxWorldsToGenerate; // max number of worlds to generate at once.

   Vector <World_Local*> vMapCache; // Stores all generated maps
   Vector <World_Local*> vJobs; // Vector of maps to be loaded in
   
   Vector <World_Biome*> vBiome; // Vector of biomes to load/simulate
   
  //Vector <World_Local*> vUnloadJobs; // Vector of maps to be unloaded
  //Vector <World_Local*> vImportantMaps; // Maps to be kept loaded in.

#ifdef WILDCAT_THREADING
   std::mutex mutexArrayAccess;
   std::mutex mutexArrayResize;
   std::shared_mutex mutexArrayResize2;
   
   std::mutex mutexVector;
   std::mutex mutexJob;
   
   Mutex mutexBiome;
   
   // keep track of number of threads running
   // to perform safe shutdown
   std::atomic <int> nThreads;
   
   //std::atomic <int> nBiomeTiles; // The current number of biome tiles in memory.
#endif
   
   public:
   ArrayS2 <World_Local> aWorldTile; // pointer to World->Local Map array.
   
   World_MapManager();
   ~World_MapManager();

   // Return a World_Local object for this local map. Will generate a local map if necessary. Returns null pointer for invalid request.
   inline World_Local* operator() (const int _x, const int _y);
   
   void addBiome(World_Biome* _biome);

   void init(unsigned int _nX, unsigned int _nY, ArrayS2 <World_Local>* aWorldTile2);
   void main(); // Threaded NPC generator/simulator (per tile)
   void mainBiome(); // threaded biome generator/simulator (per biome)
   
   // Generate map immediately, because we need it.
   bool generateNow(int _mapX, int _mapY);
};

#endif