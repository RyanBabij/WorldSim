#pragma once
#ifndef WORLDSIM_SETTLEMENT_CPP
#define WORLDSIM_SETTLEMENT_CPP

#include "Settlement.hpp"

#include "Government.cpp"

Settlement::Settlement()
{
	world = 0;
	colourRed = 0;
	colourGreen = 0;
	colourBlue = 0;

	race = NONE;
	parentCiv = 0;

	random.seed(Random::randInt(99999));
	
	name=globalNameGen.generate();
}

std::string Settlement::getName()
{
	return name;
}

Character* Settlement::getRandomCharacter()
{
	if (vCharacter.empty())
	{ 
		return 0; 
	}
	
	return vCharacter(globalRandom.rand(vCharacter.size()-1));
}

bool Settlement::removeCharacter(Character* _character)
{
	return vCharacter.erase(_character);
}

Texture* Settlement::currentTexture()
{
	return &TEX_TEST_404;
}

/* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement::incrementTicks(int /* nTicks */)
{
	std::cout << "Incrementing settlement\n";

	return;
}

bool Settlement::hasIdea(Idea idea)
{
	for (int i=0;i<vIdea.size();++i)
	{
		if ( vIdea(i)==idea )
		{
			return true;
		}
	}
	return false;
}

bool Settlement::isSpecialIdea(Idea idea)
{
	return technology.isSpecialIdea(idea);
}

void Settlement::giveIdea(Idea idea)
{
	vIdea.push(idea);
	
	if ( technology.isSpecialIdea(idea) )
	{
		std::cout<<"SPECIAL IDEA BY "<<idea.originator->getFullName()<<".\n";
		SpecialIdea special = technology.addSpecialIdea(idea);
		idea.originator->vOriginalSpecialIdea.push(special);
		
		return;
	}
	// convert it to tech here
	technology.addIdea(idea);
}

/* TABLE INTERFACE */

std::string Settlement::getColumn(std::string _column)
{
	if (_column == "name")
	{
		return name;
	}
	if (_column == "race")
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
  
	if (_column == "metal")
	{
		return DataTools::toString(nMetalStockpile);
	}
	if (_column == "food")
	{
		//return DataTools::toString(nFood);
	}
	if (_column == "population")
	{
		return DataTools::toString(vCharacter.size());
	}
	if (_column == "coordinates")
	{
		//return DataTools::toString(worldX)+","+DataTools::toString(worldY);
	}
	if (_column == "territory")
	{
		//return DataTools::toString(vTerritory.size());
	}
	if (_column == "civilization")
	{
		if (parentCiv == 0)
		{
			return "?";
		}
		return parentCiv->name;
	}
	// else if (_column == "id")
	// {
	// 	return DataTools::toString(id);
	// }
	
	return "?";
}
std::string Settlement::getColumnType(std::string _column)
{
	if (_column == "population" || _column == "territory" || _column == "food" || _column == "metal")
	{
		return "number";
	}
	return "string";
}

#endif // WORLDSIM_SETTLEMENT_CPP
