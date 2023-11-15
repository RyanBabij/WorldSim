#pragma once
#ifndef WORLDSIM_SOCIAL_CPP
#define WORLDSIM_SOCIAL_CPP

#include "Social.hpp"

Relationship::Relationship(Character* _sourceCharacter, Character* _destinationCharacter, char _relationshipLevel,
unsigned char _compatibility) : sourceCharacter(_sourceCharacter), destinationCharacter(_destinationCharacter),
relationshipLevel(_relationshipLevel), compatibility(_compatibility)
{
}

bool Relationship::operator==(Relationship& other)
{
	return this->compatibility == other.compatibility;
}

bool Relationship::operator>(Relationship& other)
{
	return this->compatibility > other.compatibility;
}

bool Relationship::operator<(Relationship& other)
{
	return this->compatibility < other.compatibility;
}

// Implementation of the << operator
std::ostream& operator<<(std::ostream& os, const Relationship& r)
{
	return os << r.sourceCharacter->social << ", " << r.destinationCharacter->social << ": "<< static_cast<int>(r.compatibility);
}

Social::Social(Character* _thisCharacter)
: thisCharacter(_thisCharacter)
{
}

// Operator Overloads
// bool Social::operator==(Social& other)
// {
	// return (this->compatibility == other.compatibility);
// }

// bool Social::operator>(Social& other)
// {
	// return (this->compatibility > other.compatibility);
// }

// bool Social::operator<(Social& other)
// {
	// return (this->compatibility < other.compatibility);
// }

// Implementation of the << operator
std::ostream& operator<<(std::ostream& os, const Social& s)
{
	return os << s.thisCharacter->getFullName()<<": "<<s.personality<<", "<<s.desiredPersonality;
}

unsigned char Social::compatibilityWith(Social* social)
{
	return desiredPersonality.distanceTo(social->personality);
}

unsigned char Social::compatibilityWith(Social& social)
{
	return desiredPersonality.distanceTo(social.personality);
}

WrappingUChar Social::getPersonality()
{
	return personality;
}

WrappingUChar Social::getDesiredPersonality()
{
	return desiredPersonality;
}


void Social::setCompatibility(unsigned char _personality, unsigned char _desiredPersonality)
{
	personality = _personality;
	desiredPersonality = _desiredPersonality;
}

void Social::setCompatibility(Social* compatible)
{
	personality = compatible->getPersonality();
	desiredPersonality = compatible->getDesiredPersonality();
}

void Social::setCompatibility(Social& compatible)
{
	personality = compatible.getPersonality();
	desiredPersonality = compatible.getDesiredPersonality();
}

void Social::setFullyCompatible(Social* compatible)
{
	personality = compatible->getDesiredPersonality();
	desiredPersonality = compatible->getPersonality();
}

void Social::setFullyCompatible(Social& compatible)
{
	personality = compatible.getDesiredPersonality();
	desiredPersonality = compatible.getPersonality();
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
	if ( c==thisCharacter )
	{
		// idk meditate or something
		return;
	}
	else if (isFamily(c)!=-1)
	{
		return;
	}
	else if (isFriend(c)!=-1)
	{
		return;
	}
	else if (isEnemy(c)!=-1)
	{
		return;
	}
	else if (isAcquaintance(c)!=-1)
	{
		return;
	}
	
	// Add new acquaintance
	vAcquaintance.push(Relationship(thisCharacter,c,1,compatibilityWith(c->social)));
	
}

Relationship* Social::getBestFriend()
{
	if (vFriend.empty())
	{
		return nullptr;
	}

	Relationship* bestFriend = &vFriend(0);
	unsigned char bestCompatibility = vFriend(0).compatibility;

	for (int i = 1; i < vFriend.size(); ++i)
	{
		unsigned char thisCompatibility = vFriend(i).compatibility;
		
		if (thisCompatibility < bestCompatibility)
		{
			bestCompatibility = thisCompatibility;
			bestFriend = &vFriend(i);
		}
	}

	return bestFriend;
}

Relationship* Social::getWorstFriend()
{
	if (vFriend.empty())
	{
		return nullptr; // Return nullptr if there are no friends
	}

	Relationship* worstFriend = nullptr;
	unsigned char worstCompatibility = 0; // Start with the lowest possible compatibility

	for (int i = 0; i < vFriend.size(); ++i)
	{
		unsigned char thisCompatibility = vFriend(i).compatibility;

		if (thisCompatibility > worstCompatibility)
		{
			worstCompatibility = thisCompatibility;
			worstFriend = &vFriend(i);
		}
	}

	return worstFriend; // Return the pointer to the worst friend
}

int Social::getWorstFriendSlot()
{
	if (vFriend.empty())
	{
		return -1; // Return -1 if there are no friends
	}

	int worstFriend = -1;
	int worstCompatibility = -1; // Start with the lowest possible compatibility

	for (int i = 0; i < vFriend.size(); ++i)
	{
		unsigned char thisCompatibility = vFriend(i).compatibility;

		if (thisCompatibility > worstCompatibility)
		{
			worstCompatibility = thisCompatibility;
			worstFriend = i;
		}
	}
	return worstFriend; // Return the index to the worst friend
}

// Move most compatible acquaintances into friendship vector.
void Social::updateLists(int maxFriends)
{
	if (maxFriends==0)
	{
		return;
	}
	
	//std::cout<<"Update lists for: "<<thisCharacter->getFullName()<<"\n";
	
	int bestCompatibility = 666;
	int bestAcquaintance = -1;
	// find best acquaintance
	for (int i=0;i<vAcquaintance.size();++i)
	{
		unsigned char thisCompatibility = vAcquaintance(i).compatibility;
		
		if (thisCompatibility < bestCompatibility)
		{
			bestCompatibility = thisCompatibility;
			bestAcquaintance = i;
		}
	}
	
	int worstFriend = getWorstFriendSlot();
	
	int worstFriendCompatibility = 666;
	if ( worstFriend!=-1)
	{
		worstFriendCompatibility = vFriend(worstFriend).compatibility;
	}
	
	if(bestAcquaintance==-1)
	{
		//std::cout<<"No new acquaintances found.\n";
		return;
	}
	
	//std::cout<<"Worst friend / alternative compat is: "<<worstFriendCompatibility<<" / "<<bestCompatibility<<"\n";

	if ( bestCompatibility < worstFriendCompatibility )
	{
		if (vFriend.size()<maxFriends)
		{
			//std::cout<<"Adding friend\n";
			vFriend.add(vAcquaintance(bestAcquaintance));
			vAcquaintance.removeSlot(bestAcquaintance);
		}
		else
		{
			//std::cout<<"Replacing friend\n";
			if (worstFriend!=-1)
			{
				vFriend.removeSlot(worstFriend);
			}
			else
			{
				//std::cout<<"Worstfriend returning -1\n";
			}
			vFriend.add(vAcquaintance(bestAcquaintance));
		}
	}
	else
	{
		//std::cout<<"No better friend found\n";
	}

	
}
		
Vector<Relationship>& Social::getAcquaintances()
{
	return vAcquaintance;
}

void Social::print()
{
	std::cout<<"   Deets for "<<thisCharacter->getFullName()<<":\n";
	
	std::cout<<" Acquaintances:\n";
	for (int i=0;i<vAcquaintance.size();++i)
	{
		std::cout<<vAcquaintance(i)<<"\n";
	}
	std::cout<<" Friends:\n";
	for (int i=0;i<vFriend.size();++i)
	{
		std::cout<<vFriend(i)<<"\n";
	}
}

#endif // WORLDSIM_SOCIAL_CPP
