#pragma once
#ifndef WORLDSIM_TECHNOLOGY_CPP
#define WORLDSIM_TECHNOLOGY_CPP

/* WorldSim: Technology.cpp
	#include "Technology.cpp"

	Manage the tech tree.
	
*/

// Each civ has its own technology progress.

class Technology
{
	public:
	
		enum TECHNOLOGY_TYPE
		{
			TECHNOLOGY_MINING,
			TECHNOLOGY_SMELTING,
			TECHNOLOGY_MANUFACTURING,
			TECHNOLOGY_ASTRONOMY,
			ENUM_COUNT
		};
	
		// MINING
		int miningLevel; // Increases vein hit chance
		int smeltingLevel; // Increases ore quality
		// MANUFACTURING
		int manufacturingLevel;
		// ASTRONOMY
		int astronomyLevel;
		
		Technology()
		{
			miningLevel=0;
			smeltingLevel=0;
			manufacturingLevel=0;
			astronomyLevel=0;
		}
		
		TECHNOLOGY_TYPE advance()
		{
			// pick a field and advance it.
			TECHNOLOGY_TYPE type = static_cast<TECHNOLOGY_TYPE>(globalRandom.rand8(ENUM_COUNT));
			
			return type;
		}
		
		std::string techToString(TECHNOLOGY_TYPE tech)
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
};

// Special technology
// Will be reached once minimum requirements of levels have been met, with random chance on levelup.
class SpecialTechnology
{
	public:
	
	SpecialTechnology()
	{
	}
	
	std::string discoveryText()
	{
		return "A special technology has been discovered.";
	}
};

class SpecialTechnology_Adamantium: public SpecialTechnology
{
	public:
	SpecialTechnology_Adamantium()
	{
	}
};

#endif // WORLDSIM_TECHNOLOGY_CPP
