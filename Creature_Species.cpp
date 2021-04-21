#pragma once
#ifndef WORLDSIM_CREATURE_SPECIES_CPP
#define WORLDSIM_CREATURE_SPECIES_CPP

/* WorldSim: Creature_Species
  #include "Creature_Species.cpp"
  
  Implementation of Creature_Species.hpp
*/

#include <Container/Table/TableInterface.hpp>

#include "Creature.hpp"

	Creature_Species::Creature_Species(std::string _name, int _spawnWeight)
	{
		name = _name;
		spawnWeight = _spawnWeight;
	}
	
	// return an instance of this species
	Creature* Creature_Species::spawn()
	{
		Creature* c = new Creature;
		c->species=this;
		return c;
	}
	
	// TABLE INTERFACE
	std::string Creature_Species::getColumn(std::string _column)
	{
		if ( _column=="name" )
		{
			return name;
		}
		return "?";
	}
	std::string Creature_Species::getColumnType(std::string _column)
	{
		return "string";
	}
	
	void Creature_Species::setBaseTexture(Texture* texture)
	{
		baseTexture = texture;
	}
	
	// HASTEXTURE
	Texture* Creature_Species::currentTexture ()
	{
		return 0;
	}

#endif
