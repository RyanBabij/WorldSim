#pragma once
#ifndef WORLDSIM_CREATURE_DEER_CPP
#define WORLDSIM_CREATURE_DEER_CPP

/* WorldSim: Creature_Deer.cpp
	#include"Creature_Deer.cpp"

  Implementation of Deer_hpp.
*/

#include "Creature_Deer.hpp"


Creature_Deer::Creature_Deer()
{
  nPelt=1;
  nMeat=2;
}
Creature_Deer::~Creature_Deer()
{
}

  //_sex: 0 - Roll, 1 - Male, 2 - Female.
void Creature_Deer::init(const int _sex  /*=0*/ )
{
  
  if (_sex == 1) { isMale = true; }
  else if (_sex == 2) { isMale = false; }
  else { isMale = Random::flip(); }
  
	age=0;
  map=0;
  knowledge = new Creature_Knowledge;
  knowledge->init();

}

Texture* Creature_Deer::currentTexture()
{

  if ( isMale )
  {
    if (isAlive)
    {
      return &TEX_CREATURE_DEER;
    }
    return &TEX_CREATURE_DEER_DED;
  }
  if (isAlive)
  {
    return &TEX_CREATURE_DOE;
  }
  return &TEX_CREATURE_DOE_DED;
}
std::string Creature_Deer::getName()
{
  return "Deer";
}

#endif
