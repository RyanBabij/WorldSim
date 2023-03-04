#pragma once
#ifndef WORLDSIM_WORLD_HPP
#define WORLDSIM_WORLD_HPP

/* WorldSim: World.hpp
#include "World.hpp"

The World stores global-level data, such as the position of global units.

Map caching and generation is to be handled by World_MapManager.

Local Map data is stored in World_Local.

*/

#include "Mythology.hpp"
#include "Mythology_Deity.hpp"

#include "World_Events.cpp"
#include "World_Astronomy.hpp"
#include "LocalTile.hpp"
#include "World_Local.hpp"
#include "World_Landmass.hpp"
#include "World_Biome.hpp"
#include "World_MapManager.hpp"

#include <Graphics/Png/Png.hpp>
#include <File/FileManager.hpp>
#include <Container/ArrayS2/ArrayS2.hpp>
#include <Interface/IdleTick/IdleTickInterface.hpp>
#include <Interface/LogicTick/LogicTickInterface.hpp>
#include <System/Time/Timer.hpp>
#include <System/Time/Calendar.hpp>
#include <File/SaveFileManager.hpp>
#include <Math/Random/GlobalRandom.hpp>
#include <Game/NameGen/NameGen.hpp>

#include <atomic>
#include <map>

class Civ;
class Civ_Dwarven;
class WorldObjectGlobal;
class Character;
class Tribe;
class Tribe_Human;
class Tribe_Dwarven;
class Tribe_Elf;

class World: public LogicTickInterface, public IdleTickInterface, public SaveFileInterface
{
	private:
	RandomNonStatic random;
	World_MapManager mapManager;
	// We could remove this later by using a pointer.
	ArrayS2 <enumBiome> aTerrain;
	
	Vector <Mythology_Deity*> vDeity;
	
	// There are 3 hardcoded base mythologies, one for each race. However they can mutate in a dynamic way.
	Mythology mythologyDwarven;
	Mythology mythologyElven;
	Mythology mythologyHuman;

	public:

	// This flips to true once the player has entered the world simulator, which calls startSimulation().
	// some functionality shouldn't work if the world isn't active, especially something like generating
	// local maps.
	bool active; 


	#if defined WILDCAT_THREADING
	std::atomic <bool> generated;
	#else
	bool generated; /* False until a world has been generated. Prevents trying to simulate a non-existent world. */
	#endif

	// The size of the world, measured in tiles.
	int nX, nY;
	// The maximum global coordinate
	unsigned long int maximumX, maximumY;

	int simX, simY;

	/* The current tile that the player wants information on */
	int queryWorldX, queryWorldY; 
	// same but for local map tile (if it exists)
	int queryWorldXLocal, queryWorldYLocal; 

	long long unsigned int ticksBacklog; /* World will simulate these ticks whenever it can, while still relinquishing for input etc. */
	bool incrementContinuous;
	Timer relinquishTimer;

	// This array stores the base terrain data.
	//ArrayS2 <enumBiome> aTerrain;

	std::string name; /* The name of the world */
	std::string strSavePath; /* The path to the world's save data. Typically something like: "savedata/<world name>". */
	std::string worldFilePath;

	// Stores astronomical details from perspective of World.
	World_Astronomy astronomy;

	int landmassSeed;

	/* Counters to regulate logic */
	int dailyCounter;
	int monthlyCounter;

	/* This array stores the colour map for rendering. */
	ArrayS3 <unsigned char> aTopoMap;

	ArrayS2 <WorldObject*> aWorldObject;
	Vector <WorldObjectGlobal*> vWorldObjectGlobal;

	// Stores all tile-specific information
	ArrayS2 <World_Local> aWorldTile;
	// A vector of all local maps in memory. These worlds can be rendered and simulated.
	Vector <World_Local*> vWorldLocal;

	// All of these should be ported into aWorldTile.
	ArrayS2 <int> aBiomeID;
	ArrayS2 <int> aRiverID; /* Contains the river ID */

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

	Vector <Tribe*> vTribe;
	Vector <Civ*> vCiv;

	ArrayS2 <unsigned char> aGoodEvil;


	Calendar calendar;
	Calendar lastDate; /* The date on the last logic tick. */

	EventsManager events;

	// STATES
	bool isRaining;
	
	// keep track of how many ticks for deity actions
	int deityTicks;

	World();
	~World();

	// CHECK ALL CIV LOS ARRAYS, AND IF ANY OVERLAP, THEN MAKE THEM DISCOVER EACH OTHER.
	//void updateCivContacts();

	// Return a World_Local object for this local map. Will generate a local map if necessary. Returns null pointer for
	// invalid request. Also returns null pointer if world is not ready yet. Do not call this function until you are in
	// World Simulator mode unless you like hard-to-track crashes.
	inline World_Local* operator() (const int _x, const int _y);

	//New operator: Gets tiles using global coordinates.
	// IT DOES NOT RETURN WORLD_LOCAL LIKE THE OTHER FUNCTION
	// Yeah this is a confusing overload which needs to be fixed in the future.
	// This will generate a local map if necessary.
	inline LocalTile* operator() (unsigned long int _x, unsigned long int _y, const bool subterranean=false);

