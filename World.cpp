#pragma once
#ifndef WORLDSIM_WORLD_CPP
#define WORLDSIM_WORLD_CPP

/* WorldSim: World.cpp
#include "World.cpp"

Implementation of "World.hpp".
*/

#include "World.hpp"

#include "Mythology.cpp"
#include "Mythology_Deity.cpp"

#include "Civ.hpp"
	#include "Civ_Dwarven.hpp"
#include "Tribe.hpp"
	#include "Tribe_Human.hpp"
	#include "Tribe_Dwarven.hpp"
	#include "Tribe_Elf.hpp"
#include "Character.hpp"
#include "Settlement.hpp"
	#include "Settlement_Dwarven.hpp"
#include "WorldObjectGlobal.hpp"
#include "WorldObject.hpp"
	#include "WorldObject_Tree.hpp"
	#include "WorldObject_Rock.hpp"
	#include "WorldObject_Sign.hpp"
#include "Item.hpp"
#include "World_MapManager.cpp"

#include <Game/WorldGenerator/Biome.hpp>
#include <Game/Language/NameGenerator.cpp>
#include <Game/WorldGenerator/WorldGenerator2.hpp>
#include <File/FileLog.hpp>
#include <System/Time/Timer.hpp> // To manage work chunks and benchmark worldgen time.

#if defined WILDCAT_THREADING
	#include <thread>
	#include <mutex>
	#include <atomic>
#endif

World::World(): SaveFileInterface(),/* mapManager(this),*/ seaLevel(0), mountainLevel(0)
{
	random.seed();

	nX=-1; nY=-1;

	simX=0; simY=0;

	maximumX = 0; maximumY = 0;

	active=false;
	generated=false;

	ticksBacklog=0;
	relinquishTimer.init();

	name="";

	dailyCounter=0;
	monthlyCounter=0;

	queryWorldX = -1; queryWorldY = -1;

	worldFilePath = "";

	isRaining=false;


	
	incrementContinuous=false;
	
	deityTicks=0;

	//mapManager.main();
}

World::~World()
{
	QUIT_FLAG=true;
}

// Access local maps using the brackets operator.
// Important note: Using this generates local maps on demand, therefore you should be very careful how you use it.
// And using it before entering World Simulator is a very bad idea because it basically cascades into spawning objects
// and stuff which doesn't exist at this point.

World_Local* World::operator() (const int _x, const int _y)
{
	if (active==false) // active is true when we have finished world gen
	{
		// don't call this before world simulator
		return 0;
	}
	
	for (int i=0;i<vWorldLocal.size();++i)
	{
		if (vWorldLocal(i)->globalX == _x && vWorldLocal(i)->globalY == _y )
		{
			return vWorldLocal(i);
		}
	}

	// The local map isn't in memory, therefore we need to load it up.
	// For now we just generate it from scratch.
	generateLocal(_x,_y);

	for (int i=0;i<vWorldLocal.size();++i)
	{
		if (vWorldLocal(i)->globalX == _x && vWorldLocal(i)->globalY == _y )
		{
			return vWorldLocal(i);
		}
	}

	return 0;
}

inline LocalTile* World::operator() (unsigned long int _x, unsigned long int _y, const bool subterranean)
{
	// CONVERT COORDINATES TO RELATIVE.
	int gX = 0;
	int gY = 0;
	int lX = 0;
	int lY = 0;

	if ( absoluteToRelative(_x,_y,&gX,&gY,&lX,&lY) == false )
	{
		return 0;
	}

	// Check if local map is already loaded.
	for (int i=0;i<vWorldLocal.size();++i)
	{
		if (vWorldLocal(i)->globalX == gX && vWorldLocal(i)->globalY == gY && vWorldLocal(i)->data )
		{
			if ( subterranean && vWorldLocal(i)->dataSubterranean != 0 )
			{
				return &vWorldLocal(i)->dataSubterranean->aSubterranean(lX,lY);
			}
			return &vWorldLocal(i)->data->aLocalTile(lX,lY);
		}
	}
	// The local map isn't in memory, therefore we need to load it up.
	// For now we just generate it from scratch.
	generateLocal(gX,gY);

	for (int i=0;i<vWorldLocal.size();++i)
	{
		if (vWorldLocal(i)->globalX == gX && vWorldLocal(i)->globalY == gY && vWorldLocal(i)->data )
		{
			if ( subterranean )
			{
				return &(vWorldLocal(i)->dataSubterranean->aSubterranean(lX,lY));
			}
			return &(vWorldLocal(i)->data->aLocalTile(lX,lY));
		}
	}

	return 0;
}

inline LocalTile* World::operator() (HasXY2 <unsigned long int>* _xy, const bool subterranean)
{
	return (*this)(_xy->x,_xy->y,subterranean);
}

// COORDINATE CONVERSIONS

// Pass absolute coordinates and recieve world and local relative coordinates.
// This currently doesn't work if tileX or tileY are negative.
bool World::absoluteToRelative (const unsigned long int _absoluteX, const unsigned long int _absoluteY, int * _globalX, int * _globalY, int * _localX, int * _localY)
{
	if ( _globalX == 0 || _globalY == 0 || _localX == 0 || _localY == 0 )
	{ return false; }

	// overflow check
	if (_absoluteX > maximumX || _absoluteY > maximumY )
	{
		*_globalX = 0;
		*_globalY = 0;
		*_localX = 0;
		*_localY = 0;
		return false;
	}

	*_globalX = _absoluteX / LOCAL_MAP_SIZE;
	*_globalY = _absoluteY / LOCAL_MAP_SIZE;

	if ( *_globalX == 0 )
	{ *_localX = _absoluteX; }
	else
	{ *_localX = _absoluteX % LOCAL_MAP_SIZE; }

	if ( *_globalY == 0 )
	{ *_localY = _absoluteY; }
	else
	{ *_localY = _absoluteY % LOCAL_MAP_SIZE; } 
	return true;
}

bool World::generateMythology()
{
	//Mythology mythology;
	
	std::cout<<"Generating mythology\n";
	
	//mythology.addDeity("Creator", Mythology::none);
	
	return true;
}

