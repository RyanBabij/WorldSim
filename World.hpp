#pragma once
#ifndef WORLDSIM_WORLD_HPP
#define WORLDSIM_WORLD_HPP

/* WorldSim: World.hpp
	#include "World.hpp"

	Description:
	The World stores global-level data, such as the position of global units.

*/

#include <Graphics/Png/Png.hpp>
#include <File/Filemanager.hpp>
#include <Container/ArrayS2/ArrayS2.hpp>
#include <map>


#include <Math/Random/GlobalRandom.hpp>

#include <NameGen/NameGen.hpp>
NameGen globalNameGen;


#include <LogicTick/LogicTickInterface.hpp>
#include <Time/Calendar.hpp>

class Character;
class Tribe;
class Tribe_Human;
class Tribe_Dwarven;
class Tribe_Elf;

#include "WorldTile.cpp"
#include "LocalTile.cpp"

#include "GuildCalendar.hpp"

#include "World_Local.hpp"

#include "WorldObjectGlobal.hpp"


#include "World_Landmass.hpp"
#include "World_Biome.hpp"



class Civ;
class Civ_Dwarven;

// class TribalTerritory
// {
	// public:
	
	// TribalTerritory()
	// {}
	
	// Vector <Tribe*> vTribeID;
	// Vector <int> vTribeValue;
// };

// The new smart way to store tile data. Array of WorldTile objects.

// class WorldTile
// {
	// public:
	
	// unsigned char biomeID;
	// unsigned char height;
	// unsigned char [3] colour;
	
// };

#include <IdleTick/IdleTickInterface.hpp>

#include <Time/Timer.hpp>

/*! A test class */

class World: public LogicTickInterface, public IdleTickInterface
{
	public:
	
		/* The size of the world, measured in tiles. */
	int nX, nY;
	
	bool active; /* Whether or not the world should be simulated. */
	bool generated; /* False until a world has been generated. Prevents trying to simulate a non-existent world. */
  
  int localX, localY; /* The current local map viewed by player */
  
  /* The current tile that the player wants information on */
	int queryWorldX, queryWorldY; 
  
	long long unsigned int ticksBacklog; /* World will simulate these ticks whenever it can, while still relinquishing for input etc. */
	Timer relinquishTimer;
	
	RandomNonStatic random;
	
  /* This array stores the base terrain data. */
	ArrayS2 <enumBiome> aTerrain;
	
		/* Overlays on top of base terrain. Each tile may have a 'point of interest' on it. */
		/* Points of interest:
			MINE
			URBAN - Anything from a hamlet to a citadel.
			DOCK (?)
			RUIN
			HIGHWAY - Movement is faster on these roads.
			BRIDGE - ALLOWS EASY MOVEMENT ACROSS WATER.
		*/
		
	std::string name; /* The name of the world */
	std::string strSavePath; /* The path to the world's save data. Typically something like: "savedata/<world name>". */
  std::string worldFilePath;
  
  /* SEED */
  int landmassSeed;
  
  /* Counters to regulate logic */
  int dailyCounter;
  int monthlyCounter;
	
	
	//ArrayS2 <unsigned char> aHeightMap;
		/* This array stores the colour map for rendering. */
	ArrayS3 <unsigned char> aTopoMap;
	
	ArrayS2 <WorldObject*> aWorldObject;
	Vector <WorldObjectGlobal*> vWorldObjectGlobal;
	
	
		// All tile-specific info is to go here. Data access with array syntax eg world(x,y).biomeID;
	ArrayS2 <WorldTile> aWorldTile;
	ArrayS2 <int> aSeed;
	ArrayS2 <int> aLandmassID;
	ArrayS2 <bool> aIsLand;
	ArrayS2 <int> aBiomeID;
  
  
    // A vector of all local maps in memory. These worlds can be rendered and simulated.
  Vector <World_Local*> vWorldLocal;
  

	
	//Vector <std::string> vLandmassName;
	Vector <World_Landmass*> vLandmass;
	Vector <World_Biome*> vBiome;
	
		// A vector of vectors used to get a random tile. Vector index represents the row. Vector value represents column. Shuffle row after use.
    // NOTE: UNFORTUNATELY THIS DOES NOT PRODUCE A RANDOM DISTRIBUTION.
  Vector < Vector <int> * > vAllTiles;
  // New approach, just a single vector. Unfortunately will take longer to shuffle.
  // Can be optimised by shifting begin and end pointers.
	Vector <HasXY*> vAllTiles2;

	
	unsigned char seaLevel;
	unsigned char mountainLevel;


  Character * playerCharacter;
  
	Vector <Tribe*> vTribe;
	Vector <Civ*> vCiv;
	
	ArrayS2 <unsigned char> aGoodEvil;

	
	GuildCalendar calendar;
	GuildDate lastDate; /* The date on the last logic tick. */
	
	World();
	
		// For testing purposes. This will dump a ton of data about everything ever into a file.
	void dump(std::string /* filename */);
	
		// CHECK ALL CIV LOS ARRAYS, AND IF ANY OVERLAP, THEN MAKE THEM DISCOVER EACH OTHER.
	//void updateCivContacts();
	



