#pragma once
#ifndef WORLDSIM_TRIBE_DWARVEN_CPP
#define WORLDSIM_TRIBE_DWARVEN_CPP

/* Tribe_Dwarven.hpp
	#include "Tribe_Dwarven.hpp"

	Project: WorldSim
	Created: 14/11/2017. 0272374355.
	Updated: 14/11/2017. 0272374355.

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization.
	
	Dwarven tribes build fortresses into mountains, and typically stay there growing crops and manufacturing items.

	Notes:

	0272374355 - Created.

*/

#include "Tribe_Dwarven.hpp"

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>


bool Tribe_Dwarven::spawn()
{
	//return false;
	
	if ( world == 0 )
	{
		//std::cout<<"ABORT: Tribe doesn't have pointer to world object.\n";
		return false;
	}
	
	HasXY* spawnTile = world->getRandomTileOfType(World::MOUNTAIN);
	
	if ( spawnTile == 0 )
	{
		std::cout<<"ABORT: Couldn't find mountain tile.\n";
		return false;
	}

	name = globalNameGen.generateName();
	nFood = 10;
	
	worldX=spawnTile->x;
	worldY=spawnTile->y;

	world->putObject(this);
	world->vTribe.push(this);
	
	return true;

}




Texture* Tribe_Dwarven::currentTexture()
{
	return &TEX_WORLD_UNIT_DWARF_01;
}

#endif