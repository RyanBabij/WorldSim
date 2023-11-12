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
	
	enum PERSONALITY { PHYSICAL, MYSTERIOUS, BUILDER, TRICKSTER };
	enum ALIGNMENT { GOOD, FLAWED, EVIL };
	
	// A god can have a certain number of people it looks after.
	Vector <Character*> vObservedCharacters;
	
	World* world;
	
	Mythology_Deity();
	
	// Deity can access the world and act in it.
	// Generally this should take the form of "favouring" somebody, which could turn them into a saint
	
	// What actions could deities make?
	// * Favour an individual - Give them items, stat increase, give them knowledge, make them a saint/prophet.
	void act();

};

#endif
