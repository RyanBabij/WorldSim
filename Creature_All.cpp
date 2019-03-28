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
