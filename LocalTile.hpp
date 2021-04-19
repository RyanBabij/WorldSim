#pragma once
#ifndef WORLDSIM_LOCALTILE_HPP
#define WORLDSIM_LOCALTILE_HPP

/* WorldSim: LocalTile
	#include "LocalTile.hpp"

LocalTile stores tile info on the local maps. Stuff like objects on that tile, etc.

Basic details like heightmap are stored as a seed, to cut down on memory footprint. Tile can be constructed into an array on demand. Modifications to the world are stored seperately.

At 1:1 scale a tile is 5km by 5km, ie, 5,000 * 5000 tiles, however we will probably end up using tiles of size 513 because anything larger will have a huge memory footprint without special code.

In the future we might have polymorphic classes to distinguish simulated tiles from abstract tiles. Abstract tiles will basically be tiles that nobody has seen, and therefore they don't need data. However this may not be possible if wildlife is to be simulated.

	A tile may have various things on it:
		Terrain - All tiles have terrain. It is a fixed set of values.
		Objects - On a set of vectors, split up to aid search performance.
		Static - A single Static object which may block movement or line of sight
		Evidence - Some kind of temporary marking.
*/

#include <Interface/HasTexture.hpp>

#include <Game/WorldGenerator/Biome.hpp>

class WorldObject;
class Item;
class Creature;
class Creature_Footprint;
class Character;
class Static;

class LocalTile: public HasTexture
{
public:

	LocalTile();
	~LocalTile();

	/* Every local tile must have a seed to help determine random things,
	for example which variant of texture to draw. */
	// Actually this is probably not necessary.
	unsigned short int seed;
	enumBiome baseTerrain;
	//Base terrain can be overlaid with a floor.
	// For now just a bool.
	unsigned char hasFloor;

	// Walls block movement across them
	// Bitfield is used to determine the wall orientation.
	// The first 4 bits control travel OUT from the tile. NESW.
	// The last 4 bits control travel INTO the tile. NESW.
	// This allows one-way travel, which would be useful for traps, pits, etc.
	unsigned char bWall;
	
	// same but for cliffs.
	//unsigned char bCliff;
	// change this to a simple bool
	bool bCliff;

	short int height;

	bool isLand; /* True if not water */
	//bool hasGems;
	//bool hasMetal;
	bool isCave;

	unsigned short int nGems;
	unsigned short int nMetal;

	// bool isUphill [8]; /* Clockwise starting north */


	/* INTERACTION STUFF */
	int nFish; /* How many fish this tile has. Set to -1 if fishing is not possible here */

	/* RENDER STUFF */
	bool shotOverlay; /* Show the line of fire highlight for this tile */

	// Vector of objects on this tile.
	// This list includes all subclasses.
	// The reason there are so many duplicates is performance.
	Vector <WorldObject*> vObject;
	// Vector of non-specialised WorldObjects.
	Vector <WorldObject*> vObjectGeneric;
	// Vector of Items on this tile
	Vector <Item*> vItem;
	// Vector of Characters on this tile.
	Vector <Character*> vCharacter;
	// Vector of Creatures on this tile.
	Vector <Creature*> vCreature;

	Static* objStatic; // Static object (max 1)

	Creature_Footprint* footprint; // Creature Evidence (max 1)

	// Generic add/remove automatically sorts into appropriate lists.

	void add(WorldObject*);
	void add(Item*);
	void add(Character*);
	void add(Creature*);

	void remove(WorldObject*);
	void remove(Item*);
	void remove(Character*);
	void remove(Creature*);


	void clearObjects();


	// This returns the base terrain texture.
	virtual Texture* currentTexture();

	// Return vector of all textures to be drawn, to be drawn from index 0 to n-1.
	virtual Vector <Texture*> * currentTextures();

	// Returns true is this tile has an object that can block line of sight. */
	bool hasViewBlocker();
	// Returns true is this tile has an object that can block movement.
	bool hasMovementBlocker();

	std::string getName();


	bool canTravelNorth();
	bool canTravelEast();
	bool canTravelSouth();
	bool canTravelWest();

	std::string getAll(int /* max */); // return string listing all objects on this tile (up to max)

	std::string getSaveData();

	//Abstract data is just the bare minimum data to run simulations: Collision data and food values for wildlife.
	std::string getAbstractData();
	void loadData(std::string);

};

#endif
