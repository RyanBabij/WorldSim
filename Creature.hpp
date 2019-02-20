#pragma once
#ifndef WORLDSIM_CREATURE_HPP
#define WORLDSIM_CREATURE_HPP

/* WorldSim: Creature
  #include "Creature.hpp"
  
  Creatures are non-civilised actors. They are no members of Tribes of Civilisations, and don't have a race.
  For now they are randomly spawned in. In the future I'd like them to have realistic behaviour,
  for example travelling in groups and looking for food.
  
*/

#include "GuildCalendar.hpp"
#include <Container/Table/TableInterface.hpp>

#include "WorldObject.hpp"

class World_Local;

class Creature: public WorldObject, public TableInterface
{
	public:

	bool isMale;
	int age; /* In years. (or days?) */
	int daysCounter; /* 0-360 */
	int secondsCounter; /* 0 - 86,400 */
	
	int actionPoints; /* Base: 100. Points are deducted for each action. */

	bool isAlive;
	
	int health;
	int hunger;
	

		// INITIALIZATION
	Creature();
    // Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init( int _sex = 0);
	
		/* AI FUNCTIONS
		
		*/
	void incrementTicks(int = 1);
	
	Texture* currentTexture ();
	
	
	void die();
	

    // KNOWLEDGE
    
    //returns true if the Creature has seen this tile.
  bool hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */ );
  
  
	/* TABLE INTERFACE */
  
	std::string getColumn(std::string _column);
	std::string getColumnType(std::string _column);
  
  
  /* COMBAT FUNCTIONS */
  
    //Attack a Creature once.
  //void attack (Creature*);
  

	
};

#endif
