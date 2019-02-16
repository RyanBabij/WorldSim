#pragma once
#ifndef WORLDSIM_CHARACTER_KNOWLEDGE_HPP
#define WORLDSIM_CHARACTER_KNOWLEDGE_HPP

/* WorldSim: Character_Knowledge
  #include "Character_Knowledge.hpp"
  
  Characters will use a knowledge object to store their understanding of the world.
  Currently this will consist of explored tiles for line of sight and fog of war.
*/

class Character_Knowledge
{
	public:
  
    // Vector of places this Character has visited. I think most characters will only visit a handful of maps.
  Vector <World_Local*> vMapsVisited;
  Vector < ArrayS2 <bool>* > vaTileVisited;
  

		/* INITIALIZATION */
	Character_Knowledge();
    // Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init();

  
    // Add this tile into the Character's knowledge (if it isn't already)
  void addTile( World_Local* /* _map */, int /* _x */, int /* _y */ );
  
    //returns true if the Character has seen this tile.
  bool hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */ );
	
};

#endif
