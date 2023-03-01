#pragma once
#ifndef WORLDSIM_CIV_CPP
#define WORLDSIM_CIV_CPP

/* WorldSim: Civ
  #include "Civ.cpp"
*/

#include "Civ.hpp"
#include "Settlement.hpp"

Civ::Civ()
{
	name="N/A";
	money=100;
	world=0;
	mythology=0;
}

void Civ::init(World* _world)
{
	world = _world;
	aVisible.init(world->nX,world->nY,false);
}

void Civ::addSettlement(Settlement * _settlement)
{
  _settlement->world = world;
  vSettlement.push(_settlement);
}

void Civ::incrementTicks ( int nTicks )
{
	/* Update each city. */
	
	for ( int i=0;i<vSettlement.size();++i)
	{
		vSettlement(i)->incrementTicks(nTicks);
	}
	rebuildCharacterList();
}

void Civ::setColour( const int r, const int g, const int b)
{
	colourRed=r;
	colourGreen=g;
	colourBlue=b;
}

void Civ::rebuildCharacterList()
{
	vCharacter.clear();
	for (int i=0;i<vSettlement.size();++i)
	{
		for (int j=0;j<vSettlement(i)->vCharacter.size();++j)
		{
			if(vSettlement(i)->vCharacter(j)->isAlive)
			{
				vCharacter.push(vSettlement(i)->vCharacter(j));
			}
		}
	}
}

/* TABLE INTERFACE */

std::string Civ::getColumn(std::string _column)
{
	if ( _column=="name" )
	{
		return name;
	}
	if ( _column=="race" )
	{
		if (race == NONE)
    {
      return "None";
    }
		if (race == HUMAN)
    {
      return "Human";
    }
		if (race == DWARVEN)
    {
      return "Dwarven";
    }
		if (race == ELVEN)
    {
      return "Elven";
    }
    
	}
	if ( _column=="population" )
	{
		return DataTools::toString(vCharacter.size());
	}
	
	return "?";
}
std::string Civ::getColumnType(std::string _column)
{
	if ( _column == "population" || _column == "territory" || _column == "food" )
	{
		return "number";
	}
	return "string";
}

#endif
