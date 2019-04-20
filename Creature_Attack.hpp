#pragma once
#ifndef WORLDSIM_CREATURE_ATTACK_HPP
#define WORLDSIM_CREATURE_ATTACK_HPP

/* WorldSim: Creature_Attack
  #include "Creature_Attack.hpp"
  
  Some creatures can choose from multiple attacks. Attacks can do different types of damage and status effects.
*/


// Creatures get a choice of attacks to make, kinda like Pokemon or RPG battles.
// This should add some strategy and flavour to combat and makes it less of a grind.
// Wrestling and takedowns could also be options in some cases.

class Creature_Attack
{
  public:
  
  std::string baseName; /* Action: <X> */
  std::string actionName; /* The creature <X> you. */
  std::string pastTenseName; /* That creature just <X> me. */
  
  short int reach; /* How far the attack can reach */
  
  // Area effect
  // Status effect
  
  short int baseSlash; /* Balance of damage and bleed */
  short int baseStab; /* Extra bleed and critical chance */
  short int baseBlunt; /* High armour bypass, no bleed */
  
  
  void generate( std::string _baseName, std::string _actionName, std::string _pastTenseName, short int _reach,
    short int _baseSlash, short int _baseStab, short int _baseBlunt )
  {
    baseName = _baseName;
    actionName = _actionName;
    pastTenseName = _pastTenseName;
    
    reach = _reach;
    
    baseSlash = _baseSlash;
    baseStab = _baseStab;
    baseBlunt = _baseBlunt;
  }
  
};

Creature_Attack atkBatClaw; /* Bat attacks with claws. */
Creature_Attack atkScreech; /* Bat screeches to disorient enemy */

void initCreatureAttacks()
{
  atkBatClaw.generate("Claw", "claws", "clawed", 1, 1,0,0);
  atkScreech.generate("Screech", "screeches at", "screeched at", 5, 0,0,0);
}

#endif
