#pragma once
#ifndef WORLDSIM_TRIBE_DWARVEN_HPP
#define WORLDSIM_TRIBE_DWARVEN_HPP

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

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>


class Tribe_Dwarven: public Tribe
{
	public:
	
	Tribe_Dwarven()
	{
	}
	
	bool spawn();
	
	Texture* currentTexture();
};
#endif