#pragma once
#ifndef WORLDSIM_TECHNOLOGY_CPP
#define WORLDSIM_TECHNOLOGY_CPP

#include "Technology.hpp"

Technology::Technology()
{
	miningLevel = 0;
	smeltingLevel = 0;
	manufacturingLevel = 0;
	astronomyLevel = 0;
}

void Technology::addIdea(Idea idea)
{
	if ( idea.type == Idea::IDEA_TYPE::IDEA_MINING )
	{
		miningLevel+=1;
		
		if (miningLevel == 5)
		{
			// this tech unlocks adamantine
			
			
		}
	}
	else if ( idea.type == Idea::IDEA_TYPE::IDEA_SMELTING )
	{
		smeltingLevel+=1;
	}
	else if ( idea.type == Idea::IDEA_TYPE::IDEA_MANUFACTURING )
	{
		manufacturingLevel+=1;
	}
	else if ( idea.type == Idea::IDEA_TYPE::IDEA_ASTRONOMY )
	{
		astronomyLevel+=1;
	}
}

bool Technology::isSpecialIdea(Idea idea)
{
	if ( idea.type == Idea::IDEA_TYPE::IDEA_MINING )
	{
		if (miningLevel+1 == 5)
		{
			// this tech unlocks adamantine
			return true;
			
		}
	}
	else if ( idea.type == Idea::IDEA_TYPE::IDEA_SMELTING )
	{
	}
	else if ( idea.type == Idea::IDEA_TYPE::IDEA_MANUFACTURING )
	{
	}
	else if ( idea.type == Idea::IDEA_TYPE::IDEA_ASTRONOMY )
	{
	}
	return false;
}

Technology::TECHNOLOGY_TYPE Technology::advance()
{
	TECHNOLOGY_TYPE type = static_cast<TECHNOLOGY_TYPE>(globalRandom.rand8(ENUM_COUNT));
	return type;
}

std::string Technology::techToString(TECHNOLOGY_TYPE tech)
{
	switch (tech)
	{
		case TECHNOLOGY_MINING:   return "mining";
		case TECHNOLOGY_SMELTING: return "smelting";
		case TECHNOLOGY_MANUFACTURING:  return "manufacturing";
		case TECHNOLOGY_ASTRONOMY:  return "astronomy";
		default:    return "<UNKNOWN TECH>";
	}
}

SpecialTechnology::SpecialTechnology()
{
}

std::string SpecialTechnology::discoveryText()
{
	return "A special technology has been discovered.";
}

SpecialTechnology_Adamantium::SpecialTechnology_Adamantium()
{
}


#endif // WORLDSIM_TECHNOLOGY_CPP
