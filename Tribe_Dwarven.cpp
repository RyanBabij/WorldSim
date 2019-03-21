#pragma once
#ifndef WORLDSIM_TRIBE_DWARVEN_CPP
#define WORLDSIM_TRIBE_DWARVEN_CPP

/* WorldSim: Tribe_Dwarven.hpp
	#include "Tribe_Dwarven.hpp"

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Dwarven tribes build fortresses into mountains, and typically stay there growing crops and manufacturing items.

*/

#include "Tribe_Dwarven.hpp"

#include "Character.hpp"

class World;

#include "Time/Calendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

Tribe_Dwarven::Tribe_Dwarven()
{
  race = DWARVEN;
}


bool Tribe_Dwarven::spawn()
{
	
	if ( world == 0 )
	{ return false; }
	
  HasXY* spawnTile = world->getRandomTileOfType(MOUNTAIN);

	if ( spawnTile == 0 )
	{
    spawnTile = world->getRandomTileOfType(GRASSLAND);
	}
  
	if ( spawnTile == 0 )
	{
		std::cout<<"ABORT: Dwarf couldn't find tile to spawn into.\n";
		return false;
	}

	name = globalNameGen.generateName();
	nFood = 10;
	
	worldX=spawnTile->x;
	worldY=spawnTile->y;

	world->putObject(this,worldX,worldY);
	world->vTribe.push(this);
	
	return true;

}

			/* SIMULATE X TURNS OF THE CIV. */
void Tribe_Dwarven::incrementTicks ( int nTicks )
{
  actionPoints+=nTicks;
	dailyCounter+=nTicks;
	monthlyCounter+=nTicks;
  
	while (monthlyCounter >= 2592000)
	{
    for ( int i=0;i<30;++i)
    {
      
      
      if ( foundSettlement == false )
      {
        if ( world->getTileType(worldX,worldY) == "mountain"  && random.oneIn(10) )
        {
          foundSettlement = true;
          world->evolveToCiv(this);
          break;
        }
        else
        {
          wander();
        }
      }
      
      
    }
    
		monthlyCounter-=2592000;
	}
	
	while ( dailyCounter >= 86400 )
	{
    if ( foundSettlement == false )
    {
      if ( world->getTileType(worldX,worldY) == "mountain"  && random.oneIn(10) )
      {
        foundSettlement = true;
        world->evolveToCiv(this);
        break;
      }
      else
      {
        wander();
      }
    }
		dailyCounter-=86400;
  }
  

}

void Tribe_Dwarven::wander()
{
	if (world==0 || foundSettlement) { return; }
	
	int destinationX = worldX + random.randomIntRange(-1,1);
	int destinationY = worldY + random.randomIntRange(-1,1);
	
	// Moving options:
	//  Move to food
	//  Move to unexplored territory
	
	//aTerrain.getNeighborVector(_x,_y,&vTerrain,false /* DON'T INCLUDE SELF */);
	//Vector <HasXY*> * vXY  = world->aTerrain.getNeighbors(worldX, worldY, false);
	//vXY->shuffle();
	
	//HasXY* xyDestination = 0;

		/* If all else fails, move randomly. */
	if (world->isSafe(destinationX,destinationY) && world->isLand(destinationX,destinationY))
	{
		// MOVE THE TRIBE TO THE LOCATION.
		worldX=destinationX;
		worldY=destinationY;
	}
}


Texture* Tribe_Dwarven::currentTexture()
{
  if (foundSettlement)
  {
    return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
  }
	return &TEX_WORLD_UNIT_DWARF_01;
}

#endif