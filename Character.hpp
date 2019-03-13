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
#include "WorldObject_Tree.hpp"
#include "Item.hpp"

class Tribe;
class Item;

class Character_Knowledge;
class World_Local;

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
	int hunger; /* 500 = MAX_HUNGER */
	
		/* Governing attributes 0-100  (Might remove later) */
	int strength;
	int agility;
	int charisma;
	int intelligence;
	int perception;
	int endurance;
	int courage;
  
    /* Skills */
    
  unsigned char skillFishing;
  unsigned char skillMarksmanship;
	
    // True if player has flagged this character as favourite.
  bool isFavourite;
  
  
  char idleCounter;
  Vector <HasXY2 <unsigned long int> * > vMovesToProcess;
  Vector <HasXY2 <unsigned long int> * > vMovesToProcessSneak;
  
  Vector <Item*> vInventory;
  
  //INTERACTION
    int nPelt;
    int nMeat;
  
  
  bool isSneaking;
  
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
  
  //int worldX, worldY;
  
  enumCauseOfDeath causeOfDeath;
  
  // All loaded Characters can get a link to their world.
  World_Local* map;
  
    //Character's knowledge of the world (optional).
  Character_Knowledge* knowledge;
	
	
		/* INITIALIZATION */
	Character();
    // Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init( int _sex = 0);
	
		/* AI FUNCTIONS
		
		*/
	void incrementTicks(int = 1);
  
  // MOVE FUNCTIONS
  virtual void wander();
	
	Texture* currentTexture ();
  
  
  /* ITEM FUNCTIONS */
  
    // For consumables or crafting/alchemy.
  void useItem(Item* item)
  {
  }
  
    // For now this should just drop item at player's feet because
    // Inventory is not fully implemented.
  void giveItem(Item* _item);
    // food or potion can be consumed.
  void consume(Item* _item);
  
  void removeFromInventoryGrid(Item* _item); /* Player-only function */

	
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
	std::string getName();

  
    // LOCATION
    
    // Returns true if the Character is at this world map coordinate.
  bool isOnMap(int /* _mapX */, int /* _mapY */);
  
  
    // KNOWLEDGE
    
    //returns true if the Character has seen this tile.
  char hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */ );
  
  
	/* TABLE INTERFACE */
	std::string getColumn(std::string _column);
	std::string getColumnType(std::string _column);
  
  
  /* COMBAT FUNCTIONS */
  
    //Attack a character once.
  void attack (Character*);
  
  
    //Ensure the character's knowledge object exists.
  void initialiseKnowledge();
  
    //Update knowledge with current instance.
  void updateKnowledge();
  // Extra processing available
  void updateKnowledgeIdle();

	
};

#endif
