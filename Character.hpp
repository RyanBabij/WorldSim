#pragma once
#ifndef WORLDSIM_CHARACTER_HPP
#define WORLDSIM_CHARACTER_HPP

/* WorldSim: Character
	#include "Character.hpp"

	Characters can be either NPCs or the player. They interact with the world.

	Characters may be either human, elven, or dwarven.
*/

#include "Social.hpp"
#include "Idea.cpp"


#include "Character_Skill.cpp" // Learned skills
#include "Character_Attribute.cpp" // Base skills like Strength

#include <Game/Calendar/Calendar.hpp>
#include <Container/Table/TableInterface.hpp>

#include "WorldObject.hpp"
#include "WorldObject_Tree.hpp"
#include "Item.hpp"

#include "HasMoney.cpp"

class Job;
class Tribe;
class Item;
class Civ;
class Settlement;
class Character_Knowledge;
class World_Local;

class Character: public WorldObject, public TableInterface, public SaveFileInterface, public HasMoney
{
	public:

	std::string firstName;
	std::string lastName;
	std::string epithet;
	long unsigned int id;

	bool isMale;
	int age; /* In years. (or days?) */
	int daysCounter; /* 0-DAYS_PER_YEAR */
	int tickCounter; /* 0 - TICKS_PER_DAY */

	int actionPoints; /* Base: 100. Points are deducted for each action. */

	bool isPregnant;
	int pregnantCounter;

	bool isAlive;
	bool isMarried; /* Change to partner pointer */

	//bool isUnderground; /* Is on the subterranean layer. */

	short int maxHealth;
	short int health;
	short int hunger; /* 500 = MAX_HUNGER */
	short int thirst;

	// Base skills 0-10.
	AttributeManager baseSkill;

	/* Skills */

	unsigned char skillFishing;
	unsigned char skillMarksmanship;
	unsigned char skillMining;
	unsigned char skillFarming;
	// skillMetalSmithing modifies chances of making high-quality items. Each level improves chances by 1%
	//unsigned char skillMetalsmithing;
	Skill skillMetalsmithing;
	
	void levelUpMetalsmithing(unsigned char /* _amount */ );

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

	Calendar dateOfBirth;
	Calendar dateOfMarriage;
	Calendar dateOfDeath;

	HasXY birthLocation;
	HasXY deathLocation;

	Tribe* tribe;
	Civ* civ;
	Settlement* settlement;

	//int worldX, worldY;

	enumCauseOfDeath causeOfDeath;

	// All loaded Characters can get a link to their world.
	World_Local* map;

	//Character's knowledge of the world (optional).
	Character_Knowledge* knowledge;

	//Manage social connections
	Social social;
	
	// Any potential ideas the person has learned
	Vector <Idea> vIdea;
	
	//Ideas the Character has originally invented.
	Vector <Idea> vOriginalIdea;
	// Any particularly interesting Ideas this Character invented
	Vector <SpecialIdea> vOriginalSpecialIdea;
	
	// Events relevant to this character
	Vector <Event*> vEvent;
	

	/* INITIALIZATION */
	Character();
	// Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init( int _sex = 0);
	
	/* GETTER AND SETTER */
	
	char getCharisma();
	void setCharisma(char amount);
	
	char getIntelligence();
	void setIntelligence(char amount);
	
	char getStrength();
	void setStrength(char amount);
	
	char getBaseSkill(AttributeManager::TYPE skill);
	void setBaseSkill(AttributeManager::TYPE skill, char value);
	
	void skillUpFarming();
	void skillUpMining();
	
	Vector <Character*> getAllKnownCharacters();
	
	bool hasIdea(Idea idea);
	void giveIdea(Idea idea);
	

	
	// INTERACTION
	
	void shareIdeas(Character* c); // Share 1 new idea per call.
	
	/* UPDATE FUNCTIONS */
	
	void updateSocial();

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
  void takeItem(Item* _item);
    // food or potion can be consumed.
  void consume(Item* _item);
  
  void removeFromInventoryGrid(Item* _item); /* Player-only function */
  
  bool hasItemType(ItemType);
  
  Item* getBestFarmingEquipment();
  //Item* getBestItemFor(enumJob job);
  Item* getBestItemFor(Job job);

	
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
	std::string getBestSkills();
  
    // LOCATION
    
    // Returns true if the Character is at this world map coordinate.
  bool isOnMap(int /* _mapX */, int /* _mapY */);
  
  
    // KNOWLEDGE
    
    //returns true if the Character has seen this tile.
  char hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */, bool isSubterranean = false );
  
  
  // Debug functions
	// reveal all of current map
  void revealMap(unsigned int _range);
  
  // main AI manager, defines current AI state based on situation
  void aiManager();
  
  // AI STATES
  // HUNTING
  // CRAFTING
  // COMBAT
  // CULTURE
  
  // aiCrafting() - Has chance of creating advancement points
  // aiCulture() - has chance of creating advancement points
  // aiHunting()
  
  
  
	/* TABLE INTERFACE */
	std::string getColumn(std::string _column);
	std::string getColumnType(std::string _column);
  
  
  /* COMBAT FUNCTIONS */
  
    //Attack a character once.
  void attack (Character*);
  
    //Get attacked by a Creature
  void getAttacked (Creature*, Creature_Attack*);
  
    //Ensure the character's knowledge object exists.
  void initialiseKnowledge();
  
    //Update knowledge with current instance.
  void updateKnowledge();
  // Extra processing available
  bool updateKnowledgeIdle();
  
  
  
	// ABSTRACT FUNCTIONS
  bool abstractResearchMonth();
	// Characters will learn about each other and spread Ideas.
	void abstractSocial(Character*);

	
};

#endif