// This should be called ONCE when transitioning from worldgen into World Simulator.
// Some World functionality won't work until this has been called... Use the "active" flag to check for this.
void World::startSimulation()
{
	// BIOME INIT STUFF SHOULD GO HERE NOT IN WORLDGEN
	std::cout<<"Preparing world for simulation...\n";

	// BUILD LANDMASS ID ARRAY
	std::thread tLandmass ( [this]
	{
		int currentLandmassID = 0;
		int currentOceanID = 0;

		std::cout<<"Building landmass info.\n";

		// first build an array of isLand() to do flood fill easier
		ArrayS2 <bool> aIsLand(nX,nY,false);
		for ( int _y=0;_y<nY;++_y)
		{
			for ( int _x=0;_x<nX;++_x)
			{
				aIsLand(_x,_y)=aWorldTile(_x,_y).isLand();
			}
		}


		for ( int _y=0;_y<nY;++_y)
		{
			for ( int _x=0;_x<nX;++_x)
			{
				//if (aWorldTile(_x,_y).landID == -1 && aIsLand(_x,_y) == true )
				if (aWorldTile(_x,_y).landID == -1 && aWorldTile(_x,_y).isLand() )
				{
					Vector <HasXY*>* vFill = aIsLand.floodFillVector(_x,_y,false);

					for (int i=0;i<vFill->size();++i)
					{
						HasXY* v = (*vFill)(i);
						aWorldTile(v->x,v->y).landID = currentLandmassID;
					}

					World_Landmass* landmass = new World_Landmass;
					landmass->name = globalNameGen.generate();
					landmass->size = vFill->size();
					landmass->getAverageCoordinates(vFill);
					vLandmass.push(landmass);

					vFill->deleteAll();
					delete vFill;

					++currentLandmassID;
				}
				else if (aBiomeID(_x,_y) == -1 && aIsLand(_x,_y) == false)// fill ocean
				{

				}

			}
		}
		std::cout<<"Filled "<<currentLandmassID<<" landmasses.\n";

	});
	tLandmass.join();

	// Trying another algorithm
	std::cout<<"Building biome info.\n";

	Timer timerBiomeFill;
	timerBiomeFill.init();
	timerBiomeFill.start();

	//int currentID3=0;
	//int currentID = 0;

	// Here I am spawning 1 thread for every possible biome. Each thread then processes biomes that have that type.
	// Performance is significantly improved and I'm getting solid 100% CPU on my quad core. It's important to `
	// a small number of large threads, rather than a large number of small threads. I tried the latter and I only gained
	// a few seconds on large maps, and small maps generated slower. This new approach is more than 3x faster.
	// threaded - cb - 2049 - seed 12345: 6.61 7.15 6.98        Biome time: 2.48, 2.46, 2.44
	// unthreaded - cb - 2049 - seed 12345: 12.47 13.02 13.06   Biome time: 8.25, 8.17, 8.16
	std::thread * aThread [N_BIOMES] = { 0 };
	std::mutex m;
	std::atomic<int> biomeID=0; /* Atomic is generally more efficient than using lock because of OS-level support. */
	Mutex biomeIncrement;
	int currentBiomeID=0; /* Every unique biome gets a unique ID, which will make later processing much easier. */
	
	// The threaded biome fill works by looking for a biome is hasn't seen before. It then spawns a new search
	// thread from this coordinate, which will then fill all biomes of that type from that point onward.
	
	// BUILD BIOME VECTOR
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			if (aBiomeID(_x,_y) == -1 && aThread[aTerrain(_x,_y)] == 0)
			{
				const int biomeSearch = aTerrain(_x,_y);
				// Spawn a thread to process this biome type
				aThread [aTerrain(_x,_y)] = new std::thread ( [ this, _x, _y, biomeSearch, /* &m, */ &biomeID, &biomeIncrement ]
				{
					// This technically works because same biomes can never touch. However it's asking for trouble so we should change it.
					// biomeIncrement.lock();
					// std::cout<<"Biome increment: "<<biomeID<<" to "<<biomeID+1<<"\n";
					int thisBiomeID;
					// ++biomeID;
					// biomeIncrement.unlock();
					
					int startX = _x;

					// Begin scanning from the current x and y of the main scanning loop.
					// Remember that x2 needs to reset to 0 after the first row.
					for ( int _y2=_y;_y2<nY;++_y2)
					{
						for ( int _x2=startX;_x2<nX;++_x2)
						{
							if (aTerrain(_x2,_y2) == biomeSearch && aBiomeID(_x2,_y2) == -1)
							{
								World_Biome* biome = new World_Biome;
								biome->type = biomeSearch;
								biomeIncrement.lock();
								thisBiomeID = biomeID;
								biome->id = thisBiomeID;
								++biomeID;
								// The biome is immediately pushed onto the vector when assigned an ID, to ensure the position
								// on the vector corresponds with the Vector index. This makes later searches more convenient.
								vBiome.push(biome);
								biomeIncrement.unlock();
								mutexCout.lock();
								//std::cout<<"assigning biome id: "<<thisBiomeID<<"\n";
								mutexCout.unlock();

								// Perform a flood fill from this point to find all connected tiles of this type, this defines
								// the biome.
								Vector <HasXY*>* vFill = aTerrain.floodFillVector(_x2,_y2,true);
								for (int i=0;i<vFill->size();++i)
								{
									HasXY* v = (*vFill)(i);
									aBiomeID(v->x,v->y) = thisBiomeID;
									biome->vXY.push(HasXY((*vFill)(i)->x,(*vFill)(i)->y));
									// give the map to the biome object to keep track of.
									biome->addMap(&aWorldTile((*vFill)(i)->x,(*vFill)(i)->y));
								}
								biome->size = vFill->size();
								biome->getAverageCoordinates();
								
								// free up flood fill memory
								vFill->deleteAll();
								delete vFill;
							}
						}
						// x2 must reset to 0 after scanning the first partial row.
						startX=0;
					}
				});
			}
		}
	}
	
	// Let all threads finish.
	for (int i=0; i<N_BIOMES; ++i)
	{
		if ( aThread[i]!= 0)
		{
			aThread[i]->join();
			delete aThread[i];
			aThread[i]=0;
		}
	}

	// Name the biomes
	// and generate flora
	for (int i=0;i<vBiome.size();++i)
	{
		int biomeType = vBiome(i)->type;
		if ( biomeType == OCEAN)
		{
			if ( vBiome(i)->size < LOCAL_MAP_SIZE )
			{
				vBiome(i)->name = "Lake " + globalNameGen.generate();
			}
			else
			{
				vBiome(i)->name = "Ocean of "+globalNameGen.generate();
			}
		}
		else
		{
			vBiome(i)->name = globalNameGen.generate();
		}

		if ( biomeType == MOUNTAIN)
		{
			if ( vBiome(i)->size == 1 )
			{
				vBiome(i)->name = "Mount " + globalNameGen.generate();
			}
			else
			{
				vBiome(i)->name = globalNameGen.generate() + " Mountains";
			}
		}
		// convert biome names to all upper case for now to make sorting easier.
		for (auto & c: vBiome(i)->name) c = toupper(c);
		
		//std::cout<<"Generating Flora and Creatures for "<<vBiome(i)->name<<"\n";
		vBiome(i)->generateFlora();
		vBiome(i)->generateCreatures();
	}
	
	std::cout<<"Filled "<<vBiome.size()<<" biomes.\n";

	//isRaining=true;
	isRaining=false;

	active=true;
	mapManager.init(nX,nY,&aWorldTile);

	for (int i=0;i<vBiome.size();++i)
	{
		mapManager.addBiome(vBiome(i));
	}
	
	// Now the world is fully generated, we can init the Civs.
	for (int i=0; i<vCiv.size(); ++i)
	{
		vCiv(i)->initSimulation();
	}

	// launch tile caching manager
	//mapManager.main();
	// launch biome caching manager
	mapManager.mainBiome();
}

void World::nameRegions()
{
}

bool World::putObject(WorldObjectGlobal* _object, int x /* =-1 */, int y /* =-1 */)
{
	if ( x != -1 && y != -1 )
	{
		_object->worldX=x;
		_object->worldY=y;
	}

	if ( isSafe(_object->worldX, _object->worldY) )
	{
		if(vWorldObjectGlobal.contains(_object)==false)
		{
			vWorldObjectGlobal.push(_object);
			return true;
		}
		else
		{
			std::cout<<"Debug: WorldObjectGlobal was already placed.\n";
		}
	}

	std::cout<<"ERROR: Error placing worldobject.\n";

	return false;
}

bool World::removeObject(WorldObjectGlobal* _object)
{
	vWorldObjectGlobal.remove(_object);
	return false;
}

Vector <WorldObjectGlobal*>* World::getNeighboringObjects(WorldObjectGlobal* _obj)
{
	Vector <WorldObjectGlobal*>* vNearbyObjects = new Vector <WorldObjectGlobal*>;

	for (int i=0;i<vWorldObjectGlobal.size();++i)
	{
		WorldObjectGlobal* _o = vWorldObjectGlobal(i);
		if (_o != _obj)
		{
			if ( _obj->distanceTo(_o) <= 1 )
			{
				vNearbyObjects->push(_o);
			}
		}
	}
	return vNearbyObjects;
}

bool World::isSafe(int _x, int _y)
{
	return ( _x >= 0 && _x < nX && _y >= 0 && _y < nY );
}

bool World::isSafe(unsigned long int _x, unsigned long int _y)
{
	return ( _x <= maximumX && _y <= maximumY );
}

// Return true if the map for this tile has been generated
bool World::isGenerated(unsigned long int _absoluteX, unsigned long int _absoluteY)
{
	if ( isSafe (_absoluteX, _absoluteY) )
	{
		int gX,gY,lX,lY;
		if (absoluteToRelative(_absoluteX,_absoluteY,&gX,&gY,&lX,&lY))
		{
			for (int i=0;i<vWorldLocal.size();++i)
			{
				if (vWorldLocal(i)->globalX == gX && vWorldLocal(i)->globalY == gY )
				{
					return true;
				}
			}
		}
	}
	return false;
}


Character* World::getRandomCharacter()
{
	//Get total world population.
	Character* chosenChar = 0;

	int totalPop = 0;
	for ( int i=0; i<vTribe.size(); ++i )
	{
		totalPop += vTribe(i)->vCharacter.size();
	}

	int randomChar = Random::randomInt(totalPop-1);

	for (int i=0;i<vTribe.size();++i)
	{
		if (randomChar < vTribe(i)->vCharacter.size() )
		{
			return vTribe(i)->vCharacter(randomChar);
		}
		else
		{
			randomChar -= vTribe(i)->vCharacter.size();
		}
	}
	return 0;
}

