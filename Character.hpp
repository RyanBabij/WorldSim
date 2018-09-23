#pragma once
#ifndef WORLDSIM_CHARACTER_HPP
#define WORLDSIM_CHARACTER_HPP

/* WorldSim: Character
  #include "Character.hpp"
  
  Characters can be either NPCs or the player. They interact with the world.
  
  Characters may be either human, elven, or dwarven.
*/

#include "GuildCalendar.hpp"
#include <Container/Table/TableInterface.hpp>

#include "WorldObject.hpp"

class Tribe;

enum enumCauseOfDeath { UNKNOWN=0, STARVATION=1, MASSACRE=2, COMBAT=3, OLD_AGE=4 };
const std::string enumCauseOfDeathStr [5] = { "unknown", "starvation", "massacre", "combat", "old age" };

class Character: public WorldObject, public TableInterface, public SaveFileInterface
{
	public:
	
	std::string firstName;
	std::string lastName;
	std::string epithet;
	long unsigned int id;
	
	bool isMale;
	int age; /* In years. (or days?) */
	int daysCounter; /* 0-360 */
	int secondsCounter; /* 0 - 86,400 */
	
	int actionPoints; /* Base: 100. Points are deducted for each action. */
	
	bool isPregnant;
	int pregnantCounter;
	
	bool isAlive;
	bool isMarried; /* Change to partner pointer */
	
	int money; /* Gold coins or some shit. */
	
	int health;
	int hunger;
	
		/* Attributes 0-100 */
	int strength;
	int agility;
	int charisma;
	int intelligence;
	int perception;
	int endurance;
	int courage;
	
    // True if player has flagged this character as favourite.
  bool isFavourite;
  
		/* Right now each character has a biological mother and father. In the future we might also have guardians */
	Character* father;
	Character* mother;
	Character* spouse;
	Vector <Character*> vChildren;
	Vector <Character*> vSiblings;
	
    //People this character has killed.
  Vector <Character*> vKills;
  
	GuildDate dateOfBirth;
	GuildDate dateOfMarriage;
	GuildDate dateOfDeath;
	
	HasXY birthLocation;
	HasXY deathLocation;
	
	Tribe* tribe;
  
  enumCauseOfDeath causeOfDeath;
	
	
		/* INITIALIZATION */
	Character();
    // Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init( int _sex = 0);
	
		/* AI FUNCTIONS
		
		*/
	void incrementTicks(int = 1);
	
	Texture* currentTexture ();
	
	
		/* SOCIAL FUNCTIONS
			marry
			canMarry

			getDescendants
			Return all characters descended from this one.


		*/
	
	bool marry(Character* );
		/* Checks that the couple is not closely related, and several other conditions. */
	bool canMarry (Character*);
	
	Character* giveBirth();
	
	
		/* RECURSIVE ALGORITHM WHICH RETURNS ALL CHILDREN AND CHILDREN'S CHILDREN UNTIL NO CHILDREN REMAIN */
	Vector <Character*> * getDescendants(Vector <Character*> * vDescendants);
	
		/* RETURNS ALL DESCENDANTS OF ALL GRANDPARENTS. INCLUDES SELF. */
	Vector <Character*> * getRelatives();
	
	
	void die(enumCauseOfDeath = UNKNOWN);
	
	void starve();
	
		/* INFORMATION FUNCTIONS
		
		*/
	
		/* getBiography
		returns a string of text which provides a description of this character's name,
		appearance, life, achievements, etc.
		*/
	std::string getBiography();
	std::string getFullName() const;

	/* TABLE INTERFACE */
	std::string getColumn(std::string _column);
	std::string getColumnType(std::string _column);
  
  
  /* COMBAT FUNCTIONS */
  
    //Attack a character once.
  void attack (Character*);

	
};

#endif
