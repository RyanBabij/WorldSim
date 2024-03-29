#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_HPP
#define WORLDSIM_WORLD_LOCAL_HPP

/* WorldSim: World_Local.hpp
#include "World_Local.hpp" */

/**

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

World_Local has 2 types of data. The first is essential data needed for the world view. This is always initialised.
The second type of data is only generated when necessary. It's basically in-depth data about the tile used for local view.

*/

#include "LocalTile.hpp"

#include "Static.hpp"
#include "Static_Flora.hpp"

#include "Creature_Species.hpp"

#include <Interface/IdleTick/IdleTickInterface.hpp>
#include <Interface/LogicTick/LogicTickInterface.hpp>
#include <Game/Calendar/Calendar.hpp>
#include <Math/Random/RandomLehmer.hpp>
#include <Container/Bitfield/Bitfield.hpp> // for abstract collision data

#include <map>
#include <atomic> // thread-safe vars

class Creature;
class Item;
class Tribe;

class WorldObject_Tree;
class World_Biome;


class World_Local: public LogicTickInterface, public IdleTickInterface, public HasTexture
{
private:

	RandomLehmer rng;
	// The generation seed for this local map.
	// Used to derive heightmaps and spawning
	unsigned int seed;

	Vector <WorldObject_Tree*> vTree;
	// list of all types of flora in the game
	//Vector <Flora*> vFlora;

	// Simplified world data for use in fast abstract simulations.
	// Should generally always be loaded in, but I'd like to add caching support regardless.

	// It looks like storing all collision, flora and creatures on arrays will not be possible.
	// However per-biome simulation should be possible, we could even group biomes together into
	// chunks which we know can fit into memory.

	// The only other easy alternative is each tile having a map lookup with coordinates and object ID.
	// However in cluttered maps this will not work well and probably won't be sustainable long term.

	// Grouping data by biome is probably the most sensible option.

	// in future we could probably have a generate function which solely deals in abstract data, and lets
	// the details get filled in later. For example just randomaly put down movement blockers in the
	// collision bitfield and later on we can actually generate their object data.
	struct AbstractData
	{
		// arrays we need:
		// collision (0-1)
		// statics/improvements (0-255)
		// mobs (NPCs and creatures) (0-255)


		// simplified collision map for pathing. Tile is either accessible or not.
		Bitfield bfCollision;

		//Statics have an id from 1-255 so we can just use char array
		// Statics can be constructions, trees or other flora.
		ArrayS2 <unsigned char> aStaticID;

		// we can just use food value bitfield and update it every 24 hours
		//Bitfield bfFood;

		// location of flora. Must be looked up in flora vector.
		// this can obviously be optimised but I'll do it later.
		//Bitfield bfFlora;

		//Bitfield bfStatic;
		// location of creatures. Must be looked up in creature vector
		//Bitfield bfMob;


		//std::map can be used as a sparse array. Push a 2D point with the ID value.
	};

	// Data stores all data which is only present in generated maps.
	// This is done to keep memory footprint low, because million of maps are generated on large worlds.
	// This basically combines all pointers to these arrays into a single pointer.
	struct Data
	{
		// /* LOCAL MAP ARRAYS */
		ArrayS2 <LocalTile> aLocalTile; // Array of all tiles on map.
		ArrayS2 <Static*> aStatic; // Array of static objects

		// Vector of all tile coordinates.
		// This is used to do things like loop through every coordinate once in random order
		// (using shuffle).
		// This should be a static function.
		//Vector <HasXY*> vAllTiles;

		// // Lists of all objects of type on this map.
		// // Useful for counting or looping through each object of type.
		// // Vector of all Creatures on this map
		// Vector <Creature*> vCreature;
		// //Vector of all Characters on this map
		// Vector <Character*> vCharacter;
		// //Vector of all Items on this map
		// Vector <Item*> vItem;
		// // // Vector of all non-categorised objects on this map.
		// Vector <WorldObject*> vObjectGeneric;

	};

	// Subterannean data is generated separately from above-ground data
	struct Data_Subterranean
	{
		ArrayS2 <LocalTile> aSubterranean;  // Array of underground layer. Used for caves, tunnels, mines.
	};

	// Keeps track of influence values for each tribe.
	std::map<Tribe*,int> mInfluence;

	public:
	Data * data;
	Data_Subterranean * dataSubterranean;
	AbstractData * abstractData;

	World_Biome* biome;

	unsigned long int accessNumber; // Increments each time the map is accessed.

	// Texture of map from far away. This texture is used to rende the tile when zoomed out far.
	Texture texFar;