    /* TICK LOGIC */
		/* I want this to be the new method of incrementing time in the world. It will dynamically abstract things based on the amount of turns to simulate. Each tick is one second. */
	void incrementTicks ( int /* nTicks */ );
	void incrementTicksBacklog ( long long unsigned int /* nTicks */ );
	void handleTickBacklog ();
	void idleTick();
	void logicTick();
  
    /* FILE IO */
	bool loadWorld(std::string /* filepath */);
  
    /* WORLD GENERATION */
  
	void generateWorld (const std::string /* _name */, const int /* x */, const int /* y */, int /* seed */, int /* fragmentation */, const bool /* islandMode */, const bool /* wrapX */, const bool /* wrapY */, const double /* landPercent */);
		// Generate a local map so it can be viewed and interacted with.
	void generateLocal(const int /* worldX */, const int /* worldY */);
	void buildArrays();
		// Find all unique areas and give them names.
	void nameRegions();
	void generateTribes( int/* nTribes */, int /* nTribesDwarven */, int /* nTribesElven */);
		//Put down some Dwarven civs.
	bool addRace (int /* nTribes */, std::string /* name */);
	//bool addElvenTribe(int nTribes);
	//bool addHumanTribe(int nTribes);
  
    // Ensure conditions are set up for this character to be controlled.
    // For example generate the local map.
  void controlCharacter (Character*);
	
  /* WORLD QUERIES */
		/* Returns population of all characters (Human, Dwarven, Elven)
    alive in the world. */
	int getPopulation();
  
      /* WORLD TILE / COORDINATE QUERIES */
  
		// RETURNS TRUE IF THE TILE IS LAND. RETURNS FALSE IF IT IS NOT LAND, OR IF IT IS OUT OF BOUNDS.
	bool isLand(int _x, int _y);
		inline bool isLand(HasXY*);
	
		// RETURNS TRUE IF THE COORDINATES ARE WITHIN BOUNDS OF THE MAP.
	bool isSafe (int _x, int _y);
  
   /* Return a pointer to the WorldTile at the coordinates.
    Return null pointer if fail */
  WorldTile * getTile (const int /* x */, const int /* y */ );
		/* Return the fertility value of the tile. */
	int getTileFertility(const int /* x */, const int /* y */);
		int getTileFertility(const HasXY* /* xy */);
		/* Returns the total fertility of the passed tile, and its 8 neighbors. */
	int getSurroundingFertility (const int _x, const int _y);
	
		/* Return average hunting yield of tile. Hunting is all non-agricultural food.
		Hunting yield is the average of the surrounding tiles, plus the tile the tribe is standing on.
		This is important because coastal tiles provide high yield. */
	int getHuntingYield( const int _x, const int _y);
	
		/* Set the pointers to a coordinate of a random tile in the world. */
	void getRandomTile (int* x, int* y);
		/* Set the pointers to a coordinate of a random land tile in the world. May fail, in which case returns false and sets coords to (0,0). */
	bool getRandomLandTile(int* x, int* y);
	
	HasXY* getRandomTileOfType(enumBiome _type);
	
		// RETURN ANY OBJECTS WHICH ARE NEXT TO THE PASSED OBJECT.
	Vector <WorldObjectGlobal*>* getNeighboringObjects(WorldObjectGlobal*);
	
		// Put a worldobject anywhere in the world. If x,y are not provided
    // It will use the object's own coordinates.
	bool putObject(WorldObjectGlobal* _object, int x = -1, int y = -1);
    // Remove object from world.
  bool removeObject(WorldObjectGlobal* _object);
  
		//Return the name of the landmass the tile is on. Return empty string if not on a landmass.
	std::string getLandmassName(const int /* x */, const int /* y */);
		std::string getLandmassName (HasXY*);
    
  std::string getBiomeName( const int /* x */, const int /* y */);
    
	//INFO GETTING FUNCTIONS
	std::string getTileType (const int _x, const int _y);
	void queryTile( int hoveredXTile, int hoveredYTile);
	
  
  
      /* TRIBE FUNCTIONS */
	
	void addInfluence(Tribe* tribe, int amount);
		// Subtract an influence point from every tile this tribe has.
	void degradeInfluence(int amount=1);
    //Set influence to 0.
	void destroyInfluence(Tribe* tribe);
		/* Return pointer to tribe with most influence here. Return null pointer if no tribe */
	Tribe* getDominantInfluence (const int, const int);
	Tribe* getDominantInfluence (HasXY*);
  
  Vector <Tribe*>* getTribesOn(const int /* x */, const int /* y */);
  
  // Check if tribe is in engagement range of another tribe.
  // If yes, return pointer to Tribe, otherwise return 0.
  Tribe* combatCheck (Tribe* );
  
  //Returns true if this landmass has an unclaimed tile.
  bool hasFreeTerritory(int landmassID);
    // Return number of unclaimed tiles on this landmass.
  int nFreeTerritory (int landmassID);
	
  void evolveToCiv( Tribe * );
  
    // Return the nearest tribe to the passed tribe which is on the same landmass.
    // Return 0 if none. Only check same race by default.
  Tribe * getNearestConnectedTribe (Tribe *, bool sameRace = true);
  
	int getHighestInfluence(const int, const int);
		int getHighestInfluence(HasXY*);
	
};


#endif
