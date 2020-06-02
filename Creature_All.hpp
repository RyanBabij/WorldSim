#pragma once
#ifndef WORLDSIM_CREATURE_ALL_HPP
#define WORLDSIM_CREATURE_ALL_HPP

/* WorldSim: Creature_All.hpp
   #include"Creature_All.hpp"

*/

#include "Creature.hpp"

class Creature_Bat: public Creature
{
   public:

   Creature_Bat();
  ~Creature_Bat();
  
   void init( int _sex = 0);
  
  virtual void incrementTicks (int nTicks) override;
  
  virtual void attack (Creature*, Creature_Attack*) override;
  virtual void attack (Character*, Creature_Attack*) override;
  
  virtual void wander() override;
  
   virtual Texture* currentTexture() override;
  virtual std::string getName() override;
};


// Test creatures. They will later be given some vaeriety once the basic wildlife modelling is in place.

// Herbivore which hangs out in groups and grazes on grass. Obviously only present in grassy biomes.
// An example would be zebras or horses.
// They are individually weak and slow but safe in large packs. Generally the old or sick animals will fall behind and be eaten by carnivores.
class Creature_HerbivoreGrazing: public Creature
{
   public:

   Creature_HerbivoreGrazing();
  ~Creature_HerbivoreGrazing();
  
   void init( int _sex = 0);
  
  virtual void incrementTicks (int nTicks) override;
  
  virtual void attack (Creature*, Creature_Attack*) override;
  virtual void attack (Character*, Creature_Attack*) override;
  
  virtual void wander() override;
  
   virtual Texture* currentTexture() override;
  virtual std::string getName() override;
};

// Herbivore which travels in family groups and ranges over medium or long distances to find food.
// An example would be deer or camels
// They rely on speed and alertness to escape predators.
class Creature_HerbivoreScavenging: public Creature
{
   public:

   Creature_HerbivoreScavenging();
  ~Creature_HerbivoreScavenging();
  
   void init( int _sex = 0);
  
  virtual void incrementTicks (int nTicks) override;
  
  virtual void attack (Creature*, Creature_Attack*) override;
  virtual void attack (Character*, Creature_Attack*) override;
  
  virtual void wander() override;
  
   virtual Texture* currentTexture() override;
  virtual std::string getName() override;
};

// Carnivores which hunt in packs. Examples would be hyenas or wolves.
// They rely on teamwork to attack larger creatures
class Creature_CarnivorePack: public Creature
{
   public:

   Creature_CarnivorePack();
  ~Creature_CarnivorePack();
  
   void init( int _sex = 0);
  
  virtual void incrementTicks (int nTicks) override;
  
  virtual void attack (Creature*, Creature_Attack*) override;
  virtual void attack (Character*, Creature_Attack*) override;
  
  virtual void wander() override;
  
   virtual Texture* currentTexture() override;
  virtual std::string getName() override;
};

// Carnivores which hunt alone or in family groups. They rely on their
// individual strength to catch prey.
// An example would be bears.
class Creature_CarnivoreSolo: public Creature
{
   public:

   Creature_CarnivoreSolo();
  ~Creature_CarnivoreSolo();
  
   void init( int _sex = 0);
  
  virtual void incrementTicks (int nTicks) override;
  
  virtual void attack (Creature*, Creature_Attack*) override;
  virtual void attack (Character*, Creature_Attack*) override;
  
  virtual void wander() override;
  
   virtual Texture* currentTexture() override;
  virtual std::string getName() override;
};

#endif
