#pragma once
#ifndef WORLDSIM_WORLD_MAPMANAGER_CPP
#define WORLDSIM_WORLD_MAPMANAGER_CPP

/* WorldSim: World_ThreadManager.cpp
#include "World_ThreadManager.cpp" */

/**
	Manages the threads being run in World class. Currently testing, but in future will control
	background world generation, unloading, and simulation.
*/

#include <System/Sleep/Sleep.hpp>

#include "World_MapManager.hpp"

World_MapManager::World_MapManager()
{
	maxWorldsToGenerate=4;
#ifdef WILDCAT_THREADING
	nThreads=0;
	nBiomeThreads=0;
#endif
}

World_MapManager::~World_MapManager()
{
	std::cout<<"Map manager shutting down.\n";   
#ifdef WILDCAT_THREADING
	MUTEX_SHUTDOWN.lock();
	mutexArrayAccess.lock();
	// kinda messy but a wait loop seems to be the
	// best way to cleanly wait for threads to stop
	int MAX_WAIT = 10;
	int i=0;
	while (nThreads > 0 && nBiomeThreads > 0 && i<MAX_WAIT  )
	{
		sleep(25);
		++i;
		if ( i==MAX_WAIT )
		{
			std::cout<<"Hanging detected.\n";
		}
	}
#endif
	std::cout<<"Saving maps\n";
	for (int i2=0;i2<vMapCache.size();++i2)
	{
		//vMapCache(i2)->save();
		//vMapCache(i2)->unload();
	}
	std::cout<<"Map manager shutdown.\n";
#ifdef WILDCAT_THREADING
	MUTEX_SHUTDOWN.unlock();
#endif
}

void World_MapManager::init(unsigned int _nX, unsigned int _nY, ArrayS2 <World_Local>* aWorldTile2)
{
	// Passing the old array is a temporary measure. I will probably pass the World Generator instead.
	#ifdef WILDCAT_THREADING
	mutexArrayAccess.lock();
	aWorldTile.initClass(_nX,_nY);

	mutexJob.lock();
	vJobs.clear();

	for (unsigned int _y=0;_y<_nY;++_y)
	{
		for (unsigned int _x=0;_x<_nX;++_x)
		{
			// tile is already inited on worldgen
			//aWorldTile(_x,_y).init(_x,_y,(*aWorldTile2)(_x,_y).baseBiome /* BIOME ID */, 0 /* seed */, 0 /* hasriver */);
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
#ifdef WILDCAT_THREADING
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
#ifdef WILDCAT_THREADING

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

	if (QUIT_FLAG)
	{ return false; }
	local->generate(false);
	local->threadAccess=false;

	mutexVector.lock();
	vMapCache.push(local);
	mutexVector.unlock();
#else

	for (int i=0;i<vJobs.size();++i)
	{
		if ( vJobs(i)->globalX == _mapX && vJobs(i)->globalY  == _mapY )
		{
			vJobs.removeSlot(i);
			break;
		}
	}

	World_Local* local = &aWorldTile(_mapX,_mapY);

	local->generate(false);
	vMapCache.push(local);

#endif
	return true;
}

void World_MapManager::addBiome(World_Biome* _biome)
{
	vBiome.push(_biome);
}

void World_MapManager::main()
{
	return;
#ifdef WILDCAT_THREADING
	// due to file IO there may be reason to maintain more threads than cores.
	for (int i=0;i<N_CORES;++i)
	{
		std::thread testThread( [this,i]
		{
			const int threadID = i;
			// Wait for the Job vector to be built.
			while(true)
			{
				sleep(500);
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

						if (QUIT_FLAG)
						{ return; }
						mutexArrayAccess.lock();

						if ( local->threadAccess == false && local->initialized==false )
						{
							local->threadAccess=true;
							mutexArrayAccess.unlock();

							//  generate the map
							if (QUIT_FLAG)
							{ return; }
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
										vMapCache(i2)->threadAccess=true;
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
									mutexCout.lock();
									//std::cout<<"Caching map: "<<mapToCache->globalX_TS<<", "<<mapToCache->globalY_TS<<"\n";
									mutexCout.unlock();
									if (QUIT_FLAG) { return; }
									mapToCache->save();
									if (QUIT_FLAG) { return; }
									mapToCache->unload();
									mapToCache->threadAccess=false;
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
						sleep(200);
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

// updates biome ticks in background
void World_MapManager::mainBiome(const unsigned short int sleepTime=0)
{
	std::cout<<"MAIN BIOME\n";
	// due to file IO there may be reason to maintain more threads than cores.
	// however per-biome simulation is memory intensive so we should be careful
	// biomes should be processed in an order which prevents loading multiple huge
	// biomes.
	for (unsigned short int i=0;i<N_CORES;++i)
	{
		std::thread biomeThread( [this,i, sleepTime]
		{
			mutexCout.lock();
			std::cout<<"Launching biome thread "<<i<<"\n";
			mutexCout.unlock();
			while ( true && QUIT_FLAG == false)
			{
				sleep(sleepTime);
				World_Biome* biome=0;

				// select a biome that needs work
				mutexBiome.lock();
				for(int i2=0;i2<vBiome.size();++i2)
				{
					if ( vBiome(i2)->type!=OCEAN && vBiome(i2)->type!= ICE
					&& vBiome(i2)->isGenerated == false && vBiome(i2)->threadAccess == false )
					{
						vBiome(i2)->threadAccess = true;
						mutexCout.lock();
						std::cout<<"Generating biome id: "<<vBiome(i2)->id<<".\n";
						mutexCout.unlock();

						biome=vBiome(i2);
						break;
					}
				}
				mutexBiome.unlock();

				if (biome)
				{
					biome->generate();
					mutexBiome.lock();
					biome->threadAccess = false;
					biome->isGenerated=true;
					mutexBiome.unlock();
				}
				else
				{
					mutexCout.lock();
					std::cout<<"All biomes generated. Entering simulation mode.\n";
					mutexCout.unlock();
					break;
				}
			}
			while (true && QUIT_FLAG == false)
			{
				sleep(1000);
			}
			--nBiomeThreads;
		});
		++nBiomeThreads;
		biomeThread.detach();
	}
}

#endif
