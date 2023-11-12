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
	
	Vector <Mythology*> vChildMythology; // Mythologies derived from this one.
	
	Vector <Mythology_Deity*> vDeity;
	Vector <Mythology_Event*> vEvent; // List of events which happened in this Mythology.
	
	protected:
	
		World* world;
	
	public:
	
	std::string name; // what the mythology calls itself
	
	enum mythology_type { MYTHOLOGY_NONE, MYTHOLOGY_SPIRITUAL, MYTHOLOGY_MONOTHEISTIC, MYTHOLOGY_POLYTHEISTIC, MYTHOLOGY_PAGAN };
	mythology_type type; // The base type of mythology.
	// the race which created the mythology. Other races can still follow it.
	enumRace race; // The base type of mythology.

	
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
	
	void addDeity(std::string /* _name */, Mythology_Deity::PERSONALITY /* _type */ );
	
	// do some stuff
	void increment();
	
	std::string getType();
	std::string getDescription();
	
	std::string getColumn(std::string _column) override;
	std::string getColumnType(std::string _column) override;
	 
	virtual std::string getLongDescription();
	
};

class Mythology_Manager
{
	public:
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
Mythology_Manager mythologyManager;

// Dwarven and Elven mythology are to be mostly hardcoded and not too subject to change.

class Mythology_Dwarven: public Mythology
{
	public:
	
	Mythology_Dwarven()
	{
		type = Mythology::MYTHOLOGY_MONOTHEISTIC;
		race = DWARVEN;
		name = "Dwarven mythology";
		
		Mythology_Deity* deity = new Mythology_Deity();
		deity->name = globalNameGen.generate();
	}
	
	std::string getLongDescription()
	{
		std::string description = "The Dwarven religion is monotheistic, believing in a single Architect God who "
		"designed the universe according to mathematical laws. The Architect generally does not concern itself with "
		"human affairs, but may sometimes provide assistance to a particularly gifted individual in the form of madness. "
		"The main goal of their religion is to elevate themselves beyond their mortality, using knowledge and technology "
		"to do so.";
		return description;
	}
};

Mythology_Dwarven mythologyDwarven;

#endif

