#pragma once
#ifndef WORLDSIM_GOVERNMENT_CPP
#define WORLDSIM_GOVERNMENT_CPP

/* WorldSim: Government.cpp
	#include "Government.cpp"

	The government controls executive decisions of a Settlement and/or Civ
	
*/

#include "Character.hpp"

class Government;

class Government_Position
{
	public:
	Government* government;
	Character* character;
	
	Government_Position(Government* _government): government(_government)
	{
		character=nullptr;
	}
	
	Government_Position(const Government_Position& other)
		 : government(other.government), character(other.character)
	{
	}

	Government_Position& operator=(const Government_Position& other)
	{
		if (this != &other) // Check for self-assignment
		{
			government = other.government;
			character = other.character;
		}
		return *this;
	}

	// Does the passed Character have this job?
	bool operator==(const Character* otherCharacter) const
	{
		return character == otherCharacter;
	}
	
	virtual void govern()=0;
	
	virtual void assign(Character* _character)
	{
		character=_character;
	}
	
	bool empty()
	{
		return (character==0);
	}
};

class Government_Leader: public Government_Position
{
	public:

	Government_Leader(Government* _government) : Government_Position(_government)
	{
	}
	
	virtual void govern()
	{
		if ( empty() )
		{
			std::cout<<"There is no king\n";
			return;
		}
		std::cout<<"King is kinging.\n";
		
		if ( character->vIdea.size() > 0 )
		{
			//std::cout<<"King has ideas\n";
		}
	}
};

class Government_Scribe: public Government_Position
{
	public:
	
	Government_Scribe(Government* _government) : Government_Position(_government)
	{
	}
	
	virtual void govern()
	{
		if ( empty() )
		{
			std::cout<<"There is no scribe\n";
			return;
		}
		std::cout<<"Scribe is scribing.\n";
	}
};

class Government_Captain: public Government_Position
{
	public:
	
	Government_Captain(Government* _government) : Government_Position(_government)
	{
	}
	
	virtual void govern()
	{
		if ( empty() )
		{
			std::cout<<"There is no captain\n";
			return;
		}
		std::cout<<"Captain is capping.\n";
	}
};



class Government
{
	public:
		Settlement* governedSettlement;
		Government_Leader leader;
		Government_Scribe scribe;
		Government_Captain captain;
		
		Vector <Government_Position> vWorkers;
		
		
		// Constructor with member initializer list
		Government() : leader(this), scribe(this), captain(this)
		{
		}
		
		// Copy constructor
		Government(const Government& other)
		: governedSettlement(other.governedSettlement), // Shallow copy
		leader(this), // Reinitialize with this Government instance
		scribe(this), // Reinitialize with this Government instance
		captain(this) // Reinitialize with this Government instance
		{
		}
		
		Government& operator=(const Government& other)
		{
			if (this != &other) // Check for self-assignment
			{
				// Copy the simple fields
				governedSettlement = other.governedSettlement; // Shallow copy

				// Reinitialize the positions with this Government instance
				setLeader(other.leader.character);
				setCaptain(other.captain.character);
				setScribe(other.scribe.character);
				// Copy the vector (assuming Vector supports assignment)
				//vWorkers = other.vWorkers;
			}
			return *this;
		}
	 
		void govern()
		{
			leader.govern();
			scribe.govern();
			captain.govern();
		}

		bool needsLeader()
		{
			if (leader.empty())
			{
				return true;
			}
			return false;
		}
		void setLeader(Character* character)
		{
			leader.assign(character);
		}
		void setScribe(Character* character)
		{
			scribe.assign(character);
		}
		void setCaptain(Character* character)
		{
			captain.assign(character);
		}
};

#endif // WORLDSIM_GOVERNMENT_CPP
