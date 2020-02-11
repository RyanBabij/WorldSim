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
   #include <atomic>
#endif


class World_MapManager
{
   int maxWorldsToGenerate; // max number of worlds to generate at once.
   // Vector <World_Local*> vGenningMaps; // vector of worlds currently generating.
   // Vector <World_Local*> vGeneratedMaps; // vector of worlds generated
   // Vector <World_Local*> vMapsToUnload; // vector of worlds to cache to disk

   ArrayS2 <World_Local> aWorldTile; // pointer to World->Local Map array.
   
#ifdef THREAD_ALL
   std::mutex mutexArrayAccess;
#endif
   
   public:
//   World* world;

   // Return a World_Local object for this local map. Will generate a local map if necessary. Returns null pointer for invalid request.
   inline World_Local* operator() (const int _x, const int _y);

   void init(unsigned int _nX, unsigned int _nY);
   void generate(unsigned int _nX, unsigned int _nY);
   
   World_MapManager();
   void main();
};

#endif