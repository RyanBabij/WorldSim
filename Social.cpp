#pragma once
#ifndef WORLDSIM_SOCIAL_CPP
#define WORLDSIM_SOCIAL_CPP

/* WorldSim: Social.cpp
	#include "Social.cpp"

	Manage social standings and interactions. Each character has a Social object.
	A person can have X friends and X enemies, where X is their charisma. A person is always able to know their
	immediate family.

	Compatibility determines whether a person becomes a friend more than anything else. This is not a perfect system
	as it could create a long circuit of relationships but it should work well enough. It also adds an element of
	chance beyond just charisma detemining who will like you.

*/

#include "Social.hpp"

Relationship::Relationship(Character* _sourceCharacter, Character* _destinationCharacter, char _relationshipLevel,
unsigned char _compatibility) : sourceCharacter(_sourceCharacter), destinationCharacter(_destinationCharacter),
relationshipLevel(_relationshipLevel), compatibility(_compatibility)
{
}

Social::Social(Character* _thisCharacter)
: thisCharacter(_thisCharacter)
{
}

unsigned char Social::compatibilityWith(Social* social)
{
	return compatibility.distanceTo(social->compatibility);
}

WrappingUChar Social::getCompatibility()
{
	return compatibility;
}

void Social::setCompatibility(unsigned char _compatibility)
{
	compatibility = _compatibility;
}

void Social::setCompatibility(Social* compatible)
{
	compatibility = compatible->getCompatibility();
}

int Social::isFamily(Character* c)
{
	for (int i = 0; i < vFamily.size(); ++i)
	{
		if (c == vFamily(i).destinationCharacter)
		{
			return i;
		}
	}
	return -1;
}

int Social::isFriend(Character* c)
{
	for (int i = 0; i < vFriend.size(); ++i)
	{
		if (c == vFriend(i).destinationCharacter)
		{
			return i;
		}
	}
	return -1;
}

int Social::isEnemy(Character* c)
{
	for (int i = 0; i < vEnemy.size(); ++i)
	{
		if (c == vEnemy(i).destinationCharacter)
		{
			return i;
		}
	}
	return -1;
}

int Social::isAcquaintance(Character* c)
{
	for (int i = 0; i < vAcquaintance.size(); ++i)
	{
		if (c == vAcquaintance(i).destinationCharacter)
		{
			return i;
		}
	}
	return -1;
}

void Social::interact(Character* c)
{
	// Implementation of interact
}

void Social::updateLists(int maxFriends)
{
	// Implementation of updateLists
}

Vector<Relationship>& Social::getAcquaintances()
{
	return vAcquaintance;
}

#endif // WORLDSIM_SOCIAL_CPP
