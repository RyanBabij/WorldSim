#pragma once
#ifndef WORLDSIM_CHARACTER_SKILL_CPP
#define WORLDSIM_CHARACTER_SKILL_CPP

/* WorldSim: Character_Skill
	#include "Character_Skill.cpp"

	Container for managing Skills of a Character
*/

// For now levelling will be linear, however I might change it in the future.
class Skill
{
	public:
	
	std::string skillName;
	int currentExp;
	int expToNext;
	int level;
	int maxLevel;
	
	
	Skill()
	{
		skillName="";
		currentExp=0;
		expToNext=0;
		level=0;
		maxLevel=100;
	}
	
	void init(std::string _skillName, int _expToNext)
	{
		skillName=_skillName;
		currentExp=0;
		expToNext=_expToNext;
		level=0;
		maxLevel=100;
	}
	
	void addExp(int _amount)
	{
		currentExp+=_amount;
		while ( currentExp > expToNext)
		{
			++level;
			currentExp-=expToNext;
		}
	}
};

#endif
