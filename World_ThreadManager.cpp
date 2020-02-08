#pragma once
#ifndef WORLDSIM_WORLD_THREADMANAGER_CPP
#define WORLDSIM_WORLD_THREADMANAGER_CPP

/* WorldSim: World_ThreadManager.cpp
	#include "World_ThreadManager.cpp"

	Manages the threads being run in World class.
   Currently testing, but in future will control
   background world generation, unloading, and
   simulation.

*/

#include "World_ThreadManager.hpp"


World_ThreadManager::World_ThreadManager(World* _world)
{
   world=_world;
   maxWorldsToGenerate=4;
}

void World_ThreadManager::main()
{
#if defined THREAD_ALL
  if ( world==0 )
  {
     std::cout<<"Error: There is no world.\n";
     return;
  }

std::thread testThread( [this]
{
  Sleep(100);
  
  while(true)
  {
     std::cout<<"Cell loading manager tick.\n";
     

     if ( world->generated )
     {
        std::cout<<"World is generated\n";
     }
     
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
