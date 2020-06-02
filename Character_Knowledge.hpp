#pragma once
#ifndef WORLDSIM_CHARACTER_KNOWLEDGE_HPP
#define WORLDSIM_CHARACTER_KNOWLEDGE_HPP

/* WorldSim: Character_Knowledge
  #include "Character_Knowledge.hpp"
  
  Characters will use a knowledge object to store their understanding of the world.
  Currently this will consist of explored tiles for line of sight and fog of war.
*/

#include "Pathing.hpp"

class Character_Knowledge
{
	public:
  Pathing_Local p;
  
    // Vector of places this Character has visited. I think most characters will only visit a handful of maps.
  Vector <World_Local*> vMapsVisited;
  Vector < ArrayS2 <char>* > vaTileVisited;
  
    // Same but for subterranean tiles
  Vector <World_Local*> vSubMapsVisited;
  Vector < ArrayS2 <char>* > vaSubTileVisited;
  
  Vector <char> vPath; // current path
  
  HasXY currentGoal; /* Characters can wander by moving toward a particular waypoint */
  

		/* INITIALIZATION */
	Character_Knowledge();
    // Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init();

  
    // Add this tile into the Character's knowledge (if it isn't already)
  void addTile( World_Local* /* _map */, int /* _x */, int /* _y */, bool isSubterranean = false);
  
  // Adds the tile to the Character's knowledge.
  // Overloaded to allow absolute coordinates.
  void addTile( unsigned long int /* _x */, unsigned long int /* _y */, bool isSubterranean = false);
  
    //returns true if the Character has seen this tile. 0 = no. 1 = has seen. 2 = currently visible.
  char hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */, bool isSubterranean = false);
  
  
  // Sets all 2 to 1 for LOS update.
  void updateLOS();
	
    //returns true if the Character can currently see this tile.
  //bool canSee( World_Local* /* _map */, int /* _x */, int /* _y */ );
};

#endif
