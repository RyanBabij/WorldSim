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
  bool hasFloor;
  
  int height;
  
  bool isUphill [8]; /* Clockwise starting north */
  
    // Vector of objects on this tile.
  Vector <WorldObject*> vObject;
  
  
  void addObject(WorldObject*);
  void removeObject(WorldObject*);
  
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
