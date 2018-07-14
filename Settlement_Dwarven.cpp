#pragma once
#ifndef GUILD_SETTLEMENT_DWARVEN_CPP
#define GUILD_SETTLEMENT_DWARVEN_CPP

#include "Settlement_Dwarven.hpp"

Settlement_Dwarven::Settlement_Dwarven()
{
  
  name = "Dwarven Settlement";
  world = 0;
  colourRed = 0;
  colourGreen = 0;
  colourBlue = 0;
  
  race = DWARVEN;
  
}

Texture* Settlement_Dwarven::currentTexture()
{
	return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

  /* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement_Dwarven::incrementTicks ( int /* nTicks */ )
{
 // std::cout<<"Dwarven set increment.\n";

  if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
  {
   // std::cout<<"Mining\n";
  }
  return;
}


#endif
  