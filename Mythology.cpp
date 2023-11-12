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
	
	race=DWARVEN;
	type = Mythology::MYTHOLOGY_MONOTHEISTIC;
	
	
}
void Mythology::generateBaseElven()
{
	std::cout<<"Generating base Elven mythology.\n";
}
void Mythology::generateBaseHuman()
{
	std::cout<<"Generating base Human mythology.\n";
}

void Mythology::addDeity(std::string _name, Mythology_Deity::PERSONALITY _type)
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

std::string Mythology::getMythologyType()
{
	return "<MYTH TYPE>";
}

std::string Mythology::getMythologyDescription()
{
	return "<MYTH DESC>";
}

std::string Mythology::getColumn(std::string _column)
{
	if (_column == "date")
	{
		return "date"; // Placeholder for actual date handling
	}
	// else if (_column == "type")
	// {
		// return getEventTypeStr();
	// }
	// else if (_column == "description")
	// {
		// return eventDescription;
	// }
	// return eventDescription; // This might be a mistake; consider handling unknown columns
	return "<DATA>";
}

std::string Mythology::getColumnType(std::string _column)
{
	// Placeholder for actual type handling
	return "string";
}

std::string Mythology::getLongDescription()
{
	return "<MYTHOLOGY DESC>";
}


#endif