//Return a vector of coordinates visible from the given location.
Vector <HasXY2 <unsigned long int> *> * World::rayTraceLOS (unsigned long int _x, unsigned long int _y, const int RANGE, const bool isSneaking = false, bool subterranean)
{
	if (RANGE <= 0)
	{ return 0; }

	if (_x > maximumX || _y > maximumY )
	{ return 0; }

	if (FOG_OF_WAR == false)
	{
		auto vVisibleTiles = new Vector <HasXY2 <unsigned long int> *>;
		return vVisibleTiles;
	}

	//Step 1: Get all raytrace coordinates.
	Vector <HasXY2 <unsigned long int> *> rayTraceCoordinates;

	unsigned long int rayX = _x-RANGE;

	//overflow check
	if ( rayX > maximumX )
	{ rayX = 0; }

	unsigned long int rayY = _y-RANGE;

	//overflow check
	if ( rayY > maximumY )
	{ rayY = 0; }

	unsigned long int rayMaxX = _x+RANGE;

	//overflow check
	if ( rayMaxX > maximumX )
	{ rayMaxX = maximumX; }

	unsigned long int rayMaxY = _y+RANGE;

	//overflow check
	if ( rayMaxY > maximumY )
	{ rayMaxY = maximumY; }

	unsigned long int tempX = rayX;
	unsigned long int tempY = rayY;

	auto hXY = new HasXY2 <unsigned long int>;

	rayTraceCoordinates.push( new HasXY2 <unsigned long int> (tempX,tempY) );

	while (tempX <= rayMaxX)
	{
		rayTraceCoordinates.push( new HasXY2 <unsigned long int> (tempX,rayY) );
		rayTraceCoordinates.push( new HasXY2 <unsigned long int> (tempX,rayMaxY) );
		++tempX;
	}

	// On the Y pass we prevent doing the corners again.
	++tempY;
	while (tempY < rayMaxY)
	{
		rayTraceCoordinates.push( new HasXY2 <unsigned long int> (rayX,tempY) );
		rayTraceCoordinates.push( new HasXY2 <unsigned long int> (rayMaxX,tempY) );
		++tempY;
	}

	// We now have a list of coordinates to raytrace.
	auto vVisibleTiles = new Vector <HasXY2 <unsigned long int> *>;


	for (int i=0;i<rayTraceCoordinates.size();++i)
	{
		rayTrace (_x,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles, subterranean);

		// Very bad implementation of peeking. Should be optimised in future.
		if (isSneaking)
		{
			rayTrace (_x+1,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
			rayTrace (_x-1,_y,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
			rayTrace (_x,_y+1,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
			rayTrace (_x,_y-1,rayTraceCoordinates(i)->x,rayTraceCoordinates(i)->y,vVisibleTiles);
		}
	}

	return vVisibleTiles;
}

// This should probably be an external game lib
void World::rayTrace (unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2, Vector <HasXY2 <unsigned long int> *> * vVisibleTiles, bool subterranean)
{
	int xDiff = 0;
	if ( _x1 > _x2 )
	{ xDiff = _x2 - _x1; }
	else if ( _x2 > _x1 )
	{ xDiff = _x1 - _x2; }

	int yDiff = 0;
	if ( _y1 > _y2 )
	{ yDiff = _y2 - _y1; }
	else if ( _y2 > _y1 )
	{ yDiff = _y1 - _y2; }

	double slope = BasicMath::getSlopeULI(_x1,_y1,_x2,_y2);

	// IF SLOPE IS INFINITY, CHANGE VALUE TO 0.
	if ( slope == std::numeric_limits<double>::infinity() )
	{ slope=0; }

	// SPECIAL CASE: 1 TILE.
	// REVEAL TILE STANDING ON.
	if ( (_x1==_x2) && (_y1==_y2) )
	{
		if ( isSafe(_x1,_y2) )
		{
			auto temp = new HasXY2 <unsigned long int> (_x1,_y1);

			// Proper optimisation would use a set, however for now we'll just do a pre-check
			// to prevent storing duplicates.
			// Update: I removed the dupe detection as it significantly hurts performance.
			// The renderer must iterate over the dupes, however it seems to be able to handle it better for now.
			//if ( vVisibleTiles->contains(temp) == false )
			if (true)
			{
				vVisibleTiles->push( temp );
			}
			else
			{
				delete temp;
			}
		}
	}

	// SPECIAL CASE: UP/DOWN
	else if (_x1 == _x2)
	{
		do
		{
			if ( isSafe(_x1,_y1) )
			{
				auto temp = new HasXY2 <unsigned long int> (_x1,_y1);

				//if ( vVisibleTiles->containsPtr(temp) == false )
				if (true)
				{
					vVisibleTiles->push( temp );
				}
				else
				{
					delete temp;
				}

				LocalTile * lt = (*this)(_x1, _y1,subterranean);
				if (lt != 0 && lt->hasViewBlocker())
				{ break; }
			}
			if ( _y1 < _y2 )
			{ ++_y1; }
			else
			{ --_y1; }
		} 
		while( _y1 != _y2 );

		// Final case
		if (_y1 == _y2)
		{
			vVisibleTiles->push(new HasXY2 <unsigned long int> (_x1,_y1) );
		}
	}
	//SHALLOW SLOPE
	else if ( xDiff <= yDiff )
	{
		if (_x1>_x2)
		{ slope*=-1; }

		double currentY = _y1;

		//while (_x1 != _x2 )
		do
		{
			unsigned long int roundedY = 0;

			if ( _y1 > _y2 )
			{
				roundedY = floor(currentY);
			}
			else if ( _y1 < _y2 )
			{
				roundedY = ceil(currentY);
			}
			else
			{
				roundedY = round(currentY);
			}

			if ( isSafe(_x1,roundedY) == true )
			{
				auto temp = new HasXY2 <unsigned long int> (_x1,roundedY);

				//if ( vVisibleTiles->containsPtr(temp) == false )
				if (true)
				{
					vVisibleTiles->push( temp );
				}
				else
				{
					delete temp;
				}

				LocalTile * lt = (*this)(_x1, roundedY, subterranean);
				if (lt != 0 && lt->hasViewBlocker())
				{ break; }

				currentY+=slope;
			}
			if (_x1<_x2)
			{ ++_x1; }
			else
			{ --_x1; }
		}
		while (_x1 != _x2 );

		// Final case
		if (_x1 == _x2)
		{
			vVisibleTiles->push(new HasXY2 <unsigned long int> (_x1,_y2) );
		}
		//Final case
		//vVisibleTiles->push(new HasXY (_x1,_y1) );
	}
	//STEEP SLOPE.
	else
	{
		slope = BasicMath::getSlopeULI(_y1,_x1,_y2,_x2);

		if (_y1>_y2)
		{ slope*=-1; }

		double currentX = _x1;

		//while (_y1 != _y2 )
		do
		{
			unsigned long int roundedX=0;

			if ( _x1 > _x2 )
			{ roundedX = floor(currentX); }
			else if ( _x1 < _x2 )
			{ roundedX = ceil(currentX); }
			else
			{ roundedX = round(currentX); }

			if ( isSafe(roundedX,_y1) == true )
			{
				auto temp = new HasXY2 <unsigned long int> (roundedX,_y1);

				//if ( vVisibleTiles->containsPtr(temp) == false )
				if (true)
				{
				vVisibleTiles->push( temp );
				}
				else
				{
				//std::cout<<"DUPE DETECTED\n";
				delete temp;
				}

				LocalTile * lt = (*this)(roundedX, _y1, subterranean);
				if (lt != 0 && lt->hasViewBlocker())
				{ break; }

				currentX+=slope;
			}
			if (_y1<_y2)
			{ ++_y1; }
			else
			{ --_y1; }
		}
		while (_y1 != _y2 );

		// Final case
		if (_y1 == _y2)
		{
			vVisibleTiles->push(new HasXY2 <unsigned long int> (_x2,_y1) );
		} 
		//Final case
		//vVisibleTiles->push(new HasXY (_x1,_y1) );
	}

}



void World::generateTribes( int nTribesHuman = DEFAULT_NUMBER_TRIBES_HUMAN, int nTribesDwarven = DEFAULT_NUMBER_TRIBES_DWARVEN, int nTribesElven = DEFAULT_NUMBER_TRIBES_ELVEN)
{
	std::cout<<"Generate tribes\n";

	if (nTribesHuman < 0 )
	{ nTribesHuman = 0; }
	if (nTribesDwarven < 0 )
	{ nTribesDwarven = 0; }
	if (nTribesElven < 0 )
	{ nTribesElven = 0; }

	if (nTribesHuman < 0 || nTribesHuman > 999)
	{ std::cout<<"WARNING: Unexpected number of human tribes.\n"; }
	if (nTribesDwarven < 0 || nTribesDwarven > 999)
	{ std::cout<<"WARNING: Unexpected number of human tribes.\n"; }
	if (nTribesElven < 0 || nTribesElven > 999)
	{ std::cout<<"WARNING: Unexpected number of human tribes.\n"; }

	std::cout<<"Generate humans: ";
	for (int i=0;i<nTribesHuman;++i)
	{
		std::cout<<".";
		Tribe_Human * t = new Tribe_Human;
		t->init(this);
		if (t->spawn() == false)
		{
			std::cout<<" FAIL ";
			delete t;
		}
		else
		{
			t->generateCouples(STARTING_TRIBE_SIZE_HUMAN);
		}
	}
	std::cout<<"\n";
	
	generateDwarves(nTribesDwarven);

	std::cout<<"Generate elves: ";
	for (int i=0;i<nTribesElven;++i)
	{
		std::cout<<".";
		Tribe_Elf * t = new Tribe_Elf;
		t->init(this);
		if (t->spawn() == false)
		{
			std::cout<<" FAIL ";
			delete t;
		}
		else
		{
			t->generateCouples(STARTING_TRIBE_SIZE_ELVEN);
		}
	}
	std::cout<<"\n";

	std::cout<<"END Generate tribes\n";
}

// Dwarves start out as Civs for the sake of simplicity, as they all eventually end up dwelling in mountains.
// Later on I might add back the Dwarven tribes.
// Dwarves have only a single mythology, though minor variations may be introduced later.
void World::generateDwarves(int _nCivs )
{
	
	if (_nCivs < 0 )
	{ _nCivs = 0; }

	if (_nCivs > 100)
	{ std::cout<<"WARNING: Excessive number of Dwarven tribes. Execution continuing.\n"; }


	std::cout<<"Generating "<<_nCivs<<" Dwarven civilizations.\n";
	for (int i=0;i<_nCivs;++i)
	{
		std::cout<<".";
		Tribe_Dwarven * t = new Tribe_Dwarven;
		t->init(this);
		if (t->spawn() == false)
		{
			std::cout<<" FAIL ";
			delete t;
		}
		else
		{
			
			// Generate legendary couples.
			t->generateCouples(STARTING_TRIBE_SIZE_DWARVEN/2);
			
			// Pick one couple for each stat to set as legendary.
			
			// Generate mythology. If it's a fresh tribe from worldgen, generate mythology from scratch, otherwise generate it from the parent.
			
			Mythology_Dwarven * myth = new Mythology_Dwarven();
			t->mythology = myth;
			mythologyManager.add(myth);
			evolveToCiv(t);
		}
	}
	std::cout<<"\n";
	



}
void World::generateElves(int _nTribes )
{
}
void World::generateHumans(int _nTribes )
{
}


// Remove tribe object and replace it with a Civ and Settlement (whereever the Tribe is standing.
void World::evolveToCiv( Tribe * _tribe )
{
	if ( _tribe == 0 )
	{ return; }

	Console (Stream() << "Tribe: " << _tribe->name << " became a Civ.");

	if ( _tribe->race == DWARVEN)
	{
		Civ_Dwarven * civ = new Civ_Dwarven;

		civ->name = _tribe->name;
		civ->world = this;
		civ->setColour(_tribe->colourRed,_tribe->colourGreen,_tribe->colourBlue);
		civ->vCharacter.copy(&_tribe->vCharacter);
		civ->mythology = _tribe->mythology;

		Settlement_Dwarven * s = new Settlement_Dwarven;
		s->world = this;
		s->parentCiv = civ;
		s->vCharacter.copy(&_tribe->vCharacter);
		putObject(s, _tribe->worldX, _tribe->worldY);
		s->government=_tribe->government;
		s->government.governedSettlement=s;
		
		
		s->location.biome = getBiome(_tribe->worldX,_tribe->worldY);
		s->buildDwarvenFortress();
		removeObject(_tribe);
		vTribe.remove(_tribe);  

		// Give the Civ a Settlement
		civ->addSettlement(s);
		vCiv.push(civ);

		// Move all characters into the first Settlement
		s->vCharacter.copy(&_tribe->vCharacter);
		
		for (int i=0;i<_tribe->vCharacter.size();++i)
		{
			_tribe->vCharacter(i)->settlement=s;
			s->location.putCharacter(_tribe->vCharacter(i),LOCATION_MAIN_HALL);
		}
		

		
	}
}

void World::incrementTicksBacklog(long long unsigned int nTicks)
{
	ticksBacklog+=nTicks;
}

// Increments the world by nTicks ticks. Higher values may lead to abstraction.
// This seems to be badly designed right now. Each Tribe and Civ should increment by a month then let
// others increment.
void World::incrementTicks(int nTicks)
{
	worldViewer.rainManager.updateRain();

	// std::cout<<"Testing biome vector\n";
	// for (int i=0;i<vBiome.size();++i)
	// {
	// std::cout<<"biome "<<i<<"\n";
	// }

	//ticksBacklog+=nTicks;
	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;

	globalCalendar.advanceSecond(nTicks);

	for ( int i=0;i<vTribe.size();++i)
	{
		if ( vTribe(i)->isAlive == false )
		{
			destroyInfluence(vTribe(i));
			//vTribe(i) = 0;
		}
		else if (vTribe(i) != 0)
		{
			vTribe(i)->incrementTicks(nTicks);
		}
	}

	vTribe.removeNulls();

	// for ( int i=0;i<vTribe.size();++i)
	// {
	// vTribe(i)->incrementTicks(nTicks);
	// }

	for ( int i=0;i<vCiv.size();++i)
	{
		vCiv(i)->incrementTicks(nTicks);
	}


	while (monthlyCounter >= TICKS_PER_MONTH)
	{
		//std::cout<<"Degrade influence.\n";
		degradeInfluence();
		
		incrementDeities(nTicks);
		
		monthlyCounter-=TICKS_PER_MONTH;
		nTicks-=TICKS_PER_MONTH;
	}

	while ( dailyCounter >= TICKS_PER_DAY )
	{
		dailyCounter-=TICKS_PER_DAY;
		nTicks-=TICKS_PER_DAY;
	}

	// FOR NOW INCREMENT INDIVIDUAL TICKS THROW AWAY SOME IF THERE'S TOO MANY
	if (nTicks > 10)
	{ nTicks = 10; }

	while (nTicks-- > 0 )
	{
		// CHECK ALL LOADED MAPS
		for (int i=0;i<vWorldLocal.size();++i)
		{
			vWorldLocal(i)->incrementTicks(1);
		}

		if ( isRaining )
		{
			if (random.oneIn(150))
			{
				isRaining = !isRaining;
			}
		}
		else
		{
			if (random.oneIn(400))
			{
				isRaining = !isRaining;
			}
		}
	}

	// if (calendar.minute%5==0)
	// { isRaining=true; }
	// else
	// { isRaining = false; }


	//for ( int i=0;i<vCiv.size();++i)
	//{
	//vCiv(i)->incrementTicks(nTicks);
	//}
	//updateCivContacts();

	// if (nTicks == 1)
	// {
	// /* MINIMAL ABSTRACTION. */
	// }
	// else if (nTicks == 2)
	// {
	// }
	// else
	// {
	// /* MAXIMUM ABSTRACTION. */
	// }
	RENDER_NEXT_FRAME=true;
}

void World::logicTick()
{
	//std::cout<<"logic tick\n";
	//handleTickBacklog();
}

void World::incrementDeities(int nTicks)
{
	// I think we should let deities act once a month.
	
	deityTicks+=nTicks;
	
	// increment monthly
	while (deityTicks >= TICKS_PER_MONTH)
	{
		//std::cout<<"Deity actions here\n";
		if (random.oneIn(12))
		{
			eventManager.addEvent("DEITY ACT",Event::EVENT_DEITY_ACT);
		}
		deityTicks-=TICKS_PER_MONTH;
	}
	
	
}

void World::updateMaps()
{
	if ( playerCharacter != 0 && playerCharacter->tribe != 0 && playerCharacter->knowledge != 0 )
	{
		// if ( playerCharacter->x < 20 )
		// {
		// generateLocal(playerCharacter->worldX-1,playerCharacter->worldY);
		// }
		// if ( playerCharacter->y < 20 )
		// {
		// generateLocal(playerCharacter->worldX,playerCharacter->worldY-1);
		// }
		// if (playerCharacter->x < 20 && playerCharacter->y < 20)
		// {
		// generateLocal(playerCharacter->worldX-1,playerCharacter->worldY-1);
		// }

		// if ( playerCharacter->x > LOCAL_MAP_SIZE-21 )
		// {
		// generateLocal(playerCharacter->worldX+1,playerCharacter->worldY);
		// }
		// if ( playerCharacter->y > LOCAL_MAP_SIZE-21 )
		// {
		// generateLocal(playerCharacter->worldX,playerCharacter->worldY+1);
		// }
		// if (playerCharacter->x > LOCAL_MAP_SIZE-21 && playerCharacter->y > LOCAL_MAP_SIZE-21)
		// {
		// generateLocal(playerCharacter->worldX+1,playerCharacter->worldY+1);
		// }
	}
}

bool World::handleTickBacklog()
{
	
	// In this mode we just keep ticking in the background in idle time until interrupted by clicking the button again
	// or pressing escape.
	if (incrementContinuous)
	{
		//for (int i=0;i<
		incrementTicks(TICKS_PER_DAY);
		return true;
	}
	
	
	if (ticksBacklog<=0)
	{ return false; }



	unsigned long long int MAXIMUM_TICKS_AT_ONCE = TICKS_PER_MONTH;

	relinquishTimer.start();

	while (ticksBacklog > MAXIMUM_TICKS_AT_ONCE)
	{
		incrementTicks(MAXIMUM_TICKS_AT_ONCE);
		ticksBacklog-=MAXIMUM_TICKS_AT_ONCE;
		relinquishTimer.update();

		if (relinquishTimer.uSeconds > 100 )
		{
			RENDER_NEXT_FRAME=true;
			return true;
		}
	}

	// We have less than maximum ticks remaining, so finish them.
	incrementTicks(ticksBacklog);
	ticksBacklog=0;
	RENDER_NEXT_FRAME=true;
	return true;
}

void World::idleTick()
{
	if ( active == false )
	{ return; }

	if ( playerCharacter != 0 )
	{
		playerCharacter->updateKnowledgeIdle();
	}

	if (handleTickBacklog() == false)
	{
		playerKeypressTimer.update();

		if (playerKeypressTimer.fullSeconds > 0.2 )
		{
			++simX;
			if (simX>=nX)
			{ simX=0; ++simY; }
			if (simY>=nY)
			{ simY=0; }

			// BACKGROUND IDLE UPDATE LOOP
			// Eventually this should be threaded, but it would be a big job to do that.
			// if ( aWorldTile(simX,simY).active==false
			// && aWorldTile(simX,simY).baseBiome != OCEAN
			// && (aWorldTile(simX,simY).initialized==false || aWorldTile(simX,simY).localDate != calendar)
			// )
			// {
			// generateLocal(simX,simY);
			// // Update and sync
			// aWorldTile(simX,simY).localDate.set(&calendar);
			// RENDER_NEXT_FRAME=true;
			// return;
			// }
			if ( ENABLE_BACKGROUND_SIMULATION )
			{
				for (int i=0;i<vAllTiles2.size();++i)
				{
					HasXY * tXY = vAllTiles2(i);
					if ( aWorldTile(tXY).active==false
					&& aWorldTile(tXY).baseBiome != OCEAN
					&& (aWorldTile(tXY).initialized==false || aWorldTile(tXY).localDate != globalCalendar)
					)
					{
						generateLocal(tXY);
						// Update and sync
						aWorldTile(tXY).localDate.set(&globalCalendar);
						RENDER_NEXT_FRAME=true;
						NO_BACKLOG=false;
						return;
					}
				}
			}
			NO_BACKLOG=true;
		}
	}
}

bool World::isLand(int _x, int _y)
{
	if ( aWorldTile.isSafe(_x,_y) )
	{
		return aWorldTile(_x,_y).isLand();
	}
	return false;
}
inline bool World::isLand(HasXY* _xy)
{
	return isLand(_xy->x, _xy->y);
}

int mX;

// This should be done externally.
void World::buildArrays(WorldGenerator2& wg)
{
	// //aTopoMap.initNoFill(nX,nY,3,0);

	// // Create a lightening darkening texture fractal.
	// ArrayS2 <int> aLightModifier2 (nX,nY,0);
	// //aLightModifier2.init(mapSize,mapSize,0);

	// // GENERATE HEIGHTMAP
	// DiamondSquareAlgorithmCustomRange dsa2;
	// dsa2.maxValue=12;
	// //HEIGHTMAP TABLE FREESTEPS SMOOTHING
	// dsa2.generate(&aLightModifier2,0,2,0.75,250);

	// for (int _y=0;_y<nY;++_y)
	// {
	// for (int _x=0;_x<nX;++_x)
	// {
	// const int lightModifier = aLightModifier2(_x,_y);
	// //const int lightModifier = 0;

	// //aSeed(_x,_y) = random.randInt(PORTABLE_INT_MAX-1);

	// //Initialise the WorldTile with biome enum.
	// //aWorldTile(_x,_y).init(_x,_y,aTerrain(_x,_y), aSeed(_x,_y),aRiverID(_x,_y));
	// aWorldTile(_x,_y).init(_x,_y,aTerrain(_x,_y), 0, aRiverID(_x,_y));
	// //Initialise the subterranean tiles

	// int _red;
	// int _green;
	// int _blue;

	// if ( aTerrain(_x,_y) == OCEAN )
	// {
	// _red=50;
	// _green=50;
	// _blue=240;
	// }
	// else if ( aTerrain(_x,_y) == RIVER )
	// {
	// _red=100;
	// _green=100;
	// _blue=240;
	// }
	// else if ( aTerrain(_x,_y) == MOUNTAIN )
	// {
	// _red=80;
	// _green=80;
	// _blue=80;
	// }
	// else if ( aTerrain(_x,_y) == HILLY )
	// {
	// _red=80;
	// _green=140;
	// _blue=80;
	// }
	// else if ( aTerrain(_x,_y) == FOREST )
	// {
	// _red=0;
	// _green=120;
	// _blue=0;
	// }
	// else if ( aTerrain(_x,_y) == DESERT )
	// {
	// _red=200;
	// _green=200;
	// _blue=20;
	// }
	// else if ( aTerrain(_x,_y) == SNOW )
	// {
	// _red=220;
	// _green=220;
	// _blue=240;
	// }
	// else if ( aTerrain(_x,_y) == JUNGLE )
	// {
	// _red=0;
	// _green=70;
	// _blue=0;
	// }
	// else if ( aTerrain(_x,_y) == WETLAND )
	// {
	// _red=20;
	// _green=150;
	// _blue=200;
	// }
	// else if ( aTerrain(_x,_y) == STEPPES )
	// {
	// _red=180;
	// _green=120;
	// _blue=40;
	// }
	// else if ( aTerrain(_x,_y) == ICE )
	// {
	// _red=255;
	// _green=255;
	// _blue=255;
	// }
	// else if ( aTerrain(_x,_y) == CAVE )
	// {
	// _red=0;
	// _green=0;
	// _blue=0;
	// }
	// else if ( aTerrain(_x,_y) == RUIN )
	// {
	// _red=255;
	// _green=0;
	// _blue=255;
	// }
	// else
	// {
	// _red=66;
	// _green=180;
	// _blue=66;
	// }

	// //mof light values.
	// // _red += lightModifier;
	// // if ( _red > 255 ) { _red = 255; }

	// // _green += lightModifier;
	// // if ( _green > 255 ) { _green = 255; }

	// // _blue += lightModifier;
	// //if ( _blue > 255 ) { _blue = 255; }

	// _red -= lightModifier;
	// if ( _red < 0 ) { _red = 0; }

	// _green -= lightModifier;
	// if ( _green < 0 ) { _green = 0; }

	// _blue -= lightModifier;
	// if ( _blue < 0 ) { _blue = 0; }


	// aTopoMap(_x,_y,0) = _red;
	// aTopoMap(_x,_y,1) = _green;
	// aTopoMap(_x,_y,2) = _blue;



	// }
	// }

	// #ifdef SAVE_DATA
	// Png png;
	// png.encodeS3(strSavePath+"/worldmap.png",&aTopoMap);
	// #endif
}

// Generate world and world data.
// A lot of CPU could be saved by only generating the basic data in worldgen mode and then
// letting the other data be generated in the background.
// However worldgen is already fast enough so it's not a high priority

void World::generateWorld(const std::string _worldName, const int x=127, const int y=127, const int seed=0, const int fragmentation=2, const bool islandMode = true, const bool wrapX=true, const bool wrapY=false, const double landPercent = 0.66)
{
	if ( x <= 0 || y <= 0 )
	{
		std::cout<<"Error: Invalid world size.\n";
		return;
	}
	if (_worldName == "")
	{
		std::cout<<"Error: World must have a name.\n";
		return;
	}
	if (DataTools::isAlphaNumeric(_worldName,true)==false)
	{
		std::cout<<"Error: World name must be alphanumeric.\n";
		return;
	}
	name=_worldName;

#ifdef SAVE_DATA
	FileManager::createDirectory(globalSettings.SAVE_FOLDER_PATH);
	if ( FileManager::directoryExists(globalSettings.SAVE_FOLDER_PATH) == false )
	{
		std::cout<<"Error: Unable to create save directory.\n";
		return;
	}
#endif

	nX = x;
	nY = y;

	//maximum possible tile X/Y coordinate.
	maximumX = LOCAL_MAP_SIZE*nX-1;
	maximumY = LOCAL_MAP_SIZE*nY-1;

	mX=x;
	landmassSeed = seed;

	strSavePath = globalSettings.SAVE_FOLDER_PATH+"/"+name;
	currentSavePath = globalSettings.SAVE_FOLDER_PATH+"/"+name;

#ifdef SAVE_DATA

	// For now, we will just delete any worlds with the same name.
	FileManager::deleteDirectory(strSavePath,true);
	FileManager::createDirectory(strSavePath);

	if ( FileManager::directoryExists(strSavePath) == false )
	{
		std::cout<<"Error: Unable to create save directory.\n";
		return;
	}

#endif

	Timer worldGenTimer;
	worldGenTimer.init();
	worldGenTimer.start();

	// Multithreading this segment runs 23% faster. Saves about 0.1 seconds or something.
#define THREAD_WORLD_INIT
#if defined WILDCAT_THREADING || defined THREAD_WORLD_INIT 

	std::thread t1( [this,x,y] { aWorldObject.init(x,y,0); aTopoMap.init(x,y,3,0); });
	std::thread t4( [this,x,y] { aBiomeID.init(x,y,-1); aWorldTile.initClass(x,y); /* mapManager.init(x,y); */ });
	std::thread t5( [this] { vWorldObjectGlobal.deleteAll(); vLandmass.deleteAll(); vBiome.deleteAll(); vTribe.clear(); });

#else

	aWorldObject.init(x,y,0);
	aTopoMap.init(x,y,3,0);
	aBiomeID.init(x,y,-1);
	aIsLand.init(x,y,true);
	aWorldTile.initClass(x,y);

	// vTribe does not need to be deleted, because all Tribe objects are in vWorldObjectGlobal
	// However, the vector must be cleared.
	vWorldObjectGlobal.deleteAll();
	vLandmass.deleteAll();
	vBiome.deleteAll();
	vTribe.clear();

#endif

	dailyCounter=0;
	monthlyCounter=0;

	WorldGenerator2 wg;
	wg.wrapX=wrapX;
	wg.wrapY=wrapY;
	wg.seed = seed;
	wg.mapSize=x;

	wg.oceanPercent=landPercent;

	wg.mountainPercent=0.025;
	wg.forestPercent=0.25;
	wg.desertPercent=0.33;
	wg.snowPercent=0.04;

	wg.goodPercent=0.05;
	wg.evilPercent=0.05;

	wg.freeSteps=fragmentation;
	wg.islandMode=islandMode;
	wg.variance=400;
	wg.landSmoothing=0.88;

	// Create master file.
	worldFilePath = strSavePath+"/main.dat";

#ifdef SAVE_DATA

	FileManager::createFile(worldFilePath);
	FileManager::writeTag("LANDSEED",DataTools::toString(landmassSeed),worldFilePath);
	FileManager::writeTag("WORLDNAME",name,worldFilePath);
	FileManager::writeTag("SIZEX",DataTools::toString(nX),worldFilePath);
	FileManager::writeTag("SIZEY",DataTools::toString(nY),worldFilePath);
	//FileManager::writeString(DataTools::toString(landmassSeed)+"\n"+name+"\n"+DataTools::toString(nX)+"\n"+DataTools::toString(nY)+"\n",worldFilePath);


	//Add the world to a master list. This is used for later loading the world from a menu.
	// The path is where you can go to load it.
	// The version ID allows us to see if the save will be compatible with this version (spoilers: it won't)

	std::string masterData = "";
	masterData+="[SAVEPATH:"+strSavePath+"]";
	masterData+="[VERSION ID:"+COMPILE_COUNT+"]";

	FileManager::writeString(masterData,globalSettings.SAVE_FOLDER_PATH+"/master.dat");

#endif

#if defined WILDCAT_THREADING || defined THREAD_WORLD_INIT 
	t1.join();
	t4.join();
	t5.join();
#endif

	std::cout<<"Generating astronomy\n";

	int nMoons = 1;
	if ( globalRandom.oneIn(100) )
	{
		++nMoons;
	}

	astronomy.generateSystem(nMoons,globalRandom.multiRoll8(5,3,true));

	wg.generate();
	aTerrain.init(nX,nY,OCEAN);
	aRiverID = wg.aRiverMap; // this needs to be moved to aTerrain.

	//buildArrays(wg);  // This seems to mostly just build the aTopoMap and PNG export.

	for (int _y=0;_y<nY;++_y)
	{
		for (int _x=0;_x<nX;++_x)
		{
			aWorldTile(_x,_y).init(_x,_y,wg.aTerrainType(_x,_y), globalRandom.rand32(), aRiverID(_x,_y));
			aTerrain(_x,_y) = wg.aTerrainType(_x,_y);
		}
	}

	// BUILD RIVER CONNECTIVITY
	// This could probably be done in world generator
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			if (aRiverID(_x,_y)!=-1)
			{
				char riverConnectivity = 0;
				// Connect east
				if ( aRiverID.isSafe(_x+1,_y) && aRiverID(_x+1,_y) != -1 )
				{
					riverConnectivity=riverConnectivity|0b00001000;
				}
				// Connect west
				if ( aRiverID.isSafe(_x-1,_y) && aRiverID(_x-1,_y) != -1 )
				{
					riverConnectivity=riverConnectivity|0b00010000;
				}
				// Connect north
				if ( aRiverID.isSafe(_x,_y+1) && aRiverID(_x,_y+1) != -1 )
				{
					riverConnectivity=riverConnectivity|0b01000000;
				}
				// Connect south
				if ( aRiverID.isSafe(_x,_y-1) && aRiverID(_x,_y-1) != -1 )
				{
					riverConnectivity=riverConnectivity|0b00000010;
				}
				aWorldTile(_x,_y).riverConnections = riverConnectivity;

				// If tile is surrounded by rivers, it becomes a lake tile.
				if ( riverConnectivity == 0b01011010 )
				{
					aWorldTile(_x,_y).baseBiome=OCEAN;
				}
				// All tiles surrounded on at least 3 sides by rivers become lakes.
				if ( riverConnectivity == 0b01010010 )
				{
					aWorldTile(_x,_y).baseBiome=OCEAN;
				}
				else if ( riverConnectivity == 0b01001010 )
				{
					aWorldTile(_x,_y).baseBiome=OCEAN;
				}
				else if ( riverConnectivity == 0b01010010 )
				{
					aWorldTile(_x,_y).baseBiome=OCEAN;
				}
				else if ( riverConnectivity == 0b01011000 )
				{
					aWorldTile(_x,_y).baseBiome=OCEAN;
				}
				else if ( riverConnectivity == 0b00011010 )
				{
					aWorldTile(_x,_y).baseBiome=OCEAN;
				}
			}
		}
	}
	
	// Build and shuffle the tile vector
	// We need this in generation because tribe spawning uses it.
	//Vector < Vector <HasXY> > vAllTiles;

	std::thread tvAllTiles( [this]
	{
		for (int i=0;i<vAllTiles.size();++i)
		{
			delete vAllTiles(i);
		}
		vAllTiles.clear();
		for (int i=0;i<nY;++i)
		{
			Vector <int>* vCoord = new Vector <int>;
			for (int _x=0;_x<nX;++_x)
			{
				vCoord->push(_x);
			}
			vCoord->shuffle();
			vAllTiles.push(vCoord);
		}
	});

	std::thread tvAllTiles2( [this]
	{
		for (int i=0;i<vAllTiles2.size();++i)
		{
			delete vAllTiles2(i);
		}
		vAllTiles2.clear();
		for (int _y=0;_y<nY;++_y)
		{
			for (int _x=0;_x<nX;++_x)
			{
				vAllTiles2.push( new HasXY (_x,_y) );
			}
		}
		vAllTiles2.shuffle();
	});

	tvAllTiles.join();
	tvAllTiles2.join();

	//timerBiomeFill.update();
	//std::cout<<"Filled "<<currentID<<" biomes in "<<timerBiomeFill.fullSeconds<<" seconds.\n";

	buildMinimap();
	
	eventManager.clear();
	eventManager.addEvent("WORLD CREATED",Event::EVENT_WORLD_CREATED);
	eventManager.addEvent("EVENT1",Event::EVENT_NONE);
	eventManager.addEvent("EVENT2",Event::EVENT_NONE);
	eventManager.addEvent("EVENT3",Event::EVENT_NONE);

	generated = true;
	worldGenTimer.update();
	std::cout<<"world generated in: "<<worldGenTimer.fullSeconds<<" seconds.\n";
	std::cout<<"The world's uid is: "<<getUID()<<".\n";
}


//This makes the local map visible.
void World::generateLocal(const int _localX, const int _localY)
{
	std::cout<<"Generate local: "<<_localX<<", "<<_localY<<"\n";
	//if ( isSafe(_localX,_localY) == false || active == false )
	if ( isSafe(_localX,_localY) == false )
	{
		std::cout<<"notsafe or inactive\n";
		return;
	}

	// Only generate this local map if it isn't already in memory.
	for ( int i=0;i<vWorldLocal.size();++i)
	{
		if ( vWorldLocal(i)->globalX == _localX && vWorldLocal(i)->globalY == _localY )
		{
			std::cout<<"Already in vector\n";
			return;
		}
	}

	//mapManager.generateNow(_localX,_localY);
	
	short int gX = _localX;
	short int gY = _localY;

	std::cout<<"global coords: "<<gX<<", "<<gY<<"\n";
	World_Local* wl0 = 0;
	if (aWorldTile.isSafe(gX-1,gY+1))
	{
		wl0 = &aWorldTile(gX-1,gY+1);
	}
	World_Local* wl1 = 0;
	if (aWorldTile.isSafe(gX,gY+1))
	{
		wl1 = &aWorldTile(gX,gY+1);
	}
	World_Local* wl2 = 0;
	if (aWorldTile.isSafe(gX+1,gY+1))
	{
		wl2 = &aWorldTile(gX+1,gY+1);
	}
	
	World_Local* wl3 = 0;
	if (aWorldTile.isSafe(gX-1,gY))
	{
		wl3 = &aWorldTile(gX-1,gY);
	}
	World_Local* wl4 = 0;
	if (aWorldTile.isSafe(gX+1,gY))
	{
		wl4 = &aWorldTile(gX+1,gY);
	}

	World_Local* wl5 = 0;
	if (aWorldTile.isSafe(gX-1,gY-1))
	{
		wl5 = &aWorldTile(gX-1,gY-1);
	}
	World_Local* wl6 = 0;
	if (aWorldTile.isSafe(gX,gY-1))
	{
		wl6 = &aWorldTile(gX,gY-1);
	}
	World_Local* wl7 = 0;
	if (aWorldTile.isSafe(gX+1,gY-1))
	{
		wl7 = &aWorldTile(gX+1,gY-1);
	}

	// vMap(i)->generate(false,wl0,wl1,wl2,wl3,wl4,wl5,wl6,wl7);
	// vMap(i)->active=true;
	// vMap(i)->initialized=true;
	
	//std::cout<<"tile: "<<&aWorldTile(_localX,_localY)<<"\n";
	//return;

	// don't cache the generated map because the map manager does it
	// Generate a map which should stitch the heightmaps with any generated neighbors.
	// Note that the stitching is dependent on the order that maps have been generated.
	aWorldTile(_localX,_localY).generate(false,wl0,wl1,wl2,wl3,wl4,wl5,wl6,wl7);
	//aWorldTile(_localX,_localY).generate(false,0,0,0,0,0,0,0,0);
	
	//return;

	
	//aWorldTile(_localX,_localY).generateSubterranean();
	aWorldTile(_localX,_localY).active=true;
	aWorldTile(_localX,_localY).initialized=true;
	
	// Local Map management.
	//There needs to be a minimum of 4 maps active at any time. (1 map the player is currently in,
	// and potentially three neighboring maps. Additional maps will likely need to be loaded in
	// the background in the same way, therefore I'll set it to 5 for now.
	// I'll need to make an algorithm to decide which maps to purge.
	// For now we can disable it if we need to for debugging.
	// I am not sure how well this works currently. may need a lot of testing.
	//if ( vWorldLocal.size() > MAX_LOCAL_MAPS_IN_MEMORY )
	if ( false )
	{
		std::cout<<"unloading old map\n";
		// Pick a map to unload from memory.
		// Note: Don't delete World_Local. It must always be loaded. However some internal data must be cleaned up.
		// Don't unload tiles near player.

		// build vector of maps we shouldn't unload (maps near player)
		Vector <World_Local*> vImportantMaps;
		if ( playerCharacter != 0)
		{
			if ( aWorldTile.isSafe(playerCharacter->worldX,playerCharacter->worldY) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX,playerCharacter->worldY) ); }

			if ( aWorldTile.isSafe(playerCharacter->worldX-1,playerCharacter->worldY) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX-1,playerCharacter->worldY) ); }
			if ( aWorldTile.isSafe(playerCharacter->worldX+1,playerCharacter->worldY) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX+1,playerCharacter->worldY) ); }

			if ( aWorldTile.isSafe(playerCharacter->worldX,playerCharacter->worldY+1) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX,playerCharacter->worldY+1) ); }
			if ( aWorldTile.isSafe(playerCharacter->worldX-1,playerCharacter->worldY+1) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX-1,playerCharacter->worldY+1) ); }
			if ( aWorldTile.isSafe(playerCharacter->worldX+1,playerCharacter->worldY+1) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX+1,playerCharacter->worldY+1) ); }

			if ( aWorldTile.isSafe(playerCharacter->worldX,playerCharacter->worldY-1) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX,playerCharacter->worldY-1) ); }
			if ( aWorldTile.isSafe(playerCharacter->worldX-1,playerCharacter->worldY-1) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX-1,playerCharacter->worldY-1) ); }
			if ( aWorldTile.isSafe(playerCharacter->worldX+1,playerCharacter->worldY-1) )
			{ vImportantMaps.push( &aWorldTile(playerCharacter->worldX+1,playerCharacter->worldY-1) ); }
		}
		if (DEBUG_X != -1 && DEBUG_Y != -1 )
		{
			vImportantMaps.push( &aWorldTile(DEBUG_X,DEBUG_Y) );
		}


		// find a map we can unload and unload it.
		// This code section is unsurpisingly not playing nice with the map manager.
		// I disable it for now.
		for ( int i=0;i<vWorldLocal.size();++i)
		{
			if (vImportantMaps.contains(vWorldLocal(i)) == false )
			{
				//vWorldLocal(i)->active=false;
				// vWorldLocal(i)->unload(); // let mapmanager handle this
				//vWorldLocal.eraseSlot(i);
				break;
			}
		}
	}
	std::cout<<"Pushing local map: "<<&aWorldTile(_localX,_localY)<<"\n";
	vWorldLocal.push(&aWorldTile(_localX,_localY));
	std::cout<<"finished generate local\n";
}
void World::generateLocal(HasXY* _xy) { generateLocal(_xy->x,_xy->y); }

