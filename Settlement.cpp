#pragma once
#ifndef GUILD_SETTLEMENT_CPP
#define GUILD_SETTLEMENT_CPP

#include "Settlement.hpp"

Settlement::Settlement()
{
  
  name = "Settlement";
  world = 0;
  colourRed = 0;
  colourGreen = 0;
  colourBlue = 0;
  
  race = NONE;
  
}

Texture* Settlement::currentTexture()
{
	return &TEX_TEST_404;
}

  /* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement::incrementTicks ( int /* nTicks */ )
{
  return;
}

#endif
  