#pragma once
#ifndef WORLDSIM_MYTHOLOGY_HPP
#define WORLDSIM_MYTHOLOGY_HPP

/* WorldSim: Mythology
	#include "Mythology.hpp"

	Container for mythology of a group of Characters.
	
	Each Civ may have a "State Mythology", however each Character can choose their own.
*/






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
#include "Mythology_Deity.hpp"
#include <Container/Table/TableInterface.hpp>

// changes should be made by gradual steps, like government in a Paradox game
// at least for now, the initial creation myth should be hardcoded. Only later details should mutate.
class Mythology: public TableInterface
{
	private:
	
	World* world;
	
	Vector <Mythology*> vChildMythology; // Mythologies derived from this one.
	
	Vector <Mythology_Deity*> vDeity;
	Vector <Mythology_Event*> vEvent; // List of events which happened in this Mythology.
	
	public:
	
	std::string name; // what the mythology calls itself
	
	enum mythology_type { MYTHOLOGY_NONE, MYTHOLOGY_SPIRITUAL, MYTHOLOGY_MONOTHEISTIC, MYTHOLOGY_POLYTHEISTIC, MYTHOLOGY_PAGAN };
	// the race which created the mythology. Other races can still follow it.
	enum mythology_race { mixed, dwarven, elven, human };
	mythology_type type; // The base type of mythology.
	mythology_race race; // The base type of mythology.

	
	// Name of the World. For now hardcoded.
	
	// Name of creator deity/deities. For now divided into limited sets.
	
	// Sub-deities - Maybe we can make this freeform.
	// Mythological stories - The deities and sub-deities interact.
	
	Mythology();
	
	// generate the base mythology
	void generateBase();
	
	void generateBaseDwarven();
	void generateBaseElven();
	void generateBaseHuman();
	
	void addDeity(std::string /* _name */, Mythology_Deity::mythology_deity_type /* _type */ );
	
	// do some stuff
	void increment();
	
	std::string getMythologyType();
	std::string getMythologyDescription();
	
	std::string getColumn(std::string _column) override;
	std::string getColumnType(std::string _column) override;
	 
	virtual std::string getLongDescription();
	
};

class Mythology_Manager
{
	public:
	
	Mythology mythologyDwarven;
	Mythology mythologyElven;
	Mythology mythologyHuman;
	
	Mythology_Deity deity1;
	Mythology_Deity deity2;
	
	Vector <Mythology*> vMythology;
	
	Mythology_Manager()
	{
	}
	
	void add(Mythology* _mythology)
	{
		vMythology.add(_mythology);
	}
	
	Mythology* get(int i)
	{
		if (vMythology.isSafe(i))
		{
			return vMythology(i);
		}
		return 0;
	}
	
	int size()
	{
		return vMythology.size();
	}
	
	void init()
	{
	}
};

#endif