void World::unloadLocal(const int _localX, const int _localY)
{
	if ( isSafe(_localX,_localY) == false )
	{ return; }

	FileManager::createDirectory(globalSettings.SAVE_FOLDER_PATH);
	if ( FileManager::directoryExists(globalSettings.SAVE_FOLDER_PATH) == false )
	{
		std::cout<<"Error: Unable to create save directory.\n";
		return;
	}

	// Only unload the local map if it is loaded.
	for ( int i=0;i<vWorldLocal.size();++i)
	{
		if ( vWorldLocal(i)->globalX == _localX && vWorldLocal(i)->globalY == _localY )
		{
			//save to file
			SaveFileManager sfm;

			std::string saveData="";

			for (int _y=0;_y<LOCAL_MAP_SIZE;++_y)
			{
				for (int _x=0;_x<LOCAL_MAP_SIZE;++_x)
				{
					saveData+=".";
				} saveData+="\n";
			}
			return;
		}
	}
	return;
}


Vector <Tribe*>* World::getTribesOn(const int _x, const int _y)
{
	Vector <Tribe*>* vTribeOn = new Vector <Tribe*>;
	if ( isSafe(_x,_y) )
	{
		for ( int i=0;i<vTribe.size();++i)
		{
			if ( vTribe(i)->worldX == _x && vTribe(i)->worldY == _y )
			{
				vTribeOn->push(vTribe(i));
			}
		}
	}
	return vTribeOn;
}

