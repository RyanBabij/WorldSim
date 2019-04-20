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

#endif
