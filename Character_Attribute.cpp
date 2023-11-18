#pragma once
#ifndef WORLDSIM_ATTRIBUTE_CPP
#define WORLDSIM_ATTRIBUTE_CPP

/* WorldSim: Skill.cpp
	#include "Skill.cpp"

	Manage Character skills
	
*/

class AttributeManager
{
	public:
	
		enum TYPE
		{
			STRENGTH,
			AGILITY,
			CHARISMA,
			INTELLIGENCE,
			PERCEPTION,
			ENDURANCE,
			COURAGE,
			ENUM_COUNT
		};
	
		/* Governing attributes 0-10  (Might remove later) */
		char strength; // should govern carry capacity
		char agility; // governs movement, dodge
		char charisma; // governs relationships
		char intelligence; // should govern exp gain
		char perception; // governs line of sight and traps
		char endurance; // should govern energy level and expenditure
		char courage; // governs morale, possibly also luck
		
		short int total;
		
		AttributeManager()
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
			std::cout<<"Attributes:\n";
			std::cout<<"Strength:\t"<<(int)strength<<"\n";
			std::cout<<"Agility:\t"<<(int)agility<<"\n";
			std::cout<<"Charisma:\t"<<(int)charisma<<"\n";
			std::cout<<"Intelligence:\t"<<(int)intelligence<<"\n";
			std::cout<<"Perception:\t"<<(int)perception<<"\n";
			std::cout<<"Endurance:\t"<<(int)endurance<<"\n";
			std::cout<<"Courage:\t"<<(int)courage<<"\n";
		}
		
		std::string toString(bool showStars = true) const
		{
			std::ostringstream oss;
			auto formatSkill = [&oss, showStars](const std::string& skillName, char skillLevel)
			{
				oss << skillName;
				if (showStars)
				{
					for (int i = 0; i < 10; ++i)
					{
						oss << (i < skillLevel ? '*' : '_');
					}
					oss << " (" << static_cast<int>(skillLevel) << ")\n";
				}
				else
				{
					oss << std::string(10 - skillName.length(), '_') << static_cast<int>(skillLevel) << "\n";
				}
			};

			formatSkill("Strength:     ", strength);
			formatSkill("Agility:      ", agility);
			formatSkill("Charisma:     ", charisma);
			formatSkill("Intelligence: ", intelligence);
			formatSkill("Perception:   ", perception);
			formatSkill("Endurance:    ", endurance);
			formatSkill("Courage:      ", courage);

			return oss.str();
		}


		void updateTotal()
		{
			total = (short int)strength+agility+charisma+intelligence+perception+endurance+courage;
		}
		
		TYPE getBestSkill()
		{
			char maxSkillValue = strength;
			TYPE bestSkill = STRENGTH;

			if (agility > maxSkillValue)
			{
				maxSkillValue = agility;
				bestSkill = AGILITY;
			}
			if (charisma > maxSkillValue)
			{
				maxSkillValue = charisma;
				bestSkill = CHARISMA;
			}
			if (intelligence > maxSkillValue)
			{
				maxSkillValue = intelligence;
				bestSkill = INTELLIGENCE;
			}
			if (perception > maxSkillValue)
			{
				maxSkillValue = perception;
				bestSkill = PERCEPTION;
			}
			if (endurance > maxSkillValue)
			{
				maxSkillValue = endurance;
				bestSkill = ENDURANCE;
			}
			if (courage > maxSkillValue)
			{
				maxSkillValue = courage;
				bestSkill = COURAGE;
			}

			return bestSkill;
		}
		
		char getSkillValue(TYPE skillType) const
		{
			switch (skillType)
			{
				case STRENGTH:
					return strength;
				case AGILITY:
					return agility;
				case CHARISMA:
					return charisma;
				case INTELLIGENCE:
					return intelligence;
				case PERCEPTION:
					return perception;
				case ENDURANCE:
					return endurance;
				case COURAGE:
					return courage;
				default:
					// Handle invalid skill type if necessary
					return 0;
			}
		}
		
		void setSkillValue(TYPE skillType, char value)
		{
			switch (skillType)
			{
				case STRENGTH:
					strength = value;
					break;
				case AGILITY:
					agility = value;
					break;
				case CHARISMA:
					charisma = value;
					break;
				case INTELLIGENCE:
					intelligence = value;
					break;
				case PERCEPTION:
					perception = value;
					break;
				case ENDURANCE:
					endurance = value;
					break;
				case COURAGE:
					courage = value;
					break;
				default:
					return;
			}
		}

};

#endif // WORLDSIM_ATTRIBUTE_CPP