int World::getTileFertility(const int _x, const int _y)
{
	if (aWorldTile.isSafe(_x,_y)==false)
	{
		return 0;
	}
	return aWorldTile(_x,_y).baseFertility;
}
inline int World::getTileFertility(const HasXY* xy)
{ return getTileFertility(xy->x, xy->y); }

int World::getSurroundingFertility(const int _x, const int _y)
{
	if (aWorldTile.isSafe(_x,_y)==false)
	{
		return 0;
	}

	int totalFertility = 0;

	Vector < HasXY* >* vTerrain = aWorldTile.getNeighbors(_x,_y,true);
	if ( vTerrain==0 )
	{
		return 0;
	}

	for ( int i=0;i<vTerrain->size();++i)
	{
		totalFertility+= getTileFertility((*vTerrain)(i));
	}
	vTerrain->deleteAll();
	delete vTerrain;
	return totalFertility;
}

int World::getHuntingYield(const int _x, const int _y)
{
	// This is outdated and shoult be replaced anyway

	//int totalYield = 0;

	// aWorldTile.getNeighborVector(_x,_y,&vTerrain,true /* INCLUDE SELF */);
	// Vector < HasXY* >* vTerrain = aWorldTile.getNeighbors(_x,_y,true);

	// for ( int i=0;i<vTerrain->size();++i)
	// {
	// int terrainType = (*vTerrain)(i)

	// if ( terrainType == OCEAN )
	// {
	// totalYield += 2;
	// }
	// else if (terrainType == GRASSLAND )
	// {
	// totalYield += 1;
	// }
	// else if (terrainType == FOREST )
	// {
	// totalYield += 2;
	// }
	// else if (terrainType == JUNGLE)
	// {
	// totalYield += 2;
	// }
	// }

	//return totalYield;

	return 5;
}

