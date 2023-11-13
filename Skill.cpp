#pragma once
#ifndef WORLDSIM_SKILL_CPP
#define WORLDSIM_SKILL_CPP

/* WorldSim: Skill.cpp
	#include "Skill.cpp"

	Manage Character skills
	
*/

class BaseSkillManager
{
	public:
	
		/* Governing attributes 0-10  (Might remove later) */
		char strength; // should govern carry capacity
		char agility; // governs movement, dodge
		char charisma; // governs relationships
		char intelligence; // should govern exp gain
		char perception; // governs line of sight and traps
		char endurance; // should govern energy level and expenditure
		char courage; // governs morale, possibly also luck
		
		short int total;
		
		BaseSkillManager()
		{
			strength=0;
			agility=0;
			charisma=0;
			intelligence=0;
			perception=0;
			endurance=0;
			courage=0;
		}
		
		void roll(bool isMale)
		{
			strength=flip();
			agility=flip();
			charisma=flip();
			intelligence=flip();
			perception=flip();
			endurance=flip();
			courage=flip();
			
			updateTotal();
		}
		
		char flip()
		{
			return globalRandom.multiFlip(10);
		}
		
		void print()
		{
			std::cout<<"Skills:\n";
			std::cout<<"Strength:\t"<<(int)strength<<"\n";
			std::cout<<"Agility:\t"<<(int)agility<<"\n";
			std::cout<<"Charisma:\t"<<(int)charisma<<"\n";
			std::cout<<"Intelligence:\t"<<(int)intelligence<<"\n";
			std::cout<<"Perception:\t"<<(int)perception<<"\n";
			std::cout<<"Endurance:\t"<<(int)endurance<<"\n";
			std::cout<<"Courage:\t"<<(int)courage<<"\n";
		}

		void updateTotal()
		{
			total = (short int)strength+agility+charisma+intelligence+perception+endurance+courage;
		}

};

#endif // WORLDSIM_SKILL_CPP