	Calendar localDate; /* The date that this map has been updated to. */

	/* Special features */
	bool hasCave; // should be ncaves
	bool hasRuin; // should be nruins

	//WORLD DATA
	std::atomic <short int> globalX_TS;
	std::atomic <short int> globalY_TS;
	short int globalX, globalY; /* The local world's position in the world. */
	std::atomic <bool> threadAccess;
	std::atomic <bool> initialized;
	std::atomic <bool> active;

	int testValue;

	//WORLD DATA
	//short int globalY; /* The local world's position in the world. */

	// How many metals may be mined from this tile.
	short int baseMetal;

	// texture pointer

	//enumBiome biome; /* Determines what it looks like and is called */
	//short int baseMoveCost; /* how many ap to move onto the tile. */
	//bool canHaveSettlement;
	short int baseFertility;
	bool canMove; /* True if units can walk over it. */
	//char baseLogisticsCost; /* Will consume logistics from armies and navies. Used to prevent armies from travelling through desert, and early navies from travelling through ocean. */
	//char defensiveBonus;

	// The id of the landmass.
	short int landID;
	// The id of the biome.
	short int biomeID;

	short int hasRiver; /* In future will be expanded to have more detailed info about river direction etc */
	char riverConnections; // NW N NE E SE S SW W. There should be at least 2.

	// WILDLIFE ABSTRACTION
	// These vars determine maximum herbivore population, and by extension, maximum carnivore population.
	unsigned int nFloraGrass; // Grass, low quality food
	unsigned int nFloraGood; // Shrubs and fruit, high energy food
	unsigned int nFloraDifficult; // Food in trees, only some creatures can eat it.
	
	
	// the height of the center tile. Used to build the remaining heightmap.
	// The value is based on the tile type and is randomised on init.
	unsigned short int centerHeight;
	ArrayS2 <char> aHeightDiff; //each value is the height difference from the neighboring tile closest to the center.


	ArrayS2 <int> aFullHeight; // should be short int in future.
	//Return a string with the name of the terrain.
	std::string getTerrainName();

	//Add influence from the particular tribe for this tile. 
	void addInfluence (Tribe* tribe, int amount);
	//Remove influence from the particular tribe for this tile. 
	void removeInfluence (Tribe* tribe, int amount);
	//Lower all influence by a certain amount.
	void degradeInfluence (int amount);
	//Erase the influence entry of this tribe.
	void destroyInfluence (Tribe* tribe);

	// Return the tribe with the greatest influence on the tile.
	Tribe* getDominantInfluence ();
	// Return the value of the greatest influence on the tile.
	int getDominantInfluenceValue ();

	// LOCAL MAP DATA

	// Local RNG
	//RandomNonStatic random;

	/* The size of the world, measured in tiles. */
	short int nX, nY;

	long long unsigned int ticksBacklog; /* World will simulate these ticks whenever it can, while still relinquishing for input etc. */

	//The base designated biome for this tile. Will influence generation.
	enumBiome baseBiome;

	// This should be moved into LocalTile.
	//ArrayS2 <bool> aIsLand;


	/* LOCAL MAP ARRAYS */
	//ArrayS2 <LocalTile> aLocalTile; // Array of all tiles on map.
	//ArrayS2 <LocalTile> aSubterranean;  // Array of underground layer. Used for caves, tunnels, mines.

	// Vector of all tile coordinates.
	// This is used to do things like loop through every coordinate once in random order
	// (using shuffle).
	//Vector <HasXY*> vAllTiles;

	// A vector with a coordinate object for every tile on a local map.
	// Useful for randomly looping through all tiles in a map.
	//static Vector <HasXY*> vAllTiles;

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

	//bool hasRiver; /* For now just a basic bool. In future it will need to define the river sides. */


	// Initialisation
	World_Local();
	virtual ~World_Local();
	void unload();
	void init(const int _globalX, const int _globalY, const enumBiome _biomeID, const int _seed /*=0*/, const int _hasRiver /*=-1*/);

	// Access
	inline LocalTile* operator() (int _x, int _y);

	// Returns true if the coordinate is inside this map
	bool isSafe(int /* x */, int /* y */);
	bool isSafe(HasXY*);
	bool isSafe(WorldObject*);

	bool isLand(); // returns true if this is a land tile.

	// Generate a world or load from cache.
	bool generate(bool cache=true, World_Local* c0=0, World_Local* c1=0, World_Local* c2=0,
	World_Local *c3=0, World_Local* c4=0, World_Local* c5=0, World_Local* c6=0, World_Local* c7=0);
	bool generateSubterranean(); // subterranean maps should be generated on demand.
	bool save();
	std::string getSaveData();
	bool load();
	
