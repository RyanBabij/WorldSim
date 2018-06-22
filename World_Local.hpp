#pragma once
#ifndef WORLDSIM_WORLD_LOCAL_HPP
#define WORLDSIM_WORLD_LOCAL_HPP

#include <IdleTick/IdleTickInterface.hpp>
#include <LogicTick/LogicTickInterface.hpp>

/* World_Local.hpp
	#include "World_Local.hpp"

	Project: WorldSim
	Created: 10/12/2017, 0272632857.
	Updated: 10/12/2017, 0272632857.

	Description:
	The World stores local-level data, such as the position of local units.
	
	There is only one world, but each tile on the world is a potential local_world, therefore different techniques must be used to simulate them. For example
	they must be swapped into and out of memory because it is impossible to hold all of them in memory at once.

	Notes:

	0272632857 - Added basic documentation.

*/

class World_Local: public LogicTickInterface, public IdleTickInterface
{
	private:
	int globalX, globalY; /* The local world's position in the world. */
	
	public:
	World_Local();
	
		/* The size of the world, measured in tiles. */
	int nX, nY;
	
	bool active; /* Whether or not the world should be simulated. */
	bool generated; /* False until a world has been generated. Prevents trying to simulate a non-existent world. */
	
	long long unsigned int ticksBacklog; /* World will simulate these ticks whenever it can, while still relinquishing for input etc. */
	//Timer relinquishTimer;
	
	//RandomNonStatic random;

	
	ArrayS2 <bool> aIsLand;



	
	void init();
	
	bool generate();
	
	bool saveToFile(std::string /* path */);
	
};


#endif
