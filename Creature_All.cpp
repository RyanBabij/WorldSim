#pragma once
#ifndef WORLDSIM_CREATURE_ALL_CPP
#define WORLDSIM_CREATURE_ALL_CPP

/* WorldSim: Creature_All.cpp
	#include"Creature_All.cpp"

  Implementation of Creature_All.hpp.
*/

#include "Creature_All.hpp"


Creature_Bat::Creature_Bat()
{
  nPelt=1;
  nMeat=2;
}
Creature_Bat::~Creature_Bat()
{
}

  //_sex: 0 - Roll, 1 - Male, 2 - Female.
void Creature_Bat::init(const int _sex  /*=0*/ )
{
  
  if (_sex == 1) { isMale = true; }
  else if (_sex == 2) { isMale = false; }
  else { isMale = Random::flip(); }
  
	age=0;
  map=0;
  knowledge = new Creature_Knowledge;
  knowledge->init();

}

void Creature_Bat::incrementTicks(int nTicks)
{
  
  if ( map == 0 ) { return; }
  
  //std::cout<<"batty tick\n";
  
  //std::cout<<"Finding nearest Character.\n";
  
  wander();
  
  int closestDistance = -1;
  Character* closestCharacter = 0;
  
  if (map!=0)
  {
    for (int i=0;i<map->vCharacter.size();++i)
    {
      Character* target = map->vCharacter(i);
      
      if ( isUnderground == target->isUnderground && ( closestCharacter == 0 || distanceTo(target) < closestDistance ))
      {
        closestCharacter = target;
        closestDistance = distanceTo(target);
      }
    }
  }
  
  
  if ( closestDistance != -1 && closestDistance < 2 )
  {
    //std::cout<<"Closest character is "<<closestDistance<<" tiles away\n";
    if ( closestCharacter == playerCharacter )
    {
      Console("A BAT ATTAC");
    }
  }
  
  
  //Bat AI will be simple. Fly around and attack any Characters nearby.
  
	secondsCounter+=nTicks;
	
	while(secondsCounter>=86400)
	{
		++daysCounter;
		secondsCounter-=86400;
	}
	while(daysCounter >=360)
	{
		age++;
		daysCounter-=360;
	}
}

Texture* Creature_Bat::currentTexture()
{
  if (isAlive)
  {
    return &TEX_CREATURE_BAT;
  }
  return &TEX_CREATURE_BAT_DED;
}
std::string Creature_Bat::getName()
{
  return "Bat";
}


#endif
