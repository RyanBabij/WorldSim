#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_CPP
#define WORLDSIM_WORLD_LOCAL_CPP

/* World_Local.hpp
	#include "World_Local.cpp"

	Project: WorldSim
	Created: 10/12/2017, 0272632857.
	Updated: 10/12/2017, 0272632857.

	Description:
	The World stores local-level data, such as the position of local units.

	Notes:

	0272632857 - Added basic documentation.

*/



#include "World_Local.hpp"

World_Local::World_Local()
{
	globalX=0;
	globalY=0;
	
	nX=0;
	nY=0;
	active=false;
	generated=false;
}

void World_Local::init(int _x, int _y)
{
  globalX = _x;
  globalY = _y;
  
  aLocalTile.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
  aSubterranean.initClass(LOCAL_MAP_SIZE,LOCAL_MAP_SIZE);
}

bool World_Local::generate()
{
	
	aIsLand.init(3000,3000,false);
	return false;
}

bool World_Local::saveToFile(std::string _path)
{
	return false;
}


#endif
