#pragma once
#ifndef GUILD_WORLDTILE_HPP
#define GUILD_WORLDTILE_HPP

/*
	WorldTile is how tile data is stored on the strategic level. Stuff like fertility, move penalty, etc.
*/

/* Basic details like heightmap are stored as a seed, to cut down on memory footprint. Tile can be constructed into an array on demand. Modifications to the world are stored seperately.

A tile is 5km by 5km, ie, 5,000 * 5000 tiles. */

// In the future we might have polymorphic classes to distinguish simulated tiles from abstract tiles. However for now it's not a major concern as the world map should need less than 100MB of RAM.

#include <Interface/HasTexture.hpp>

class WorldTile: public HasTexture
{
	public:
	
	WorldTile();
	
	int seed; /* Seed to feed into RNG to deconstruct into a map. */
  
    // How many metals may be mined from this tile.
  int baseMetal;
	
	// texture pointer
	
	enumBiome biome; /* Determines what it looks like and is called */
	int baseMoveCost; /* how many ap to move onto the tile. */
	bool canHaveSettlement;
	int baseFertility;
	bool canMove; /* True if units can walk over it. */
	int baseLogisticsCost; /* Will consume logistics from armies and navies. Used to prevent armies from travelling through desert, and early navies from travelling through ocean. */
	int defensiveBonus;
	
		// The id of the landmass.
	int landID;
		// The id of the biome.
  int biomeID;
  
  void init(enumBiome _biomeID);
	
		// This returns the base texture.
	virtual Texture* currentTexture();
	
		// This returns an additional texture to overlay on the base texture.
	Texture* getImprovementTexture();
	
};

#endif
