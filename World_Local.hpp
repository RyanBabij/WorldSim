#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_HPP
#define WORLDSIM_WORLD_LOCAL_HPP

#include <IdleTick/IdleTickInterface.hpp>
#include <LogicTick/LogicTickInterface.hpp>

/* World_Local.hpp
	#include "World_Local.hpp"

	Project: WorldSim
	Created: 10/12/2017, 0272632857.
	Updated: 10/12/2017, 0272632857.

	Description:
	The World stores local-level data, such as the position of local units.
	
	There is only one world, but each tile on the world is a potential local_world, therefore different techniques must be used to simulate them. For example
	they must be swapped into and out of memory because it is impossible to hold all of them in memory at once. For this reason there is only limited interactivity between neigboring maps.
  
  In order to play well with the global map and borders between tiles, local maps are subject to certain constraints in how they are generated. For example, it would be bad if a coastal tile was generated which only had land along a thin strip along the edge. This would make it impossible to place a coastal city on that tile. Therefore coastal tiles are guaranteed to have a certain amount of land. This also reduces the amount of memory taken up by local maps.

  Here are some attributes that local maps can have:
  
  Coast - A local map can have coast on 1 or more edges.
  Island - This map has coast along all 4 edges. This situation is special because islands are allowed to have more variation in the amount of land they have.
  
  Biome type - Each map has a set base biome type which reflects its global tile biome. A tile can't contain multiple biomes unless it is bordering another biome.

  River - A tile may have a river running through it. Rivers will run between tile boundaries. This is because if you build a fortress or move an army, it's important to know which side of the river it is on.
  
  
  World_Local should have a custom texture allowing a kind of thumbnail view of the tile.
  
  
	Notes:

	0272632857 - Added basic documentation.

*/

class World_Local: public LogicTickInterface, public IdleTickInterface
{
	private:
	
	public:
  
	int globalX, globalY; /* The local world's position in the world. */
  
	World_Local();
  
  virtual ~World_Local()
  {
    
  }
  
  
  // Local RNG
  RandomNonStatic random;
	
		/* The size of the world, measured in tiles. */
	int nX, nY;
	
    // The generation seed for this local map.
  int seed;
  
	bool active; /* Whether or not the world should be simulated. */
	bool generated; /* False until a world has been generated. Prevents trying to simulate a non-existent world. */
	
	long long unsigned int ticksBacklog; /* World will simulate these ticks whenever it can, while still relinquishing for input etc. */
	//Timer relinquishTimer;
	
	//RandomNonStatic random;

    //The base designated biome for this tile. Will influence generation.
  enumBiome baseBiome;
	
	ArrayS2 <bool> aIsLand;
  
  
    /* LOCAL MAP ARRAYS */
  ArrayS2 <LocalTile> aLocalTile;
    //Underground map
  ArrayS2 <LocalTile> aSubterranean;
  
  // Vector of all tile coordinates.
	Vector <HasXY*> vAllTiles;



	
	void init(int /*x*/, int /*y*/);
	
	bool generate();
  
	bool generateTestMap();
	
	bool saveToFile(std::string /* path */);
  
  bool moveObject (WorldObject* , int /* newX */, int /* newY */ );
  
// LINE OF SIGHT
    
      // Returns true if an object on this tile can block line of sight */
  bool isBlockingView(int /* _x */, int /* _y */);
  
    //Return a vector of coordinates visible from the given location.
  Vector <HasXY*> * rayTraceLOS (int /* _x */, int /* _y */, const int /* RANGE */);
	
};


#endif
