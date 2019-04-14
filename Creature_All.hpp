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
  
  virtual void incrementTicks (int nTicks);
  
	virtual Texture* currentTexture();
  virtual std::string getName();
};

#endif