std::string World::getTileType(const int _x, const int _y)
{
	if (aWorldTile.isSafe(_x,_y))
	{
		return biomeName[aWorldTile(_x,_y).baseBiome];
	}
	return "?";
}

void World::getRandomTile (int* x, int* y)
{
	*x = Random::randomInt(nX);
	*y = Random::randomInt(nY);
}

int World::getPopulation()
{
	int population = 0;
	for (int i=0;i<vTribe.size();++i)
	{
		population+=vTribe(i)->getPopulation();
	}
	return population;
}

bool World::getRandomLandTile(int* x, int* y)
{
	int randomRow = random.randomInt(nY-1);
	int nRowsSearched = 0;

	while ( nRowsSearched < nY )
	{
		for (int _x=0;_x<nX;++_x)
		{
			int randomX = (*vAllTiles(randomRow))(_x);
			if ( aWorldTile(randomX,randomRow).baseBiome != OCEAN )
			{
				*x=randomX;
				*y=randomRow;
				return true;
			}
		}
		vAllTiles(randomRow)->shuffle();

		++randomRow;
		if (randomRow >= nY)
		{ randomRow=0; }
		++nRowsSearched;
	}

	*x=0;
	*y=0;
	return false;
}

HasXY* World::getRandomLandTile()
{
	vAllTiles2.shuffle();
	for (auto xy : vAllTiles2)
	{
		if ( aWorldTile(xy->x,xy->y).baseBiome != OCEAN )
		{
			return xy;
		}
	}
	return 0;
}

