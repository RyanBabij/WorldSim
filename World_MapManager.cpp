#pragma once
#ifndef WORLDSIM_WORLD_MAPMANAGER_CPP
#define WORLDSIM_WORLD_MAPMANAGER_CPP

/* WorldSim: World_ThreadManager.cpp
	#include "World_ThreadManager.cpp"

	Manages the threads being run in World class.
   Currently testing, but in future will control
   background world generation, unloading, and
   simulation.

*/

#include "World_MapManager.hpp"


World_MapManager::World_MapManager()
{
   maxWorldsToGenerate=4;

}


void World_MapManager::init(unsigned int _nX, unsigned int _nY)
{
   aWorldTile.initClass(_nX,_nY);
}

void World_MapManager::main()
{
#if defined THREAD_ALL
  // if ( world==0 )
  // {
     // std::cout<<"Error: There is no world.\n";
     // return;
  // }

   std::thread testThread( []
   {
     Sleep(100);
     
     while(true)
     {
        std::cout<<"Cell loading manager tick.\n";
        

        // if ( world->generated )
        // {
           // std::cout<<"World is generated\n";
        // }
        
        Sleep(1000);
     }
   });
   testThread.detach();
   std::cout<<"Note: World Thread Manager started.\n";
#else
   std::cout<<"Note: World Thread Manager disabled.\n";
#endif
}

#endif
