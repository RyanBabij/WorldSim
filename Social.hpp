#pragma once
#ifndef WORLDSIM_SOCIAL_HPP
#define WORLDSIM_SOCIAL_HPP

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

		unsigned char compatibilityWith(Social* social);
		unsigned char compatibilityWith(Social& social);
		WrappingUChar getCompatibility();
		void setCompatibility(unsigned char _compatibility);
		void setCompatibility(Social* compatible);
		void setCompatibility(Social& compatible);

		int isFamily(Character* c);
		int isFriend(Character* c);
		int isEnemy(Character* c);
		int isAcquaintance(Character* c);

		void interact(Character* c);
		void updateLists(int maxFriends);

		Vector<Relationship>& getAcquaintances();
};

#endif // WORLDSIM_SOCIAL_HPP