// Return a random tile of given type
// Note: In some cases it would be more ideal to get a list of all such tiles to prevent excessive re-rolling.
HasXY* World::getRandomTileOfType(enumBiome _type)
{
	vAllTiles2.shuffle();
	for (auto xy : vAllTiles2)
	{
		if ( aWorldTile(xy->x,xy->y).baseBiome == _type )
		{
			return xy;
		}
	}
	return 0;
}

void World::queryTile( const int hoveredXTile, const int hoveredYTile)
{
	if ( isSafe (hoveredXTile,hoveredXTile) == false )
	{
		return;
	}
	queryWorldX = hoveredXTile;
	queryWorldY = hoveredYTile;
}
void World::queryTileLocal( const int hoveredXTile, const int hoveredYTile)
{
	if (hoveredXTile >= LOCAL_MAP_SIZE && hoveredYTile >= LOCAL_MAP_SIZE )
	{
		return;
	}
	queryWorldXLocal = hoveredXTile;
	queryWorldYLocal = hoveredYTile;
}

void World::addInfluence(Tribe* tribe, int amount)
{
	if ( tribe==0 || isSafe (tribe->worldX,tribe->worldY) == false )
	{ return; }

	aWorldTile(tribe->worldX,tribe->worldY).addInfluence(tribe,amount);
}

// THIS IS A BIT DODGY. THERE SHOULD BE A SINGLE PASS FOR EVERYONE AT ONCE.
void World::degradeInfluence(int value /* =1 */)
{
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			aWorldTile(_x,_y).degradeInfluence(1);
		}
	}
}

void World::destroyInfluence (Tribe* _tribe)
{
	if ( _tribe==0 )
	{ return; }


	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			aWorldTile(_x,_y).destroyInfluence(_tribe);
		}
	}
}

// Note that 0 influence doesn't count as influence. In this case it will return 0.
Tribe* World::getDominantInfluence (const int _x, const int _y)
{
	if ( isSafe(_x,_y) == false )
	{ return 0; }
	return aWorldTile(_x,_y).getDominantInfluence();
}

Tribe* World::getDominantInfluence (HasXY* _xy)
{
	return getDominantInfluence(_xy->x,_xy->y);
}


int World::getHighestInfluence(const int _x, const int _y)
{
	if ( isSafe(_x,_y) == false )
	{ return 0; }
	return aWorldTile(_x,_y).getDominantInfluenceValue();
}

int World::getHighestInfluence(HasXY* _xy)
{ return getHighestInfluence(_xy->x,_xy->y); }


std::string World::getLandmassName(const int _x, const int _y)
{
	if ( aWorldTile.isSafe(_x,_y) )
	{
		const int id = aWorldTile(_x,_y).landID;
		if (id != -1)
		{
			return vLandmass(id)->name;
		}
	}
	return "";
}
std::string World::getLandmassName (HasXY* _xy)
{ return getLandmassName(_xy->x,_xy->y); }

World_Biome * World::getBiome(const int id)
{
	for (int i=0;i<vBiome.size();++i)
	{
		if ( vBiome(i)->id == id)
		{
			return vBiome(i);
		}
	}
	
	return 0;
}

World_Biome* World::getBiome(const int _x, const int _y)
{
	if ( isSafe(_x,_y) )
	{
		if (vBiome.isSafe(aBiomeID(_x,_y)))
		{
			return vBiome(aBiomeID(_x,_y));
		}
	}
	return nullptr;
}

std::string World::getBiomeName(const int _x, const int _y)
{
	if ( isSafe(_x,_y) )
	{
		if (vBiome.isSafe(aBiomeID(_x,_y)))
		{
			return vBiome(aBiomeID(_x,_y))->name;
		}
	}
	return "";
}

bool World::hasFreeTerritory(int landmassID)
{
	Vector <HasXY*>* vLandmassTiles = 0;

	// Todo: Optimize. We don't need to use aIsLand if we already have landmass IDs
	// first build an array of isLand() to do flood fill easier
	ArrayS2 <bool> aIsLand(nX,nY,false);
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			aIsLand(_x,_y)=aWorldTile(_x,_y).isLand();
		}
	}

	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			if (aWorldTile(_x,_y).landID == landmassID && aWorldTile(_x,_y).isLand() == true )
			{
				// GET ALL COORDINATES OF THIS 
				vLandmassTiles = aIsLand.floodFillVector(_x,_y,false);

				for (int i=0;i<vLandmassTiles->size();++i)
				{
					if ( getHighestInfluence( (*vLandmassTiles)(i) ) == 0)
					{
						delete vLandmassTiles;
						return true;
					}
				}
				delete vLandmassTiles;
				return false;
			}
		}
	}
	delete vLandmassTiles;
	return false;
}

int World::nFreeTerritory(int landmassID)
{
	Vector <HasXY*>* vLandmassTiles = 0;

	// Todo: Optimize. We don't need to use aIsLand if we already have landmass IDs
	// first build an array of isLand() to do flood fill easier
	ArrayS2 <bool> aIsLand(nX,nY,false);
	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			aIsLand(_x,_y)=aWorldTile(_x,_y).isLand();
		}
	}

	int nUnclaimed = 0;

	for ( int _y=0;_y<nY;++_y)
	{
		for ( int _x=0;_x<nX;++_x)
		{
			if (aWorldTile(_x,_y).landID == landmassID && aWorldTile(_x,_y).isLand() == true )
			{
				// GET ALL COORDINATES OF THIS LANDMASS
				vLandmassTiles = aIsLand.floodFillVector(_x,_y,false);

				for (int i=0;i<vLandmassTiles->size();++i)
				{
					if ( getHighestInfluence( (*vLandmassTiles)(i) ) == 0)
					{
						++nUnclaimed;
					}
				}
				delete vLandmassTiles;
				return nUnclaimed;
			}
		}
	}
	delete vLandmassTiles;
	return 0;
}

