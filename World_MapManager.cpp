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

void World_MapManager::generate(unsigned int _x, unsigned int _y)
{
   mutexArrayAccess.lock();
   if ( aWorldTile(_x,_y).threadAccess == false )
   {
      std::cout<<"Generating world: "<<_x<<", "<<_y<<"\n";
      aWorldTile(_x,_y).threadAccess=true;
      mutexArrayAccess.unlock();
      Sleep(Random::randomInt(2000)+1000);
      //std::cout<<"World "<<_x<<", "<<_y<<"\n";
   }
   else
   {
      mutexArrayAccess.unlock();
   }
}

bool canGenerate(unsigned int _x, unsigned int _y)
{
   return true;
}

void World_MapManager::main()
{
#if defined THREAD_ALL
  // if ( world==0 )
  // {
     // std::cout<<"Error: There is no world.\n";
     // return;
  // }
  
  for (int i=0;i<4;++i)
  {
   std::thread testThread( [this, i]
   {
      Sleep(100);

      unsigned int x=0;
      unsigned int y=0;

      while(true)
      {
         //std::cout<<"Cell loading manager tick: "<<i<<"\n";
         
         if ( aWorldTile.nX == 0 || aWorldTile.nY == 0 )
         {
            Sleep(600);
            std::cout<<"Not init yet\n";
            continue;
         }
         
         if ( aWorldTile.isSafe(x,y) )
         {
            generate(x,y);
            ++x;
         }
         
         if ( aWorldTile.isSafe(x,y) == false )
         {
            x=0;
            ++y;
            
            if ( aWorldTile.isSafe(x,y) == false )
            {
               x=0;
               y=0;
            }
         }

        Sleep(100);
     }
   });
   testThread.detach();
  }
   std::cout<<"Note: World Thread Manager started.\n";
#else
   std::cout<<"Note: World Thread Manager disabled.\n";
#endif
}

#endif
