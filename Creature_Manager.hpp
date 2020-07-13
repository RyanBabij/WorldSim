#pragma once
#ifndef WORLDSIM_CREATURE_MANAGER_HPP
#define WORLDSIM_CREATURE_MANAGER_HPP

/* WorldSim: CreatureManager
  #include "CreatureManager.hpp" */

/**

	Container and generator for Creature types.


	Plan:
	Come up with a bunch of creature types and then add the adjectives simliar to the Flora system.
	
	Creatures will be less common than Flora and other objects and therefore won't need their data abstracted too much.
	
	In future some creatures will be subterranean, nocturnal, etc.
	
	Creatures will probably get random stats per type, for example speed, atk, etc. Should create some decent variety.
*/

class Creature_Attribute
{
	public:
	std::string name;
	int size;
	
	// things this attribute can do
	// value?
};

class Creature;

// Store all data common to a type of Creature.
class Creature_Species
{
	public:
	
	std::string name; // name of creature species, for example "Brown Elephant".
	
	int size; // size rating
	
	// attacks
	// bodyparts
	// attributes
	
	Creature_Species()
	{
	}
	
	
	
	
};

class CreatureManager
{
	public:
	
	Vector <Creature*> vCreatureType;
	
	CreatureManager()
	{
	}
	
	
	void generate( int amount )
	{
		
		
		
	}
	
	// Generate random Creature type
	Creature * get()
	{
	}
};

#endif
