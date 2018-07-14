#pragma once
#ifndef WORLDSIM_TRIBE_HUMAN_CPP
#define WORLDSIM_TRIBE_HUMAN_CPP

/* Tribe_Human.hpp
	#include "Tribe_Human.hpp"

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Human tribes build towns and cities.

*/

#include "Tribe_Human.hpp"

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>

#include "WorldObjectGlobal_TribalArtifact.cpp"

Tribe_Human::Tribe_Human()
{
  race = HUMAN;
  nArtifactsDropped = 0;
  nameType = "Human Tribe";
  name = "HUM";
}



bool Tribe_Human::spawn()
{
	//return false;
	
	if ( world == 0 )
	{
		//std::cout<<"ABORT: Tribe doesn't have pointer to world object.\n";
		return false;
	}
	
	HasXY* spawnTile = world->getRandomTileOfType(GRASSLAND);
	
	if ( spawnTile == 0 )
	{
		std::cout<<"ABORT: Human couldn't find tile to spawn into.\n";
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


Texture* Tribe_Human::currentTexture()
{
	if ( foundSettlement )
	{
		return &TEX_WORLD_SETTLEMENT_TOWN_URBAN01;
	}
	return &TEX_WORLD_UNIT_NOMAD_01;
}

void Tribe_Human::wander()
{
	if (world==0) { return; }
	
	int destinationX = worldX + random.randomIntRange(-1,1);
	int destinationY = worldY + random.randomIntRange(-1,1);
  
  
    // Sometimes drop an artifact on the map. (max 3)
  if (nArtifactsDropped < 3 && random.oneIn(1000))
  {
    TribalArtifact * testArtifact = new TribalArtifact;
    testArtifact->worldX = worldX;
    testArtifact->worldY = worldY;
    
    world->putObject(testArtifact);
    ++nArtifactsDropped;
  }
	
	// Moving options:
	//  Move to food
	//  Move to unexplored territory
	
	//aTerrain.getNeighborVector(_x,_y,&vTerrain,false /* DON'T INCLUDE SELF */);
	Vector <HasXY*> * vXY  = world->aTerrain.getNeighbors(worldX, worldY, false);
	vXY->shuffle();
	
	HasXY* xyDestination = 0;
	
	// Explore new territory
	if (random.oneIn(4))
	{
		//std::cout<<"1\n";
		for (auto xy : *vXY)
		{
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
				if ( world->getHighestInfluence(xy) == 0 )
				{
					xyDestination=xy;
					
					worldX=xy->x;
					worldY=xy->y;
					return;
				}
			}
		}
	}
	
	//if ( random.oneIn(2) )
	{
			/* If there is no unowned territory. Prefer own territory. Prefer weakest influence tiles to promote holding on to territory */
		for (auto xy : *vXY)
		{
			
			int weakestInfluence = -1;
			HasXY * weakestTile = 0;
			
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
				if ( world->getDominantInfluence(xy) == this)
				{
					if ( weakestInfluence == -1 || weakestTile == 0 || world->getHighestInfluence(xy) < weakestInfluence)
					{
						xyDestination=xy;
						weakestInfluence = world->getHighestInfluence(xy);
					}
				}
			}
		}
		if ( xyDestination != 0)
		{
			worldX=xyDestination->x;
			worldY=xyDestination->y;
			return;
		}
	}
	
		/* Sometimes move to a weak territory to reinforce/capture it */
	if ( random.oneIn(3) )
	{
		//std::cout<<"2\n";
			/* Move to weakest territory */
		int weakestInfluence = -1;
		HasXY * weakestTile = 0;
			
		for (auto xy : *vXY)
		{
			if (world->aTerrain.isSafe(xy) && world->isLand(xy) )
			{
				if ( world->getHighestInfluence(xy)>0 && world->getHighestInfluence(xy) < weakestInfluence)
				{
					xyDestination=xy;
					weakestInfluence = world->getHighestInfluence(xy);
				}
			}
		}
		if ( xyDestination != 0)
		{
			worldX=xyDestination->x;
			worldY=xyDestination->y;
			return;
		}
	}
	
	

		


	//std::cout<<"4\n";
		/* If all else fails, move randomly. */
	if (world->isSafe(destinationX,destinationY) && world->isLand(destinationX,destinationY))
	{
		// MOVE THE TRIBE TO THE LOCATION.
		worldX=destinationX;
		worldY=destinationY;
	}
}

#endif