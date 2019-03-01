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

}

Texture* Creature_Deer::currentTexture()
{
  if ( isMale )
  {  return &TEX_WORLD_CREATURE_DEER_DEER;
  }
  return &TEX_WORLD_CREATURE_DEER_DOE;
}
std::string Creature_Deer::getName()
{
  return "Deer";
}

#endif
