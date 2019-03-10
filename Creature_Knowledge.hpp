#pragma once
#ifndef WORLDSIM_CREATURE_KNOWLEDGE_HPP
#define WORLDSIM_CREATURE_KNOWLEDGE_HPP

/* WorldSim: Creature_Knowledge
  #include "Creature_Knowledge.hpp"
  
  Creatures will be able to have some limited understanding of the world when they are loaded in.
*/

class Creature_Knowledge
{
	public:
  
    // Vector of places this Character has visited. I think most characters will only visit a handful of maps.
  //Vector <World_Local*> vMapsVisited;
  //Vector < ArrayS2 <char>* > vaTileVisited;
  
  Vector <LocalTile*>* vVisibleTiles;

		/* INITIALIZATION */
	Creature_Knowledge();
	void init();

  
    // Add this tile into the Character's knowledge (if it isn't already)
  void addTile( World_Local* /* _map */, int /* _x */, int /* _y */ );
  
    //returns true if the Character has seen this tile. 0 = no. 1 = has seen. 2 = currently visible.
  char hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */ );
  
  
  // Sets all 2 to 1 for LOS update.
  void updateLOS();
	
    //returns true if the Character can currently see this tile.
  //bool canSee( World_Local* /* _map */, int /* _x */, int /* _y */ );
};

#endif