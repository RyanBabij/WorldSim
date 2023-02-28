#pragma once
#ifndef WORLDSIM_MYTHOLOGY_HPP
#define WORLDSIM_MYTHOLOGY_HPP

/* WorldSim: Mythology
	#include "Mythology.hpp"

	Container for mythology of a group of Characters.
	
	Each Civ may have a "State Mythology", however each Character can choose their own.
*/



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
	
	
	Mythology_Deity()
	{
	}
	
	Vector <Character*> vObservedCharacters;
	
	// Deity can access the world and act in it.
	// Generally this should take the form of "favouring" somebody, which could turn them into a saint
	
	// What actions could deities make?
	// * Favour an individual - Give them items, stat increase, give them knowledge, make them a saint/prophet.
	void act()
	{
	}
	
	void createRace(enumRace _race)
	{
		
		//void World::generateTribes( int nTribesHuman = DEFAULT_NUMBER_TRIBES_HUMAN, int nTribesDwarven = DEFAULT_NUMBER_TRIBES_DWARVEN, int nTribesElven = DEFAULT_NUMBER_TRIBES_ELVEN)
		
		//world.generateTribes(0,1,0);
		
		
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
};

// The saints of the world (notable mortal characters). Maybe prophet as well.
// It is not required that the saint/prophet be genuine. False prophets can be included.
class Mythology_Saint
{
	public:
};

// should inherit event class (for legends interface)
// Types of events:

// World created - exceptional event of course
// create race - create one of the races
// Artifact created - deity made a special artifact
// Commandment - deity gave a commandment to its people (aka a law they must follow)
// Terrain modification
// Disaster or other large scale event
// Interaction between multiple deities
// Tech provided
// Character posessed by a deity
// Structure created
// Beast created

class Mythology_Event
{
	public:
	
	std::string date; // date of event.
	std::string type; // category of event, for example creation. (should be enum)
	std::string description; // description of event for legends menu

};


class World;

// changes should be made by gradual steps, like government in a Paradox game
// at least for now, the initial creation myth should be hardcoded. Only later details should mutate.
class Mythology
{
	private:
	
	World* world;
	
	std::string name; // what the mythology calls itself
	
	Vector <Mythology*> vChildMythology; // Mythologies derived from this one.
	
	Vector <Mythology_Deity*> vDeity;
	Vector <Mythology_Event*> vEvent; // List of events which happened in this Mythology.
	
	public:
	
	enum mythology_type { none, spiritual, monotheistic, polytheistic, pagan };
	// the race which created the mythology. Other races can still follow it.
	enum mythology_race { mixed, dwarven, elven, human };
	mythology_type type; // The base type of mythology.

	
	// Name of the World. For now hardcoded.
	
	// Name of creator deity/deities. For now divided into limited sets.
	
	// Sub-deities - Maybe we can make this freeform.
	// Mythological stories - The deities and sub-deities interact.
	
	Mythology();
	
	void addDeity(std::string /* _name */, Mythology_Deity::mythology_deity_type /* _type */ );
	
	// do some stuff
	void increment();
	
};

#endif
