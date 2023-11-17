#pragma once
#ifndef WORLDSIM_IDEA_HPP
#define WORLDSIM_IDEA_HPP

/* WorldSim: Idea.hpp
	#include "Idea.hpp"

	An Idea is a precursor to Technology. A Character will have an idea about how to improve something. Once it spreads
	to the leader or a book is written about it, it will become established as a technology. The originator of the idea
	will be the one credited.
	
	Ideas will later cover more than technology.
	
*/

class Character;

class Idea
{
public:
	static int lastID; // Static variable to keep track of the last assigned ID
	int id;
	Character* originator;
	bool isSpecialIdea;

	enum IDEA_TYPE
	{
		IDEA_MINING,
		IDEA_SMELTING,
		IDEA_MANUFACTURING,
		IDEA_ASTRONOMY,
		IDEA_MILITARY,
		ENUM_COUNT
	};

	IDEA_TYPE type;

	Idea(Character* _originator, IDEA_TYPE _type);
	Idea(const Idea& other);
	Idea& operator=(const Idea& other);
	bool operator==(const Idea& other) const;
	std::string ideaToString(IDEA_TYPE idea);
};



class SpecialIdea: public Idea
{
	public:
		enum TYPE
		{
			ADAMANTINE,
			ENUM_COUNT
		};
	
		std::string specialText;
		SpecialIdea(Character* originator, IDEA_TYPE type);
		std::string discoveryText();
};

#endif // WORLDSIM_IDEA_HPP
