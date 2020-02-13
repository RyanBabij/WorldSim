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
   nThreads=0;
}

World_MapManager::~World_MapManager()
{
   std::cout<<"Map manager shutting down.\n";
   MUTEX_SHUTDOWN.lock();
   mutexArrayAccess.lock();

   // kinda messy but a wait loop seems to be the
   // best way to cleanly wait for threads to stop
   
   int MAX_WAIT = 10;
   int i=0;
   while (nThreads > 0 && i<MAX_WAIT  )
   {
      Sleep(50);
      ++i;
      
      if ( i==MAX_WAIT )
      {
         std::cout<<"Hanging detected.\n";
      }
   }
   std::cout<<"Map manager shutdown.\n";
   MUTEX_SHUTDOWN.unlock();
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
   //mutexArrayAccess.lock();
   if ( aWorldTile.isSafe(_x,_y) )
   {
      World_Local* map = &aWorldTile(_x,_y);
//mutexArrayAccess.lock();
      //map->generate();
      //mutexArrayAccess.unlock();
      return map;
   }
   else
   {
      //mutexArrayAccess.unlock();
   }
   


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
   if ( aWorldTile(_x,_y).threadAccess == false && aWorldTile(_x,_y).initialized==false )
   {
      aWorldTile(_x,_y).threadAccess=true;
      
      World_Local* const local = &aWorldTile(_x,_y);
      mutexArrayAccess.unlock();

      if (QUIT_FLAG) { return; }
      local->generate(false);
      if (QUIT_FLAG) { return; }
      local->generateSubterranean();
      if (QUIT_FLAG) { return; }
      local->save();
      if (QUIT_FLAG) { return; }
      local->unload();
      if (QUIT_FLAG) { return; }
      
      local->threadAccess=false;
   }
   else
   {
      mutexArrayAccess.unlock();
   }
#endif
}

void World_MapManager::main()
{
#ifdef THREAD_ALL
   for (int i=0;i<N_CORES;++i)
   {
      std::thread testThread( [this]
      {
         unsigned int x=0;
         unsigned int y=0;

         while(QUIT_FLAG==false)
         {
            if ( QUIT_FLAG )
            {
               break;
            }
            
            if (RELINQUISH_CPU)
            {
               //MsgWaitForMultipleObjects( 0, NULL, FALSE, 10, QS_ALLINPUT ); /* parameter 4 is milliseconds ie 1000 = 1 second. */
            }

            if ( aWorldTile.isSafe(x,y) )
            {
               //mutexArrayAccess.unlock();
               if ( aWorldTile(x,y).baseBiome != OCEAN )
               { generate(x,y);
               }
               
               ++x;
            }

            else if ( aWorldTile.isSafe(x,y) == false )
            {
               x=0;
               ++y;

               if ( aWorldTile.isSafe(x,y) == false )
               {
                  x=0;
                  y=0;
               }
            }
         }
         --nThreads;
      });
      ++nThreads;
      testThread.detach();
   }
   std::cout<<"Note: World Thread Manager started.\n";
#else
   std::cout<<"Note: World Thread Manager disabled.\n";
#endif
}

#endif
