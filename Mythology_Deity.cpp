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



#endif
