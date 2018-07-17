#pragma once
#ifndef WORLDSIM_CHARACTER_HPP
#define WORLDSIM_CHARACTER_HPP

// class City;
// class Civ;
// class Task;
// class Stackable;

// #include "GuildCalendar.hpp"

// #include "Job.hpp"
// #include "Job_Farmer.hpp"

#include "GuildCalendar.hpp"
#include <Container/Table/TableInterface.hpp>

class Tribe;

enum enumCauseOfDeath { UNKNOWN=0, STARVATION=1, MASSACRE=2, COMBAT=3, OLD_AGE=4 };
const std::string enumCauseOfDeathStr [5] = { "unknown", "starvation", "massacre", "combat", "old age" };

class Character: public WorldObject, public TableInterface
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
	void init();
	
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
	
	//bool hasJob;
	
	
	// bool isKing;
	// bool isGuardCaptain;
	// bool isSmith;
	
	
	// int fatigue; /* 0 = full energy. 100 = unconscious. */
	
	// bool isAwake; /* False if the person is sleeping. */
	
	// Job* currentJob;
	
	
	//Vector <Task*> vTask; /* Vector of tasks the citizen wants to complete. */
	//TaskManager taskManager; /* Container for tasks. */
	
	
	// Vector <Citizen*> vChildren;
	// Citizen* father;
	// Citizen* mother;
	// Citizen* partner;
	
	// Vector <Building*> vBuilding; /* Owned land and property. */
	
	// Vector <WorldObject*> vPossession; /* List of all objects that this person owns. */
	
	// /* NOTE: Vector of recently purchased goods might go here. */
	
	// City* city;
	// Civ* civ;
	
	// GuildDate birthDate;
	
	// /* The citizen needs a link to the world to know what's going on around him. */
	// World* world;
	
	// /* 2 objects may be carried/wielded.  2handed tag? */
	// WorldObject* carry1;
	// WorldObject* carry2;
	
	// Vector <WorldObject*> vInventory; /* This is a container of everything the citizen is carrying (except carry1 and carry2?). There are no backpacks or bags in guild. */
	// Vector <Stackable*> vInventoryStackable;
	// //Vector <int> vInventoryStack; 
	
	
	/* Clothing/Armour */
	

	
	//std::string getFullName();
	//std::string getSexString();
	
	//Citizen* lookForPartner(); /* Return 0 if no partner found. */
	
	
	//Citizen* makeBaby();
	
	//std::string getJobName();
	
	//void logicTick();
	//void aiSecond(); /* 1 in-game second AI algorithm. */
	//void aiDay(); /* Do a 24 hour AI algorithm. */

	
	/* Calendar functions. */
		// bool isDayTime(); /* Return true means it is daytime. Return false means it is nighttime. */
			// bool isNightTime() { return !isDayTime(); }
	
	// void updateAge();
	
	// void die();
	
	// void sleep(); /* Biology for sleep. */
	
	// void marry(Citizen*);
	
	// std::string getBiography(); /* Print out some flavour. */
	
	// bool canMarry(Citizen*); /* Make sure the citizens aren't too closely related. */
	
	// void setPregnant(); /* Makes a woman pregnant. */
	
	// Texture* currentTexture ();
	
	// bool isEmployed();
	// bool hasHome(); /* Check if the citizen has a place to crash. */
	// bool hasBuildingType(std::string); /* Check if the citizen has a particular building type. */
	// Building* getHome(); /* Return a pointer to the citizen's home, or 0 if the citizen is homeless. */
	// Building* getBuildingType(const std::string _buildingType); /* Return ptr to building of this type. Return 0 if not possible. */
	
	// bool walkHome(); /* Walk home. */
	// bool isHome(); /* If the citizen is inside a house that he owns. */
	

	// bool liesWithin(const int _x1, const int _y1, const int _x2, const int _y2); /* returns true if citizen lies within coordinates (inclusive). */
		// bool liesWithin(Building*); /* Alias for above. */
	
	
	// /* Movement. */
	
	// void wander(); /* Move in a random direction. */
	// bool move(int directionX, int directionY); /* Move in the given direction. Return false if this is not possible. */
	// bool moveTowards (WorldObject*); /* Move towards the object. Return false if this is not possible. */
		// bool moveTo (WorldObject* _object) { return moveTowards(_object); } /* Alias. */
	// bool moveTo(const int _x, const int _y); /* Move to these coords. */
	// bool moveTo(Building*);
	
	
	// bool moveTowards (Building*); /* Move towards the building. Return false if this is not possible. */
	
	// bool teleport(int /* x */, int /* y */); /* Teleport the citizen to this location. Return false if this is not possible. */
	

	
	// Vector <WorldObject*> * getVisibleObjects();
	// Vector <WorldObject*> * getObjectsInRange(const int RANGE); /* Return a vector of all objects within RANGE tiles from citizen. */
	// WorldObject* lookForObjectType(std::string);
	// //Building* findBuildingType ( std::string _buildingType );
	
	// bool isStandingOn(WorldObject*); /* Return true if the citizen is at the same coords as the passed object. */
		// bool standingOn(WorldObject* _obj) { return isStandingOn(_obj); } /* Alias of above. */
	
	// std::string getName();
	
	// void printDebug(std::string _message); /* Print a debug message prefixed with citizen name and coords. */
	
	// bool pickUp(WorldObject* object); /* Pick up an object. Return false if not possible. */
	// bool isCarrying(WorldObject* object);
	// bool drop(WorldObject* object); /* Drop an object if it is being carried. Return false if this is not possible. */
	
	// bool dropStack(Stackable* /* Stack */, int /* amount */); /* Return false if amount is too high, or not possible. */
		// bool dropStack(std::string name, int amount);
	
	// bool putInInventory(WorldObject* /* Object */); /* Put object into inventory array (vInventory). Return false if not possible due to encumberance limit or something. */
	// bool addStackable(Stackable* /* Stackable object. */); /* Add this stack of objects to the citizen's stack of this object. Or create a stack of this object if the citizen doesn't have one. */
	
	// bool moveToAndPickUp(WorldObject*); /* Walk to the object and then pick it up. Return true when complete. */
	
	
	// void goToBed(); /* Find a bed and sleep in it. */
	
	
	// bool canAfford(int /* Amount */); /* Return true if the citizen has at least <Amount> money. */
	
		// /* Trade. */
	// int offerMoney( int /* amount */ ); /* Returns the amount passed if the citizen can afford it, otherwise will return the most that the citizen can afford. */
		
	// int offerPriceFor(WorldObject*); /* Return the amount of gold that the citizen is willing to buy the item for. */
	// bool willBuy(WorldObject*); /* Return true if the citizen would be willing to buy the passed object. Otherwise return false. */
	// int sellItemTo(WorldObject*, Citizen*); /* Changes possession, returns amount paid for item. */
	// WorldObject* buyItemFrom(Citizen*, std::string /* item name */, int /* Gold Amount */); /* Changes possession. Returns purchased object.  Returns null pointer if transaction failed.*/
	
	// //int willSell(WorldObject*, int /* amount */); /* Return true
	// int getPriceToBuy(WorldObject*, int /* amount */); /* Return the total cost in gold to purchase <amount> of <WorldObject*>. */
	// int getPriceToBuy(std::string /* item name */, int /* amount */); /* Return the total cost in gold to purchase <amount> of <WorldObject*>. */
	// int getStock(WorldObject*); /* Return amount of stock the person has of this item type. */
	// int getStock(std::string /* item name */); /* Return amount of stock the person has of this item type. */
	
	// WorldObject* giveStock(std::string /* item name */); /* Return a pointer to an item with that name. Return 0 for fail. */
	// WorldObject* giveStock(WorldObject*); /* Return a pointer to an item with that name. Return 0 for fail. */
	
		// /* Possession. */
	// bool gainPossession(WorldObject*); /* Add this object to the citizen's list of possessions. */
	// bool losePossession(WorldObject*); /* Remove this object from the citizen's list of possessions. */
	// bool transferPossessionTo(WorldObject*, Citizen*); /* Remove this object from the current citizen's possession, and give it to the passed citizen. */
	
	// bool removeFromStock(WorldObject* _item);
	
};

#endif
