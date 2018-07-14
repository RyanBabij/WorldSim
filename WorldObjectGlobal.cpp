#pragma once
#ifndef GUILD_WORLDOBJECTGLOBAL_CPP
#define GUILD_WORLDOBJECTGLOBAL_CPP

/* WorldObjectGlobal.cpp
	#include "WorldObjectGlobal.cpp"

	Project: WorldSim
	Created: Unknown.
	Updated: 7/10/2017, 0271994297.

	Description:
	Interface for objects which have a presence on the world map.

	Notes:

	0271994297 - Updated basic documentation.
*/

WorldObjectGlobal::WorldObjectGlobal()
{
	worldX=-1;
	worldY=-1;
	world=0;
	type=NOTHING;
	name="?";
  nameType="?";
}

#endif
