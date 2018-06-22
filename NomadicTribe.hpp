#pragma once
#ifndef WORLDSIM_NOMADIC_TRIBE_HPP
#define WORLDSIM_NOMADIC_TRIBE_HPP

/* NomadicTribe.hpp
	#include "NomadicTribe.hpp"

	Project: WorldSim
	Created: 7/10/2017, 0271994297.
	Updated: 7/10/2017, 0271994297.

	Description:
	A nomadic tribe is a global unit which wanders the world map randomly, gathering resources from the tile it is on. When it grows large enough, it becomes a settlement.

	Notes:

	0271994297 - Created header.
	
Name: The X Tribe.

X can be a random animal noun to start.
*/


#include "WorldObjectGlobal.hpp"

class NomadicTribe: public WorldObjectGlobal
{
	public:
	NomadicTribe();

		// Stores details of all members of the tribe.
	Vector <Soldier*> vNomad;

	void generateNomads(int nNomads);
	
		// IF TRUE, THEN THE ARMY IS CURRENTLY FIGHTING, AND MAY NOT BE ABLE TO MOVE FREELY.
	bool inCombat;
	
	Vector <Citizen*> vCitizen;
	
		// WALK IN A RANDOM DIRECTION.
	void wander();
	
		/* SIMULATE X TURNS OF THE ARMY. */
	void incrementTicks(int);
	
		// RETURN TRUE IF THE TRIBE CAN REACH THIS TILE.
	bool canAccess(int _x, int _y);
	
		/* RETURN THE OBJECTIVE WITH THE HIGHEST PRIORITY. RETURN 0 FOR NO OBJECTIVES. */
	Objective* getHighestObjective();
	
	Texture* currentTexture();
	
};

#endif
