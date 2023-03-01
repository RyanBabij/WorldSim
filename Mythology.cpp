#pragma once
#ifndef WORLDSIM_MYTHOLOGY_CPP
#define WORLDSIM_MYTHOLOGY_CPP

/* WorldSim: Mythology
	#include "Mythology.cpp"

	Container for mythology of a group of Characters.
	
	Each Civ may have a "State Mythology", however each Character can choose their own.
*/

#include "Mythology.hpp"

Mythology::Mythology()
{
	world=0;
}

// Generate the base Dwarven mythology.
void Mythology::generateBaseDwarven()
{
	std::cout<<"Generating base Dwarven mythology.\n";
	
	race=dwarven;
}
void Mythology::generateBaseElven()
{
	std::cout<<"Generating base Elven mythology.\n";
}
void Mythology::generateBaseHuman()
{
	std::cout<<"Generating base Human mythology.\n";
}

void Mythology::addDeity(std::string _name, Mythology_Deity::mythology_deity_type _type)
{
}

// do some stuff
// Basically we just loop through every acting deity and let them do something.
void Mythology::increment()
{
	
	for (int i=0;i<vDeity.size();++i)
	{
		vDeity(i)->act();
	}
	
}

#endif
