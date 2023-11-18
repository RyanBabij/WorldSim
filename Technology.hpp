#pragma once
#ifndef WORLDSIM_TECHNOLOGY_HPP
#define WORLDSIM_TECHNOLOGY_HPP

/* WorldSim: Technology.hpp
	#include "Technology.hpp"

	Manage the tech tree.
	
	Each Civ and Settlement manage their own tech progress.
	
*/


#include <string> // For std::string

#include "Idea.hpp"

class Technology
{
public:
	enum TECHNOLOGY_TYPE
	{
		TECHNOLOGY_MINING,
		TECHNOLOGY_SMELTING,
		TECHNOLOGY_MANUFACTURING,
		TECHNOLOGY_ASTRONOMY,
		TECHNOLOGY_MILITARY,
		ENUM_COUNT
	};

	int miningLevel;
	int smeltingLevel;
	int manufacturingLevel;
	int astronomyLevel;
	int militaryLevel;

	Technology();
	
	void addIdea(Idea);
	bool isSpecialIdea(Idea);
	SpecialIdea addSpecialIdea(Idea);

	TECHNOLOGY_TYPE advance();
	std::string techToString(TECHNOLOGY_TYPE tech);
};

class SpecialTechnology
{
	public:
		SpecialTechnology();
		std::string discoveryText();
};

class SpecialTechnology_Adamantium : public SpecialTechnology
{
	public:
		SpecialTechnology_Adamantium();
};

#endif // WORLDSIM_TECHNOLOGY_HPP