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

Texture* Creature_Deer::currentTexture()
{
  if ( isMale )
  {  return &TEX_WORLD_CREATURE_DEER_DEER;
  }
  return &TEX_WORLD_CREATURE_DEER_DOE;
}

#endif
