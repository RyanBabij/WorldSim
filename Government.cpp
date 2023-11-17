#pragma once
#ifndef WORLDSIM_GOVERNMENT_CPP
#define WORLDSIM_GOVERNMENT_CPP

/* WorldSim: Government.cpp
	#include "Government.cpp"

	The government controls executive decisions of a Settlement and/or Civ
	
*/

#include "Government.hpp"
#include "Settlement.hpp"

// Government_Position definitions
Government_Position::Government_Position(Government* _government) : government(_government)
{
	character = nullptr;
}

Government_Position::Government_Position(const Government_Position& other)
: government(other.government), character(other.character)
{
}

Government_Position& Government_Position::operator=(const Government_Position& other)
{
	if (this != &other)
	{
		government = other.government;
		character = other.character;
	}
	return *this;
}

bool Government_Position::operator==(const Character* otherCharacter) const
{
	return character == otherCharacter;
}

void Government_Position::assign(Character* _character)
{
	character = _character;
}

bool Government_Position::empty()
{
	return (character == nullptr);
}

// Government_Leader definitions
Government_Leader::Government_Leader(Government* _government) : Government_Position(_government)
{
}

void Government_Leader::govern()
{
	if (government == nullptr || government->governedSettlement == nullptr)
	{
		return;
	}

	if (empty())
	{
		std::cout << "There is no king\n";
		return;
	}
	std::cout << "King is kinging.\n";

	if (character->vIdea.size() > 0 && government->governedSettlement!=0)
	{
		if (!government->governedSettlement->hasIdea(character->vIdea(0)))
		{
			government->governedSettlement->giveIdea(character->vIdea(0));
			character->vIdea.removeSlot(0);
			std::cout<<"King has implemented an idea as a tech\n";
		}
	}
}

// Government_Scribe definitions
Government_Scribe::Government_Scribe(Government* _government) : Government_Position(_government)
{
}

void Government_Scribe::govern()
{
	if (empty())
	{
		std::cout << "There is no scribe\n";
		return;
	}
	std::cout << "Scribe is scribing.\n";
}

// Government_Captain definitions
Government_Captain::Government_Captain(Government* _government) : Government_Position(_government)
{
}

void Government_Captain::govern()
{
	if (empty())
	{
		std::cout << "There is no captain\n";
		return;
	}
	std::cout << "Captain is capping.\n";
}

// Government definitions
Government::Government() : leader(this), scribe(this), captain(this)
{
	governedSettlement=0;
}

Government::Government(const Government& other)
	: governedSettlement(other.governedSettlement), leader(this), scribe(this), captain(this)
{
}

Government& Government::operator=(const Government& other)
{
	if (this != &other)
	{
		governedSettlement = other.governedSettlement;
		setLeader(other.leader.character);
		setScribe(other.scribe.character);
		setCaptain(other.captain.character);
	}
	return *this;
}

void Government::govern()
{
	leader.govern();
	scribe.govern();
	captain.govern();
}

bool Government::needsLeader()
{
	return leader.empty();
}

void Government::setLeader(Character* character)
{
	leader.assign(character);
}

void Government::setScribe(Character* character)
{
	scribe.assign(character);
}

void Government::setCaptain(Character* character)
{
	captain.assign(character);
}


#endif // WORLDSIM_GOVERNMENT_CPP
