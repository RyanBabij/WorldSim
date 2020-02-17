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
   
   std::cout<<"Saving maps\n";
   for (int i2=0;i2<vMapCache.size();++i2)
   {
      //vMapCache(i2)->save();
      vMapCache(i2)->unload();
   }
   
   std::cout<<"Map manager shutdown.\n";
   MUTEX_SHUTDOWN.unlock();
}

void World_MapManager::init(unsigned int _nX, unsigned int _nY, ArrayS2 <World_Local>* aWorldTile2)
{
   // Passing the old array is a temporary measure. I will probably pass the World Generator instead.
#ifdef THREAD_ALL
   mutexArrayAccess.lock();
   aWorldTile.initClass(_nX,_nY);
   
   mutexJob.lock();
   vJobs.clear();
   
   for (unsigned int _y=0;_y<_nY;++_y)
   {
      for (unsigned int _x=0;_x<_nX;++_x)
      {
         aWorldTile(_x,_y).init(_x,_y,(*aWorldTile2)(_x,_y).baseBiome /* BIOME ID */, 0 /* seed */, 0 /* hasriver */);
         
         if ( (*aWorldTile2)(_x,_y).baseBiome != OCEAN && (*aWorldTile2)(_x,_y).baseBiome != ICE )
         {
            vJobs.push(&(aWorldTile(_x,_y)));
         }
         
      }
   }
   std::cout<<"Jobs initialised\n";
   
   mutexJob.unlock();
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
      mutexArrayAccess.unlock();
      return map;
   }
   else
   {
      mutexArrayAccess.unlock();
   }
#endif
  return 0;
}

bool World_MapManager::generateNow(int _mapX, int _mapY)
{
   std::cout<<"Generating now: "<<_mapX<<", "<<_mapY<<"\n";
   
   // remove from job queue if it's there
   mutexJob.lock();
   for (int i=0;i<vJobs.size();++i)
   {
      if ( vJobs(i)->globalX == _mapX && vJobs(i)->globalY  == _mapY )
      {
         vJobs.removeSlot(i);
         break;
      }
   }
   mutexJob.unlock();
   
   mutexArrayAccess.lock();
   World_Local* local = &aWorldTile(_mapX,_mapY);
   if ( local->threadAccess == false )
   {
      local->threadAccess=true;
   }
   else
   {
      mutexArrayAccess.unlock();
      return false;
   }
   mutexArrayAccess.unlock();
   
   if (QUIT_FLAG) { return false; }
   local->generate(false);
   local->threadAccess=false;
   
   mutexVector.lock();
   vMapCache.push(local);
   mutexVector.unlock();
   return true;
}

void World_MapManager::main()
{
#ifdef THREAD_ALL
   for (int i=0;i<N_CORES;++i)
   {
      std::thread testThread( [this]
      {
         // Wait for the Job vector to be built.
         while(true)
         {
            Sleep(500);
            mutexJob.lock();
            if ( vJobs.size()>0)
            {
               mutexJob.unlock();
               break;
            }
            mutexJob.unlock();
         }
         World_Local* local = 0;

         while(QUIT_FLAG==false)
         {
            local=0;
            // get a job and handle it or wait for one
            while ( local==0 && QUIT_FLAG == false)
            {
               
               mutexJob.lock();
               if (vJobs.size()>0)
               {
                  local = vJobs(0);
                  vJobs.eraseSlot(0);
                  mutexJob.unlock();
                  
                  if (QUIT_FLAG) { return; }
                  mutexArrayAccess.lock();
                  
                  if ( local->threadAccess == false && local->initialized==false )
                  {
                     local->threadAccess=true;
                     mutexArrayAccess.unlock();

                     //  generate the map
                     if (QUIT_FLAG) { return; }
                     local->generate(false);
                     // if (QUIT_FLAG) { return; }
                     // local->save();
                     // if (QUIT_FLAG) { return; }
                     // local->unload();
                     local->threadAccess=false;
                     
                     mutexVector.lock();
                     vMapCache.push(local);
                     
                     // If there are too many maps in cache, find an unused map and free it
                     if ( vMapCache.size() > 0 && vMapCache.size() > MAP_CACHE_SIZE )
                     {
                        World_Local* mapToCache = 0;
                        mutexArrayAccess.lock();
                        for (int i2=0;i2<vMapCache.size();++i2)
                        {
                           if ( vMapCache(i2)->threadAccess==false )
                           {
                              mapToCache = vMapCache(i2);
                              vMapCache.eraseSlot(i2);
                              break;
                           }
                        }
                        mutexArrayAccess.unlock();
                        mutexVector.unlock();
                        
                        // save and unload the map
                        if (mapToCache!=0)
                        {
                           if (QUIT_FLAG) { return; }
                           mapToCache->save();
                           if (QUIT_FLAG) { return; }
                           mapToCache->unload();
                        }
                     }
                     else
                     {
                        mutexVector.unlock();
                     }
                  }
                  else
                  {
                     local=0;
                     mutexArrayAccess.unlock();
                  }
                  
               }
               // there are no jobs available, so wait
               else
               {
                  mutexJob.unlock();
                  Sleep(200);
               }
                  
               if ( QUIT_FLAG )
               { return; }
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
