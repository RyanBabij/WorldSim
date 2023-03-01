#pragma once
#ifndef WORLDSIM_MYTHOLOGY_DEITY_HPP
#define WORLDSIM_MYTHOLOGY_DEITY_HPP

/* WorldSim: Mythology_Deity
	#include "Mythology_Deity.hpp"

	Deities are immortal characters who can peform very powerful actions.
*/

class World;


// The deities of the world (non-mortal characters)
class Mythology_Deity
{
	public:
	// The balance of these 2 variables should be enough.
	int benevolence;
	int malevolence;
	int sanity;
	
	// The type of deity. Shouldn't need to be too complex for now.
	enum mythology_deity_type { benevolent, evil, chaotic };
	enum mythology_deity_subtype { creator, alien, observer };
	
	mythology_deity_type type;
	mythology_deity_subtype subtype;
	
	Vector <Character*> vObservedCharacters;
	
	World* world;
	
	
	Mythology_Deity();
	

	
	// Deity can access the world and act in it.
	// Generally this should take the form of "favouring" somebody, which could turn them into a saint
	
	// What actions could deities make?
	// * Favour an individual - Give them items, stat increase, give them knowledge, make them a saint/prophet.
	void act();
	
	void createRace(enumRace /* _race */ );
};

#endif
