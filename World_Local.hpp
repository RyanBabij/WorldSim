#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_HPP
#define WORLDSIM_WORLD_LOCAL_HPP

#include <IdleTick/IdleTickInterface.hpp>
#include <LogicTick/LogicTickInterface.hpp>

/* WorldSim: World_Local.hpp
	#include "World_Local.hpp"

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
  

*/

// #include "Creature.hpp"
// #include "Creature_Deer.hpp"
class Creature;

class Item;

class World_Local: public LogicTickInterface, public IdleTickInterface
{
	private:
	
  
	public:
  
	int globalX, globalY; /* The local world's position in the world. */

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
	
    // This should be moved into LocalTile.
	ArrayS2 <bool> aIsLand;
  
  
    /* LOCAL MAP ARRAYS */
  ArrayS2 <LocalTile> aLocalTile; // Array of all tiles on map.
  ArrayS2 <LocalTile> aSubterranean;  // Array of underground layer. Used for caves, tunnels, mines.
  
  // Vector of all tile coordinates.
  // This is used to do things like loop through every coordinate once in random order
  // (using shuffle).
	Vector <HasXY*> vAllTiles;

    // Lists of all objects of type on this map.
    // Useful for counting or looping through each object of type.
  // Vector of all Creatures on this map
  Vector <Creature*> vCreature;
  //Vector of all Characters on this map
  Vector <Character*> vCharacter;
  //Vector of all Items on this map
  Vector <Item*> vItem;
  // Vector of all non-categorised objects on this map.
  Vector <WorldObject*> vObjectGeneric;
  
  bool hasRiver; /* For now just a basic bool. In future it will need to define the river sides. */

  
    // Initialisation
	World_Local();
  virtual ~World_Local();
	void init(int /*x*/, int /*y*/);
  
  // Access
  inline LocalTile* operator() (int _x, int _y);
  
    // Returns true if the coordinate is inside this map
  bool isSafe(int /* x */, int /* y */);
    bool isSafe(HasXY*);
  
	
	bool generate();
  
	bool generateTestMap();
	
	bool saveToFile(std::string /* path */);
  
    // PUT FUNCTION
    // Overloaded put function automatically sorts object into relevant lists.
    // It's important to account for all categories otherwise it will implicitly
    // overload to a generic. And you will never see it again.
  bool put (WorldObject* , int /* _x */, int /* _y */);
    bool put (Item* , int /* _x */, int /* _y */);
    bool put (Character* , int /* _x */, int /* _y */);
    bool put (Creature* , int /* _x */, int /* _y */);
    
    // Remove object from world, but don't delete it.
    // For example if somebody picks it up.
  bool remove (WorldObject*);
    bool remove (Item*);
    bool remove (Character*);
    bool remove (Creature*);
    // Remove object from world and delete it.
    // For example if it is consumed.
  bool erase(WorldObject*);
    bool erase (Item*);
    bool erase (Character*);
    bool erase (Creature*);
  // Return true if the map has this object in it
  bool contains(WorldObject*);
    bool contains(Character*);
  
  
  bool moveObject (WorldObject* , int /* newX */, int /* newY */ );
    bool moveObject (Character* , int /* newX */, int /* newY */ );
  
  
  
  bool wander (WorldObject* /* _object */);
    bool wander (Character* /* _object */);
    bool wander (Creature* /* _object */);
  

  //bool removeItem (Item* /* _item */);
  
//SEARCH FUNCTIONS

  Vector <Character*> * getAdjacentCharacters(int /* _x */, int /* _y */);
  
  HasXY* getRandomTile();
  
// LINE OF SIGHT
    
      // Returns true if an object on this tile can block line of sight */
  bool isBlockingView(int /* _x */, int /* _y */);
  
    //Return a vector of coordinates visible from the given location.
  Vector <HasXY*> * rayTraceLOS (int /* _x */, int /* _y */, const int /* RANGE */);
  
    //Return a vector of coordinates visible from the given location.
    // New version using global coordinates
  Vector <HasXY2 <unsigned long int> *> * rayTraceLOS (long unsigned int /* _x */, long unsigned int /* _y */, const int /* RANGE */);
  
  
    // Trace 1 ray, and adds visible coords to vector.
  void rayTrace (int /* _x */, int /* _y */, int /* _x2 */, int /* _y2 */, Vector <HasXY*> * /* vVisibleTiles */ );
  
  // LOGIC
  void incrementTicks(int nTicks);
	
};


#endif