// Generate local map if necessary.
// Update worldviewer.
void World::controlCharacter(Character* _character)
{
	if (_character==0) { return; }

	playerCharacter = _character;

	/* Get character coordinates */
	int genX = playerCharacter->worldX;
	int genY = playerCharacter->worldY;

	/* Generate local map */
	generateLocal(genX,genY);

	worldViewer.tileSize = 2000;
	worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);

	playerCharacter->initialiseKnowledge();
	//playerCharacter->updateKnowledge();

	// Jiggle character around to make sure they're not inside an object.

	World_Local* wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
	if ( wl != 0 )
	{
		wl->moveObject(playerCharacter,playerCharacter->x+1,playerCharacter->y);
		playerCharacter->updateKnowledge();
	}
	wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
	if ( wl != 0 )
	{
		wl->moveObject(playerCharacter,playerCharacter->x-1,playerCharacter->y);
		playerCharacter->updateKnowledge();
	}
	wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
	if ( wl != 0 )
	{
		wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y+1);
		playerCharacter->updateKnowledge();
	}
	wl = (*this)(playerCharacter->worldX,playerCharacter->worldY);
	if ( wl != 0 )
	{
		wl->moveObject(playerCharacter,playerCharacter->x,playerCharacter->y-1);
		playerCharacter->updateKnowledge();
	}

	worldViewer.setCenterTile(playerCharacter->worldX, playerCharacter->worldY, playerCharacter->x, playerCharacter->y);
	updateMaps();
	playerCharacter->updateKnowledge();
}


Tribe * World::getNearestConnectedTribe (Tribe * _tribe, bool sameRace /* =true */ )
{
	// STEP 1: FIND TRIBE ON SAME LANDMASS
	int distance = -1;
	Tribe* closestTribe = 0;
	int landmass = aWorldTile(_tribe->worldX, _tribe->worldY).landID;

	for (int i=0;i<vTribe.size();++i)
	{
		if ( vTribe(i) != _tribe && vTribe(i)->isAlive && (sameRace==false || _tribe->race == vTribe(i)->race ) )
		{
			if ( aWorldTile(vTribe(i)->worldX,vTribe(i)->worldY).landID == landmass )
			{
				int distance2 = _tribe->distanceTo(vTribe(i));
				if ( closestTribe == 0 || distance2 < distance )
				{
					closestTribe = vTribe(i);
					distance = distance2;
				}
			}
		}
	}
	return closestTribe;
}

Tribe* World::combatCheck (Tribe* _tribe)
{
	for ( int i=0;i<vTribe.size();++i)
	{
		if ( vTribe(i) != _tribe && vTribe(i)->race == _tribe->race && vTribe(i)->isAlive && _tribe->isAlive) /* Temp limit combat between same race */
		{
			const int distX = abs(_tribe->worldX - vTribe(i)->worldX);
			const int distY = abs(_tribe->worldY - vTribe(i)->worldY);

			if ( distX < 2 && distY < 2 )
			{
				return vTribe(i);
			}
		}
	}
	return 0;
}

World_Local * World::getTile (const int x, const int y )
{
	if (isSafe(x,y) )
	{ return &aWorldTile(x,y); }
	return 0;
}

bool World::hasSettlement(const int _x, const int _y)
{
	for (int i=0;i<vCiv.size();++i)
	{
		for (int j=0;j<vCiv(i)->vSettlement.size();++j)
		{
			if (vCiv(i)->vSettlement(j)->worldX == _x && vCiv(i)->vSettlement(j)->worldY == _y)
			{
				return true;
			}
		}
	}
	
	return false;
}

// This should be merged with control character.
bool World::prepareAdventureMode( Character * _character )
{
	//std::cout<<"World is preparing for Adventure Mode.\n";

	if ( _character == 0 )
	{
		std::cout<<"Error: Bad pointer.\n";
		return false;
	}

	playerCharacter = _character;

	if ( playerCharacter->tribe == 0 )
	{
		std::cout<<"Error: Character doesn't have Tribe.\n";
		return false;
	}
	controlCharacter(_character);
	return true;
}

bool World::loadWorld(std::string _name)
{
#ifdef SAVE_DATA
	SaveFileManager sfm;

	strSavePath = globalSettings.SAVE_FOLDER_PATH+"/"+_name;

	std::cout<<"Attempting to load data from: "<<strSavePath<<".\n";

	if ( FileManager::directoryExists(strSavePath) == false )
	{
		std::cout<<"Error: This world doesn't appear to exist.\n";
		return false;
	}

	std::cout<<"Loading data from: "<<strSavePath<<".\n";

	// Load master file.
	worldFilePath = strSavePath+"/main.dat";

	if ( FileManager::fileExists(worldFilePath) )
	{
		std::cout<<"Loading master.dat\n";
		std::cout<<FileManager::getFileAsString(worldFilePath)<<".\n";
		sfm.loadFile(worldFilePath);
	}
	else
	{
		std::cout<<"Master file doesn't appear to exist.\n";
		return false;
	}

	std::string wname = sfm.loadVariableString("WORLDNAME");
	std::string wseed = sfm.loadVariableString("LANDSEED");
	std::string wX = sfm.loadVariableString("SIZEX");
	std::string wY = sfm.loadVariableString("SIZEY");

	std::cout<<"Loaded worldname is: "<<wname<<".\n";
	std::cout<<"Loaded seed is: "<<wseed<<".\n";
	std::cout<<"Loaded x is: "<<wX<<".\n";
	std::cout<<"Loaded y is: "<<wY<<".\n";

	int fileSize;
	unsigned char* data = FileManager::getFile(strSavePath+"/biome.png" ,&fileSize);

	if (data == 0)
	{
		std::cout<<"ERROR: biome.png couldn't be loaded.\n";
		return false;
	}

	Png png;

	if (png.load(data,fileSize) == false )
	{
		std::cout<<"ERROR: PNG didn't load successfully.\n";
		return false;
	}

	if ( png.nX < 1 || png.nY < 1 )
	{
		std::cout<<"ERROR: PNG has bad dimensions.\n";
		return false;
	}

	nX = png.nX;
	nY = png.nY;

	aWorldObject.init(png.nX,png.nY,0);
	aBiomeID.init(png.nX,png.nY,-1);

	for(int x=0;x<png.nX;++x)
	{
		for(int y=0;y<png.nY;++y)
		{
			// Note that PNGs use a different coordinate system, therefore must be loaded upside-down.
			// Maybe change this in future.

			aTopoMap(x,png.nY-y-1,0)=png.getPixel3D(x,y,0);
			aTopoMap(x,png.nY-y-1,1)=png.getPixel3D(x,y,1);
			aTopoMap(x,png.nY-y-1,2)=png.getPixel3D(x,y,2);

			// aTopoMap(x,y,0)=png.getPixel3D(x,y,0);
			// aTopoMap(x,y,1)=png.getPixel3D(x,y,1);
			// aTopoMap(x,y,2)=png.getPixel3D(x,y,2);

			if (aTopoMap(x,png.nY-y-1,0) == biomeRed[OCEAN] && aTopoMap(x,png.nY-y-1,1) == biomeGreen[OCEAN] && aTopoMap(x,png.nY-y-1,2) == biomeBlue[OCEAN] )
			//if (aTopoMap(x,y,0) == biomeRed[OCEAN] && aTopoMap(x,y,1) == biomeGreen[OCEAN] && aTopoMap(x,y,2) == biomeBlue[OCEAN] )
			{
				aWorldTile(x,png.nY-y-1).baseBiome=OCEAN;
			}
			else
			{
				aWorldTile(x,png.nY-y-1).baseBiome=GRASSLAND;
			}
		}
	}

	generated = true;
	return true;
#endif
	return false;
}


/** This function builds a minimap texture which is later rendered in the world simulator. */
void World::buildMinimap()
{
	minimap.init();

	std::cout<<"Building minimap\n";
	ArrayS3 <unsigned char> aMinimap (nX,nY,3,0);

	for (int _y=0;_y<nY;++_y)
	{
		for (int _x=0;_x<nX;++_x)
		{
			enumBiome _biome = aWorldTile(_x,_y).baseBiome;
			aMinimap(_x,_y,0) = biomeRed[_biome];
			aMinimap(_x,_y,1) = biomeGreen[_biome];
			aMinimap(_x,_y,2) = biomeBlue[_biome];
		}
	}
	//Png png;
	//png.encodeS3(strSavePath+"/biome.png",&aBiomeData);
	// SAVE RESOURCE INFO AS PNG.
	std::cout<<"END building minimap\n";
}

// SaveFileInterface
void World::save()
{
	SaveFileManager sfm;

	// WORLD INFO
	sfm.addVariable("WORLDNAME",name);
	sfm.addVariable("LANDSEED",landmassSeed);
	sfm.addVariable("SIZEX",nX);
	sfm.addVariable("SIZEY",nY);

	sfm.saveToFile(worldFilePath);

	// SAVE BIOME INFO AS PNG.
	// BIOME INFO ALSO FUNCTIONS AS LANDMASS INFO.
	ArrayS3 <unsigned char> aBiomeData (nX,nY,3,0);

	for (int _y=0;_y<nY;++_y)
	{
		for (int _x=0;_x<nX;++_x)
		{
			enumBiome _biome = aWorldTile(_x,_y).baseBiome;
			aBiomeData(_x,_y,0) = biomeRed[_biome];
			aBiomeData(_x,_y,1) = biomeGreen[_biome];
			aBiomeData(_x,_y,2) = biomeBlue[_biome];
		}
	}
	Png png;
	png.encodeS3(strSavePath+"/biome.png",&aBiomeData);
	// SAVE RESOURCE INFO AS PNG.
}

#endif
