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

#endif