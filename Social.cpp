#pragma once
#ifndef WORLDSIM_SOCIAL_CPP
#define WORLDSIM_SOCIAL_CPP

/* WorldSim: Social.cpp
	#include "Social.cpp"

	Manage social standings and interactions. Each character has a Social object.
	A person can have X friends and X enemies, where X is their charisma. A person is always able to know their
	immediate family.
	
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
	Vector <Relationship> vFamily; // Should probably be kept as its own thing
	Vector <Relationship> vFriends; // Probably should have a limiter on it.
	Vector <Relationship> vEnemies;
	Relationship candidate;
	public:
	
		Social()
		{
		}
		unsigned char compatibilityWith(Social* social)
		{
			return compatibility.distanceTo(social->compatibility);
		}
		
		WrappingUChar getCompatibility()
		{
			return compatibility;
		}
		void setCompatibility(unsigned char _compatibility)
		{
			compatibility=_compatibility;
		}
		void setCompatibility(Social* compatible)
		{
			compatibility=compatible->getCompatibility();
		}
		
		// Expand the network by looking at each friend and then their friends. If they find somebody more compatible
		// than their current friends, they will make them a candidate. Otherwise a random person will be picked as a
		// candidate.
		void expand()
		{
		}
		
		// void expand()
		// {
			// unsigned char bestCompatibility = 255; // Start with the worst compatibility
			// Character* mostCompatiblePerson = nullptr;

			// // Iterate through each friend
			// for (const Relationship& friendRel : vFriends)
			// {
				// Social* friendSocial = friendRel.destinationCharacter->getSocial(); // Assuming Character has a getSocial() method

				// // Iterate through each friend of a friend
				// for (const Relationship& foafRel : friendSocial->vFriends)
				// {
					// Social* foafSocial = foafRel.destinationCharacter->getSocial();

					// // Check if this friend-of-a-friend is already a friend or an enemy
					// bool alreadyKnown = std::any_of(vFriends.begin(), vFriends.end(), [&](const Relationship& rel) {
					// return rel.destinationCharacter == foafRel.destinationCharacter;
					// }) || std::any_of(vEnemies.begin(), vEnemies.end(), [&](const Relationship& rel) {
					// return rel.destinationCharacter == foafRel.destinationCharacter;
					// });

					// if (!alreadyKnown)
					// {
						// unsigned char currentCompatibility = compatibilityWith(foafSocial);

						// // Check if this person is more compatible than the current best
						// if (currentCompatibility < bestCompatibility)
						// {
							// bestCompatibility = currentCompatibility;
							// mostCompatiblePerson = foafRel.destinationCharacter;
						// }
					// }
				// }
			// }

			// // If a most compatible person was found, set them as a candidate
			// if (mostCompatiblePerson != nullptr)
			// {
				// candidate.sourceCharacter = this->sourceCharacter; // Assuming Social has a sourceCharacter member
				// candidate.destinationCharacter = mostCompatiblePerson;
				// candidate.relationshipLevel = bestCompatibility;
			// }
			// else
			// {
				// // Handle the case where no candidate was found, possibly by picking a random person
				// // or leaving the candidate as null.
			// }
		// }

		
};

#endif // WORLDSIM_SOCIAL_CPP