	inline LocalTile* operator() (HasXY2 <unsigned long int>*, const bool subterranean=false);

	// COORDINATE CONVERSIONS

	// Pass absolute coordinates and recieve world and local relative coordinates.
	bool absoluteToRelative (const unsigned long int _absoluteX, const unsigned long int _absoluteY, int * _globalX, int * _globalY, int * _localX, int * _localY);

	// Returns true if the tile is in a map that is loaded.
	bool isGenerated(unsigned long int _absoluteX, unsigned long int _absoluteY);
	
	bool generateMythology(); // Generate the base mythology
	
	
	// STATE CHANGES
	// Transition from generation to simulation.
	void startSimulation();
	// Load up the required local map, do any required logic, center map on character. Return false if error.
	bool prepareAdventureMode( Character * );

	/* TICK LOGIC */
	/* I want this to be the new method of incrementing time in the world. It will dynamically abstract things based on the amount of turns to simulate. Each tick is one second. */
	void incrementTicks ( int /* nTicks */ );
	void incrementTicksBacklog ( long long unsigned int /* nTicks */ );
	bool handleTickBacklog ();
	void idleTick();
	void logicTick();

	/* UPDATES */
	/* World updates not relying on ticks. For example loading in maps that the player is near. */
	void updateMaps();

	/* FILE IO */
	bool loadWorld(std::string /* world name */);

	/* WORLD GENERATION */

	void generateWorld (const std::string /* _name */, const int /* x */, const int /* y */, int /* seed */, int /* fragmentation */, const bool /* islandMode */, const bool /* wrapX */, const bool /* wrapY */, const double /* landPercent */);
	// Generate a local map so it can be viewed and interacted with.
	void generateLocal(const int /* worldX */, const int /* worldY */);
	void generateLocal(HasXY*);
	//Unload a local map from RAM and into a save file.
	void unloadLocal(const int /* worldX */, const int /* worldY */);

	void buildArrays(WorldGenerator2& wg);
	// Find all unique areas and give them names.
	void nameRegions();
	void generateTribes( int/* nTribes */, int /* nTribesDwarven */, int /* nTribesElven */);
	//Put down some Dwarven civs.
	bool addRace (int /* nTribes */, std::string /* name */);
	//bool addElvenTribe(int nTribes);
	//bool addHumanTribe(int nTribes);
	
	
	// Since the races are hardcoded, they should be their own functions.
	void generateDwarves(int /* nCivs */ ); // Dwarves spawn directly into mountain tiles
	void generateElves(int /* nTribes */ );
	void generateHumans(int /* nTribes */ );

	// Ensure conditions are set up for this character to be controlled.
	// For example generate the local map.
	void controlCharacter (Character*);

	Character* getRandomCharacter();

	// Return vector of coordinates visible from given coordinate.
	Vector <HasXY2 <unsigned long int> *> * rayTraceLOS (unsigned long int _x, unsigned long int _y, const int RANGE, const bool /* isSneaking = false */, const bool subterranean=false);
	void rayTrace (unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2, Vector <HasXY2 <unsigned long int> *> * vVisibleTiles, bool subterranean=false);

	/* WORLD QUERIES */
	/* Returns population of all characters (Human, Dwarven, Elven)
	alive in the world. */
	int getPopulation();

	/* WORLD TILE / COORDINATE QUERIES */

	// RETURNS TRUE IF THE TILE IS LAND. RETURNS FALSE IF IT IS NOT LAND, OR IF IT IS OUT OF BOUNDS.
	bool isLand(int _x, int _y);
	inline bool isLand(HasXY*);

	// RETURNS TRUE IF THE COORDINATES ARE WITHIN BOUNDS OF THE MAP.
	// We need to integrate HasXY with template.
	bool isSafe (int _x, int _y);
	// SAME BUT FOR ABSOLUTE COORDINATES.
	bool isSafe (unsigned long int _x, unsigned long int _y);

	/* Return a pointer to the WorldTile at the coordinates.
	Return null pointer if fail */
	World_Local * getTile (const int /* x */, const int /* y */ );
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
	
	HasXY* getRandomLandTile();

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

	World_Biome* getBiome(const int id);
	std::string getBiomeName( const int /* x */, const int /* y */);

	//INFO GETTING FUNCTIONS
	std::string getTileType (const int _x, const int _y);
	// provide info on the info panel for this tile
	void queryTile( int hoveredXTile, int hoveredYTile);
	// provide info on the info panel for this local tile
	void queryTileLocal( int hoveredXTile, int hoveredYTile);

	// TRIBE FUNCTIONS
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
	
	// Allow deities to act
	void incrementDeities(int /* nTicks */ );

	// Return the nearest tribe to the passed tribe which is on the same landmass.
	// Return 0 if none. Only check same race by default.
	Tribe * getNearestConnectedTribe (Tribe *, bool sameRace = true);

	int getHighestInfluence(const int, const int);
	int getHighestInfluence(HasXY*);
	
	bool hasSettlement(const int, const int);

/** This function builds a minimap texture which is later rendered in the world simulator. */
	void buildMinimap();

//SAVEFILEINTERFACE
	virtual void save();

};

#endif
