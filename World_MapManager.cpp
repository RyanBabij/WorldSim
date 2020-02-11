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
#ifdef THREAD_ALL
   mutexArrayAccess.lock();
   aWorldTile.initClass(_nX,_nY);
   
   for (unsigned int _y=0;_y<_nY;++_y)
   {
      for (unsigned int _x=0;_x<_nX;++_x)
      {
         aWorldTile(_x,_y).init(_x,_y,GRASSLAND /* BIOME ID */, 0 /* seed */, 0 /* hasriver */);
      }
   }
   
   mutexArrayAccess.unlock();
#endif
}

World_Local* World_MapManager::operator() (const int _x, const int _y)
{
#ifdef THREAD_ALL
   mutexArrayAccess.lock();
   if ( aWorldTile.isSafe(_x,_y) )
   {
      World_Local* map = &aWorldTile(_x,_y);
      map->generate();
      mutexArrayAccess.unlock();
      return map;
   }
   mutexArrayAccess.unlock();


  // for (int i=0;i<vWorldLocal.size();++i)
  // {
    // if (vWorldLocal(i)->globalX == _x && vWorldLocal(i)->globalY == _y )
    // {
      // return vWorldLocal(i);
    // }
  // }
  
  // // The local map isn't in memory, therefore we need to load it up.
  // // For now we just generate it from scratch.
  // generateLocal(_x,_y);

  // for (int i=0;i<vWorldLocal.size();++i)
  // {
    // if (vWorldLocal(i)->globalX == _x && vWorldLocal(i)->globalY == _y )
    // {
      // return vWorldLocal(i);
    // }
  // }
#endif
  return 0;
}


void World_MapManager::generate(unsigned int _x, unsigned int _y)
{
#ifdef THREAD_ALL
   mutexArrayAccess.lock();
   if ( aWorldTile(_x,_y).threadAccess == false )
   {
      std::cout<<"Generating world: "<<_x<<", "<<_y<<"\n";
      aWorldTile(_x,_y).threadAccess=true;
      
      World_Local* local = &aWorldTile(_x,_y);
      mutexArrayAccess.unlock();

      local->generate();
      //local->active=true;
      //local->initialized=true;
      //local=

   }
   else
   {
      mutexArrayAccess.unlock();
   }
#endif
}

bool canGenerate(unsigned int _x, unsigned int _y)
{
   return true;
}

void World_MapManager::main()
{
#ifdef THREAD_ALL
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
