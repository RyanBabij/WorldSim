#pragma once
#ifndef WORLDSIM_IDEA_CPP
#define WORLDSIM_IDEA_CPP

/* WorldSim: Idea.cpp
	#include "Idea.cpp"

	An Idea is a precursor to Technology. A Character will have an idea about how to improve something. Once it spreads
	to the leader or a book is written about it, it will become established as a technology. The originator of the idea
	will be the one credited.
	
	Ideas will later cover more than technology.
	
*/

// Each civ has its own idea progress.

class Character;

class Idea
{
	public:
		Character* originator;
		
		enum IDEA_TYPE
		{
			IDEA_MINING,
			IDEA_SMELTING,
			IDEA_MANUFACTURING,
			IDEA_ASTRONOMY,
			ENUM_COUNT
		};
		
		IDEA_TYPE type;

		// Constructor with initialization list
		Idea(Character* _originator, IDEA_TYPE _type) 
		: originator(_originator), type(_type) 
		{
		}
		
		std::string ideaToString(IDEA_TYPE idea)
		{
			switch (idea)
			{
				case IDEA_MINING:   return "mining";
				case IDEA_SMELTING: return "smelting";
				case IDEA_MANUFACTURING:  return "manufacturing";
				case IDEA_ASTRONOMY:  return "astronomy";
				default:    return "<UNKNOWN IDEA>";
			}
		}
};

// Special idea
// Will be reached once minimum requirements of levels have been met, with random chance on levelup.
// It is an idea with a more detailed description rather than a generic improvement
class SpecialIdea
{
	public:
	
	SpecialIdea()
	{
	}
	
	std::string discoveryText()
	{
		return "A special idea has been discovered.";
	}
};

#endif // WORLDSIM_IDEA_CPP
