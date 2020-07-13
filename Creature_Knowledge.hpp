#pragma once
#ifndef WORLDSIM_CREATURE_KNOWLEDGE_HPP
#define WORLDSIM_CREATURE_KNOWLEDGE_HPP

/* WorldSim: Creature_Knowledge
  #include "Creature_Knowledge.hpp"
  
  Creatures will be able to have some limited understanding of the world when they are loaded in.
  However it will be more limited compared to Characters.
*/

#include <Interface/HasXY.hpp>

#include "Pathing.hpp"

class Creature_Knowledge
{
	public:
	Pathing_Local p;

	unsigned int pathIndex;



	// Vector of places this Character has visited. I think most characters will only visit a handful of maps.
	//Vector <World_Local*> vMapsVisited;
	//Vector < ArrayS2 <char>* > vaTileVisited;

	Vector <HasXY*> vVisitedTiles; /* List of coordinates the Creature has stood on. */

	Vector <LocalTile*> vVisibleTiles;

	HasXY currentGoal; /* Creatures can wander by moving toward a particular waypoint */
	HasXY threatLocation; /* Creature should avoid this location */

	/* INITIALIZATION */
	Creature_Knowledge();
	void init();


	// Add this tile into the Character's knowledge (if it isn't already)
	void addTile( World_Local* /* _map */, int /* _x */, int /* _y */ );

	//returns true if the Character has seen this tile. 0 = no. 1 = has seen. 2 = currently visible.
	char hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */ );



	void updateThreat(short int threatX, short int threatY);

	// Sets all 2 to 1 for LOS update.
	void updateLOS();
	// Forget all knowledge
	void clear();

	// Return next step from path. NESW. 0 if no path.
	char nextStep();

	//returns true if the Character can currently see this tile.
	//bool canSee( World_Local* /* _map */, int /* _x */, int /* _y */ );
};

#endif
