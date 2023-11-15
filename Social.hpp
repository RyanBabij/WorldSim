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
		unsigned char compatibility; // lower is better

		Relationship(Character* _sourceCharacter, Character* _destinationCharacter, char _relationshipLevel,
		unsigned char _compatibility);
		
		// Operator overloads
		bool operator==(Relationship& other);
		bool operator>(Relationship& other);
		bool operator<(Relationship& other);
		
		/// cout
		friend std::ostream& operator<<(std::ostream& os, const Relationship& r);
};

class Social
{
	private:
		WrappingUChar personality; // Character's personalituy
		WrappingUChar desiredPersonality; // Personality that the Character likes
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
		/// cout
		friend std::ostream& operator<<(std::ostream& os, const Social& r);

		unsigned char compatibilityWith(Social* social);
		unsigned char compatibilityWith(Social& social);
		WrappingUChar getPersonality();
		WrappingUChar getDesiredPersonality();
		
		void setCompatibility(unsigned char _compatibility);
		void setCompatibility(unsigned char _compatibility, unsigned char _desiredPersonality);
		void setCompatibility(Social* compatible);
		void setCompatibility(Social& compatible);
		
		void setFullyCompatible(Social * compatible);
		void setFullyCompatible(Social & compatible);
		
		Relationship* getBestFriend();
		Relationship* getWorstFriend();
		int getWorstFriendSlot();
		
		int getWorstAcquaintanceSlot();

		int getFamilySlot(Character* c);
		int getFriendSlot(Character* c);
		int getEnemySlot(Character* c);
		int getAcquaintanceSlot(Character* c);
		
		void addFamily(Character* c);

		void interact(Character* c);
		void updateLists(int maxFriends);

		Vector<Relationship>& getAcquaintances();
		
		void print();
};

#endif // WORLDSIM_SOCIAL_HPP
