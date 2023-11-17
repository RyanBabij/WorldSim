#pragma once
#ifndef WORLDSIM_IDEA_CPP
#define WORLDSIM_IDEA_CPP

/* WorldSim: Idea.cpp
	#include "Idea.cpp"

	An Idea is a precursor to Technology. A Character will have an idea about how to improve something. Once it spreads
	to the leader or a book is written about it, it will become established as a technology. The originator of the idea
	will be the one credited.
	
	Ideas will later cover more than technology.
	
	Each Civ and Settlement manages its own idea progress.
	
*/

#include "Idea.hpp"

int Idea::lastID = 0;

Idea::Idea(Character* _originator, IDEA_TYPE _type) 
: id(++lastID), originator(_originator), type(_type)
{
}

Idea::Idea(const Idea& other)
: id(other.id), originator(other.originator), type(other.type)
{
}

Idea& Idea::operator=(const Idea& other)
{
	if (this != &other)
	{
		originator = other.originator;
		// Do not change the ID
		type = other.type;
	}
	return *this;
}

bool Idea::operator==(const Idea& other) const
{
	return id == other.id;
}

std::string Idea::ideaToString(IDEA_TYPE idea)
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

SpecialIdea::SpecialIdea()
{
}

std::string SpecialIdea::discoveryText()
{
	return "A special idea has been discovered.";
}


#endif // WORLDSIM_IDEA_CPP
