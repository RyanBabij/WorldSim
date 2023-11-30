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

void Settlement::initSimulation()
{
	location.biome = world->getBiome(worldX,worldY);
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

void Settlement::printAllMoneyInSettlement()
{
	int characterMoney = 0;
	for (int i=0;i<vCharacter.size();++i)
	{
		characterMoney+=vCharacter(i)->getMoney();
	}
	
	int totalMoney=resourceManager.getMoney()+characterMoney+requestManager.getTotalValue();
	
	std::cout<<"Treasury $"<<resourceManager.getMoney()<<". Cash $"<<characterMoney<<" Contracts $"
	<<requestManager.getTotalValue()<<". Total $"<<totalMoney<<".\n"
	<<"Avg wealth: $"<<(int)getAverageCharacterWealth()<<". "
	<<"% in treasury: "<<getMoneyPercentInTreasury()<<".\n";
}

int Settlement::getAllMoneyInSettlement()
{
	// Sum up the money from the resource manager and request manager
	int totalMoney = resourceManager.getMoney() + requestManager.getTotalValue();

	// Add the money from each character
	for (auto& character : vCharacter)
	{
		totalMoney += character->getMoney();
	}

	return totalMoney;
}

double Settlement::getMoneyPercentInTreasury()
{
	int totalMoney = getAllMoneyInSettlement();
	if (totalMoney == 0)
	{
		return 0.0;
	}

	// Calculate the percentage of money in the treasury
	return static_cast<double>(resourceManager.getMoney()) / totalMoney;
}

double Settlement::getAverageCharacterWealth()
{
	double characterWealth = getAllMoneyInSettlement() - resourceManager.getMoney();
	return characterWealth / vCharacter.size();
}

Character* Settlement::getCharacter(Vector <Character*>* vExclude)
{
	Character* character = nullptr;
	for (int i=0;i<vCharacter.size();++i)
	{
		character = vCharacter(i);
		
		// Check if the character is in the exclusion list
		bool isExcluded = false;
		for (int j = 0; j < vExclude->size(); ++j)
		{
			if ((*vExclude)(j) == character)
			{
				isExcluded = true;
				break;
			}
		}

		// If the character is not excluded and has a higher skillFarming, update the current best
		if (!isExcluded)
		{
			return character;
		}
	}
	return nullptr;
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

int Settlement::getPopulation()
{
	return vCharacter.size();
}

int Settlement::getMiningCapacity()
{
	return location.getMiningCapacity();
}

int Settlement::getFarmingCapacity()
{
	return location.getFarmingCapacity();
}

World_Biome* Settlement::getBiome()
{
	return location.biome;
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

void Settlement::buildDwarvenFortress()
{
	location.buildDwarvenFortress();
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
		return DataTools::toString(resourceManager.get(RESOURCE_IRON));
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
