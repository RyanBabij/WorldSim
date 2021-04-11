#pragma once
#ifndef WORLDSIM_WORLD_BIOME_HPP
#define WORLDSIM_WORLD_BIOME_HPP

/* WorldSim: World_Biome.hpp
#include "World_Biome.hpp" */

/**

Biomes are essentially collections of unique environments which also include unique flora and fauna. In this respect
biomes are a kind of container for these objects.

Much processing and simulation is handled on the biome level as these are the smallest discrete "chunks" of world which
may be independently simulated. This might cause complications with overly-large biomes, so I am considering solutions
for that, including splitting the biomes up into sub-biomes.

Local Map, Creature and Flora generation is all handled per-biome. Tribes and Civilizations are not.

Biome types are currently hardcoded, and it will probably stay that way. Here are the biome enums:
NOTHING=0, OCEAN=1, GRASSLAND=2, FOREST=3, DESERT=4, MOUNTAIN=5, SNOW=6, HILLY=7, JUNGLE=8, WETLAND=9, STEPPES=10, CAVE=11, RUIN=12, ICE=13

OCEAN and ICE are special cases which are not simulated, because nobody will be seeing much of them anyway.

There should be a worldgen setting: LARGEST_BIOME_SIZE. If a biome is generated that is too large (except water), it should be removed and then a new biome layer is generated, preserving all good biomes.

The process for generating biomes should be:

	* Generate land
	* Generate flora and creatures
	* Spawn flora and creatures
	
While this is happening, the biome should be marked as "initializing" and cannot be touched.

*/

#include "Static_FloraManager.hpp"
#include "Creature_Manager.hpp"

class World_Local;

#include <Game/WorldGenerator/WorldGenerator2.hpp>
#include <Container/Table/TableInterface.hpp>
#include <System/Sleep/Sleep.hpp>

class World_Biome: public TableInterface
{
	RandomLehmer rng;
	//FloraGenerator floraGenerator;
	FloraManager floraManager;
	Creature_Manager creatureManager;

	public:

	bool isGenerated; // true if all tiles have been generated.
	bool threadAccess; // true if a thread is using this biome
	
	bool isFloraGenerated; // flag so we only generate flora once for this biome.

	int id; // the biome's id. Used to lookup biome tables. The id is equal to it's position in the biome vector.

	std::string name;
	unsigned int size; /* size in tiles */
	unsigned char type; /* Biome type */

	Vector <HasXY> vXY; // every tile coordinate of this biome.
	Vector <World_Local*> vMap; // pointer to every map on this biome

	// placeholders for objects

	Vector <Flora*> vFlora; // Vector of Flora types
	//Vector <std::string> vHerbivore;
	//Vector <std::string> vCarnivore;
	
	Vector <Creature*> vCreature;
	
	double averageX, averageY; /* For zooming to biome. */
	int centerX, centerY; /* The closest tile to the average which is owned by the biome */
	

	World_Biome();
	virtual ~World_Biome();
	
	// generate entire biome including terrain, flora, etc.
	void generate(const unsigned short int sleepTime=0);

	// generate all local maps for this biome
	// map generation will be done per-biome rather than per tile
	void generateLocals(const unsigned short int sleepTime=0);

	void generateFlora();
	
	void generateCreatures();
	
	// pick a flora type from the weighted list to spawn
	Flora* getFlora();
	
	// return all flora types on this biome
	Vector <Flora*> * getAllFloraTypes();
	// return all Creature types on this biome
	Vector <Creature_Species*> * getAllCreatureTypes();
	
	void getAverageCoordinates(/*Vector <HasXY*> * vXY*/);
	
	void addMap(World_Local * map);
	
	
	// TableInterface
	std::string getColumn(std::string _column);
	// TableInterface
	std::string getColumnType(std::string _column);
};

#endif