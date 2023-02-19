#pragma once
#ifndef CHARACTER_MANAGER_CPP
#define CHARACTER_MANAGER_CPP

/* WorldSim: CharacterManager.cpp
	#include "CharacterManager.cpp"

	Container for storing groups of Characters, called a population. A population is a group of people who live in the same area. They will share culture, language, technology, etc. A Character can only belong to one population. A population does not have a fixed race.
	
	Could be further subdivided into cultural groups.
	
*/

#include "Character.hpp"

class CharacterManager
{
	public:
	// The name the population calls themselves.
	std::string name;
	
	//culture
	//religion
	//language
	//leadership and government
	Character* leader;
	
	Vector <Character*> vPopulation;

	CharacterManager()
	{
		name="?POPULATION?";
		leader=0;
	}
	
	Character* get(int i)
	{
		if (vPopulation.isSafe(i))
		{
			return vPopulation(i);
		}
		return 0;
	}
	
	void add(Character* _character)
	{
		vPopulation.push(_character);
	}
	
	void clear()
	{
		vPopulation.clear();
	}
	
	int size()
	{
		return vPopulation.size();
	}
	
	bool chooseLeader()
	{
		// build candidates and then make a story to explain how the leader was chosen.
		
		if ( vPopulation.size() > 0 )
		{
			// select one randomly for now.
		}
		
		return true;
	}
	
	bool hasLeader()
	{
		if (leader==0 || leader->isAlive==false)
		{
			return false;
		}
		return true;
	}
	
};



#endif
