#pragma once
#ifndef GUILD_SETTLEMENT_CPP
#define GUILD_SETTLEMENT_CPP

#include "Settlement.hpp"

Settlement::Settlement()
{
  
  name = "";
  world = 0;
  colourRed = 0;
  colourGreen = 0;
  colourBlue = 0;
  
  race = NONE;
  
}

Texture* Settlement::currentTexture()
{
	return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

#endif
  