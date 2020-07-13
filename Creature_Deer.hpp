#pragma once
#ifndef WORLDSIM_CREATURE_DEER_HPP
#define WORLDSIM_CREATURE_DEER_HPP

/* WorldSim: Creature_Deer.hpp
	#include"Creature_Deer.hpp"

	Deers have very good hearing and smell, and run very fast. Popular for hunting.
*/

#include "Creature.hpp"

class Creature_Deer: public Creature
{
	public:

	Creature_Deer();
	~Creature_Deer();

	void init( int _sex = 0);

	virtual Texture* currentTexture();
	virtual std::string getName();
};

#endif
