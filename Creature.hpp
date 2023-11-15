#pragma once
#ifndef WORLDSIM_CREATURE_HPP
#define WORLDSIM_CREATURE_HPP

/* WorldSim: Creature
  #include "Creature.hpp"
  
  Creatures are non-civilised actors. They are no members of Tribes or Civilisations, and don't have a race.
  For now they are randomly spawned in. In the future I'd like them to have realistic behaviour,
  for example travelling in groups and looking for food.
  
  Creatures may be herbivores or carnivores. Each creature type may have its own AI package.
  
  Creatures are usually simulated using the abstract data layer.
  
  Creature should refer to the base type. Creature_Implementation should refer to infividual Creature instances.
  This also provides the opportunity to later divide Creature instances into Creature_Abstract and fully detailed
  instances.
  
*/

#include <Game/Calendar/Calendar.hpp>
#include <Container/Table/TableInterface.hpp>

#include "WorldObject.hpp"
#include "Creature_Knowledge.hpp"

class World_Local;

#include "Creature_Attack.hpp"

#include "Creature_Species.hpp"

class Creature: public WorldObject, public TableInterface
{
	public:

	bool isMale;
	int age; /* In ticks */
	int daysCounter; /* 0-360 */
	int secondsCounter; /* 0 - 86,400 */
  
  bool isCarnivore; /* Creature can eat other animals. Otherwise the creature is a herbivore */
  /* Herbivores are reliant on the grass and plants on their map */
  // Home biome - Biome the creature is evolved for.

	
	int actionPoints; /* Base: 100. Points are deducted for each action. */

	bool isAlive;
	
	int health;
  
    // Creatures also need to eat and drink.
    // However herbivores can eat grass.
	int hunger;
  int thirst;
  
  short int fleeCounter; /* How many more turns the Creature should flee for. */
	
  // All loaded creatures can get a link to their world.
  World_Local* map;
  
    //Creature's knowledge of the world (optional).
  Creature_Knowledge* knowledge;
  
  Creature_Species* species;
	
    // INTERACTION
    int nPelt; /* How many pelts you can harvest from the creature. */
    int nMeat; /* How many meats you can harvest from the creature. */
    
      // ATTACK
      // Creatures might get a choice of attacks kinda like Pokemon.
      // For example an elephant might gore you with its tusks, or stomp on you.
      // A dragon might slash you with its claws or breathe fire.
      // Creatures may pick these attacks randomly or intelligently, depending on the creature.
    Vector <Creature_Attack*> vAttack;
	 
	 Texture * baseTexture;
      
    // short int baseAtkSlash;
    // short int baseAtkStab;
    // short int baseAtkBlunt;
  

		// INITIALIZATION
	Creature();
    // Initialise, including roll for stats. 0 - Roll gender. 1 - Male. 2 - Female.
	void init( int _sex = 0);
	
		/* AI FUNCTIONS
		
		*/
	virtual void incrementTicks(int = 1);
	
  virtual void wander() override;
  
	Texture* currentTexture () override;
	
	void die();
  
    // KNOWLEDGE
    
    //returns true if the Creature has seen this tile.
  bool hasSeen( World_Local* /* _map */, int /* _x */, int /* _y */ );
  
  
	/* TABLE INTERFACE */
	std::string getColumn(std::string _column) override;
	std::string getColumnType(std::string _column) override;
  

  /* COMBAT FUNCTIONS */
  
    //Attack a Creature once.
  virtual void attack (Creature*, Creature_Attack*) {}
  virtual void attack (Character*, Creature_Attack*) {}
  
    //Update knowledge with current instance.
  void updateKnowledge();
  // Extra processing available
  void updateKnowledgeIdle();
  
  virtual std::string getName() override;

	
};

// Footprints indicate the type of creature that walked here, the direction they were walking, and a rough
// estimate of how long ago they were here. Footprints disappear over time. Footprints will generally last
// a few hours.
// Footprints probably won't stack, it is assumed one set of footprints will make the others illegible.
// Todo: This could be updated to provide more than just footprints. For example droppings or blood.
// Class could be renamed as Evidence.

class Creature_Footprint: public WorldObject
{
  public:
  
  Creature* owner;
  int direction;
  int age;
  
	Texture* currentTexture () override;
};

#include "Creature_Generator.hpp"
#include "Creature_Manager.cpp"

#endif
