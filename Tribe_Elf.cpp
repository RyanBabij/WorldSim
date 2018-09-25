#pragma once
#ifndef WORLDSIM_TRIBE_ELF_CPP
#define WORLDSIM_TRIBE_ELF_CPP

/* WorldSim: Tribe_Elf.hpp
	#include "Tribe_Elf.hpp"

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Elven tribes live in forests and jungles. They know a lot about the magic arts.

*/

#include "Tribe_Elf.hpp"

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

Tribe_Elf::Tribe_Elf()
{
  race = ELVEN;
}


bool Tribe_Elf::spawn()
{
	
	if ( world == 0 )
	{
		return false;
	}
  
  HasXY* spawnTile = 0;
  if (Random::oneIn(3) )
  {
    spawnTile = world->getRandomTileOfType(FOREST);
  }
  else
  {
    spawnTile = world->getRandomTileOfType(JUNGLE);
  }
	if ( spawnTile == 0 )
	{
    spawnTile = world->getRandomTileOfType(GRASSLAND);
	}
  
	if ( spawnTile == 0 )
	{
		std::cout<<"ABORT: Elf couldn't find tile to spawn into.\n";
		return false;
	}

	name = globalNameGen.generateName();
	nFood = 10;
	
	worldX=spawnTile->x;
	worldY=spawnTile->y;
  delete spawnTile;

	world->putObject(this,worldX,worldY);
	world->vTribe.push(this);
	
	return true;

}

			/* SIMULATE X TURNS OF THE CIV. */
void Tribe_Elf::incrementTicks ( int nTicks )
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
        if ( world->getTileType(worldX,worldY) == "forest"  && random.oneIn(10) )
        {
          //foundSettlement = true;
          //world->evolveToCiv(this);
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
      if ( world->getTileType(worldX,worldY) == "forest"  && random.oneIn(10) )
      {
        //foundSettlement = true;
        //world->evolveToCiv(this);
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

void Tribe_Elf::wander()
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


Texture* Tribe_Elf::currentTexture()
{
  if (foundSettlement)
  {
    return &TEX_WORLD_SETTLEMENT_DWARFFORT_01;
  }
	return &TEX_WORLD_UNIT_ELF_01;
}

#endif