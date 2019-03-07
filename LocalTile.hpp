#pragma once
#ifndef WORLDSIM_LOCALTILE_HPP
#define WORLDSIM_LOCALTILE_HPP

/*
	LocalTile stores tile info on the local maps. Stuff like objects on that tile, etc.
*/

/* Basic details like heightmap are stored as a seed, to cut down on memory footprint. Tile can be constructed into an array on demand. Modifications to the world are stored seperately.

A tile is 5km by 5km, ie, 5,000 * 5000 tiles. */

// In the future we might have polymorphic classes to distinguish simulated tiles from abstract tiles. However for now it's not a major concern as the world map should need less than 100MB of RAM.

#include <Interface/HasTexture.hpp>

#include <WorldGenerator/Biome.hpp>

//class Tribe;
//#include "Tribe.hpp"

class Item;
class Creature;
class Character;

class LocalTile: public HasTexture
{
	public:
	
	LocalTile();
	~LocalTile();
  
  /* Every local tile must have a seed to help determine random things,
    for example which variant of texture to draw. */
  int seed;
  enumBiome baseTerrain;
    //Base terrain can be overlaid with a floor.
    // For now just a bool.
  unsigned char hasFloor;
  
    // Walls block movement across them
    // Bitfield is used to determine the wall orientation.
    // 000
    // 0 0 Where a 1 indicates movement is blocked.
    // 000
    // The bitfield is used for both pathfinding and rendering.
    // The bitfield blocks movement FROM and TO this tile.
    // Basically this makes it possible to make one-way areas. Might be useful for traps or ledges.
  unsigned char bWall;
  
  int height;
  
  bool isUphill [8]; /* Clockwise starting north */
  
  
  /* INTERACTION STUFF */
  int nFish; /* How many fish this tile has. Set to -1 if fishing is not possible here */
  
  
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
  

    // This returns the base texture.
	virtual Texture* currentTexture();
  
    // Return vector of all textures to be drawn, to be drawn from index 0 to n-1.
  virtual Vector <HasTexture*> * currentTextures();
  
    // Returns true is this tile has an object that can block line of sight. */
  bool hasViewBlocker();
    // Returns true is this tile has an object that can block movement.
  bool hasMovementBlocker();
  
  std::string getName();

	
};

#endif
