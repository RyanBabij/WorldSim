#pragma once
#ifndef WORLDSIM_SOCIAL_HPP
#define WORLDSIM_SOCIAL_HPP

/* WorldSim: Social.hpp
	#include "Social.hpp"

	Manage social standings and interactions. Each character has a Social object.
	A person can have X friends and X enemies, where X is their charisma. A person is always able to know their
	immediate family.

	Compatibility determines whether a person becomes a friend more than anything else. This is not a perfect system
	as it could create a long circuit of relationships but it should work well enough. It also adds an element of
	chance beyond just charisma detemining who will like you.
	
	Currently compatibility is mutually decided. An alternative system might be better.
	This would work by having a second compatibility. SelfCompatibility and DesiredCompatibility.
	
	Note that if you have charisma 0, you will have 0 friends.

*/

#include "Character.hpp"
#include <Math/WrappingUChar.cpp>
#include <vector>

class Relationship
{
	public:
		Character* sourceCharacter;
		Character* destinationCharacter;
		char relationshipLevel;
		unsigned char compatibility;

		Relationship(Character* _sourceCharacter, Character* _destinationCharacter, char _relationshipLevel,
		unsigned char _compatibility);
		
		// Operator overloads
		bool operator==(Relationship& other);
		bool operator>(Relationship& other);
		bool operator<(Relationship& other);
};

class Social
{
	private:
		WrappingUChar compatibility; 
		Vector<Relationship> vFamily;
		Vector<Relationship> vAcquaintance;
		Vector<Relationship> vFriend;
		Vector<Relationship> vEnemy;

		Character* thisCharacter;

	public:
		Social(Character* _thisCharacter);

		// Operator Overloads
		// bool operator==(Social& other);
		// bool operator>(Social& other);
		// bool operator<(Social& other);

		unsigned char compatibilityWith(Social* social);
		unsigned char compatibilityWith(Social& social);
		WrappingUChar getCompatibility();
		void setCompatibility(unsigned char _compatibility);
		void setCompatibility(Social* compatible);
		void setCompatibility(Social& compatible);
		
		Relationship* getBestFriend();
		Relationship* getWorstFriend();
		int getWorstFriendSlot();

		int isFamily(Character* c);
		int isFriend(Character* c);
		int isEnemy(Character* c);
		int isAcquaintance(Character* c);

		void interact(Character* c);
		void updateLists(int maxFriends);

		Vector<Relationship>& getAcquaintances();
};

#endif // WORLDSIM_SOCIAL_HPP
