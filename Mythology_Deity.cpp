#pragma once
#ifndef WORLDSIM_MYTHOLOGY_DEITY_CPP
#define WORLDSIM_MYTHOLOGY_DEITY_CPP

/* WorldSim: Mythology_Deity
	#include "Mythology_Deity.cpp"

	Deities are immortal characters who can peform very powerful actions.
*/

#include "Mythology_Deity.hpp"

class World;



Mythology_Deity::Mythology_Deity()
{
	world=0;
}


// Deity can access the world and act in it.
// Generally this should take the form of "favouring" somebody, which could turn them into a saint

// What actions could deities make?
// * Favour an individual - Give them items, stat increase, give them knowledge, make them a saint/prophet.
void Mythology_Deity::act()
{
}

void Mythology_Deity::createRace(enumRace _race)
{
	if (world==0)
	{
		return;
	}
	
	//void World::generateTribes( int nTribesHuman = DEFAULT_NUMBER_TRIBES_HUMAN, int nTribesDwarven = DEFAULT_NUMBER_TRIBES_DWARVEN, int nTribesElven = DEFAULT_NUMBER_TRIBES_ELVEN)
	
	world->generateTribes(0,1,0);
	
	
	if (_race == NONE)
	{
	}
	else if (_race == DWARVEN)
	{
	}
	else if (_race == ELVEN)
	{
	}
	else if (_race == HUMAN)
	{
	}
	
}


#endif
