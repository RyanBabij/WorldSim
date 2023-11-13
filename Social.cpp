#pragma once
#ifndef WORLDSIM_SOCIAL_CPP
#define WORLDSIM_SOCIAL_CPP

/* WorldSim: Social.cpp
	#include "Social.cpp"

	Manage social standings and interactions. Each character has a Social object.
	
*/

#include "Character.hpp"
#include <Math/WrappingUChar.cpp>

class Relationship
{
	Character* sourceCharacter;
	Character* destinationCharacter;
	char relationshipLevel;
};

class Social
{
	// wrapping number from 0-255. Closer distance means more compatible. 0 means potential for best friends.
	// 125 can lead to a nemesis situation
	WrappingUChar compatibility; 
	Vector <Relationshop> vFamily; // Should probably be kept as its own thing
	Vector <Relationship> vFriends; // Probably should have a limiter on it.
	Vector <Relationship> vEnemies;
	public:
	
		Social()
		{
		}
		unsigned char compatibilityWith(Social* social)
		{
			return compatibility.distanceTo(social->compatibility);
		}
		
};

#endif // WORLDSIM_SOCIAL_CPP
