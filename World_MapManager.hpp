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

#if defined THREAD_ALL
   #include <thread>
   #include <mutex>
   #include <shared_mutex>
   #include <atomic>
#endif

class World_MapManager
{
   int maxWorldsToGenerate; // max number of worlds to generate at once.

   Vector <World_Local*> vMapCache; // Stores all generated maps
   Vector <World_Local*> vJobs; // Vector of maps to be loaded in

#ifdef THREAD_ALL
   std::mutex mutexArrayAccess;
   std::mutex mutexArrayResize;
   std::shared_mutex mutexArrayResize2;
   
   std::mutex mutexVector;
   std::mutex mutexJob;
   
   // keep track of number of threads running
   // to perform safe shutdown
   std::atomic <int> nThreads;
#endif
   
   public:
   ArrayS2 <World_Local> aWorldTile; // pointer to World->Local Map array.
   
   World_MapManager();
   ~World_MapManager();

   // Return a World_Local object for this local map. Will generate a local map if necessary. Returns null pointer for invalid request.
   inline World_Local* operator() (const int _x, const int _y);

   void init(unsigned int _nX, unsigned int _nY, ArrayS2 <World_Local>* aWorldTile2);
   void main(); // main thread loop
};

#endif