	void generateHeightMap(const short int c0=0, const short int c1=0, const short int c2=0, const short int c3=0,
	const short int c4=0, const short int c5=0, const short int c6=0, const short int c7=0, const unsigned int _seed=0);
	
	// SPAWN FUNCTIONS
		// spawn a random creature from Biome species list.
	Creature* spawnCreature();

	//bool saveToFile(std::string /* path */);

	// PUT FUNCTION
	// Overloaded put function automatically sorts object into relevant lists.
	// It's important to account for all categories otherwise it will implicitly
	// overload to a generic. And you will never see it again.
	bool put (WorldObject* , int /* _x */, int /* _y */, bool subterranean=false);
	bool put (Item* , int /* _x */, int /* _y */, bool subterranean=false);
	bool put (Character* , int /* _x */, int /* _y */, bool subterranean=false);
	bool put (Creature* , int /* _x */, int /* _y */, bool subterranean=false);
	//bool put (WorldObject_Tree* , int /* _x */, int /* _y */, bool subterranean=false);

	bool put (WorldObject* _object, HasXY* _xy, bool subterranean=false);
	bool put (Item* _object, HasXY* _xy, bool subterranean=false);
	bool put (Character* _object, HasXY* _xy, bool subterranean=false);
	bool put (Creature* _object, HasXY* _xy, bool subterranean=false);
	//bool put (WorldObject_Tree* _object, HasXY* _xy, bool subterranean=false);

	bool put (WorldObject* _object, HasXY _xy, bool subterranean=false);
	bool put (Item* _object, HasXY _xy, bool subterranean=false);
	bool put (Character* _object, HasXY _xy, bool subterranean=false);
	bool put (Creature* _object, HasXY _xy, bool subterranean=false);
	//bool put (WorldObject_Tree* _object, HasXY _xy, bool subterranean=false);

	bool put (Static* _static, int /* _x */, int /* _y */, bool subterranean = false);  
	bool put (Static* _static, HasXY _xy, bool subterranean = false);  
	bool put (Static* _static, HasXY* _xy, bool subterranean = false);  

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

	// Uses local map coordinates, but will automatically move object to adjacent maps if necessary.
	// This shouldn't be used too much because most movement will probably be discrete steps NESW, in order to calculate
	// wall collision and whatnot. This is more of a teleport function.
	bool moveObject (WorldObject* , int /* newX */, int /* newY */ );
	bool moveObject (Character* , int /* newX */, int /* newY */ );

	// Move object one move in the passed direction (NORTH, EAST, SOUTH, WEST).
	// Multiple moves can just repeatedly call this function if necessary.
	bool moveObject(WorldObject*, const enumDirection);

	// Move object up or down the z layer. Currently there are only 2 z-layers.
	bool moveDown(WorldObject*);
	bool moveDown(Character*);
	bool moveUp(WorldObject*);
	bool moveUp(Character*);

	bool wander (WorldObject* /* _object */);
	bool wander (Character* /* _object */);
	bool wander (Creature* /* _object */);

	//bool removeItem (Item* /* _item */);

	//SEARCH FUNCTIONS

	Vector <Character*> * getAdjacentCharacters(int /* _x */, int /* _y */);

	HasXY* getRandomTile();
	HasXY* getRandomNeighbor(HasXY*);
	//Vector <HasXY*> ge

	Vector <HasXY*> * getRandomWalk(int /* nSteps */);

	// LINE OF SIGHT

	// Returns true if an object on this tile can block line of sight */
	bool isBlockingView(int /* _x */, int /* _y */);

	//Return a vector of coordinates visible from the given location.
	Vector <HasXY*> * rayTraceLOS (int /* _x */, int /* _y */, const int /* RANGE */, const bool subterranean=false);

	//Return a vector of coordinates visible from the given location.
	// New version using global coordinates
	Vector <HasXY2 <unsigned long int> *> * rayTraceLOS (long unsigned int /* _x */, long unsigned int /* _y */, const int /* RANGE */);

	// Trace 1 ray, and adds visible coords to vector.
	void rayTrace (int /* _x */, int /* _y */, int /* _x2 */, int /* _y2 */, Vector <HasXY*> * /* vVisibleTiles */ , const bool subterranean=false);

	// LOGIC
	void incrementTicks(int nTicks);
	void updateTickBacklog(Calendar);

	// HASTEXTURE
	// Return map tile texture
	virtual Texture* currentTexture();
};


#endif
