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
  
  nMetalStockpile=0;
  monthlyCounter=0;
  dailyCounter=0;
  
}

Texture* Settlement_Dwarven::currentTexture()
{
   return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
}

  /* SIMULATE X TURNS OF THE SETTLEMENT. */
void Settlement_Dwarven::incrementTicks ( int nTicks )
{
   dailyCounter+=nTicks;
   monthlyCounter+=nTicks;



  
   while (monthlyCounter >= 2592000)
   {
    if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
    {
      nMetalStockpile+=30;
    }

      monthlyCounter-=2592000;
   }
  
   while ( dailyCounter >= 86400 )
  {
    // if ( world->aWorldTile(worldX,worldY).baseMetal > 0 )
    // {
      // ++nMetalStockpile;
    // }
      dailyCounter-=86400;
   }
  
  return;
}


#endif
  