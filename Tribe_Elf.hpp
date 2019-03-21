#pragma once
#ifndef WORLDSIM_TRIBE_ELF_HPP
#define WORLDSIM_TRIBE_ELF_HPP

/* WorldSim: Tribe_Elf.hpp
	#include "Tribe_Elf.hpp"

	Description:
	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Elven tribes live in forests and jungles. They know a lot about the magic arts.

*/

#include "Character.hpp"

class World;

#include "Time/Calendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>


class Tribe_Elf: public Tribe
{
	public:
  
  //RandomNonStatic random;
	
	Tribe_Elf();

	bool spawn();
  
    /* SIMULATE X TURNS OF THE CIV. */
  void incrementTicks ( int /* nTicks */ );
  
  void wander();
	
	Texture* currentTexture();
};

#endif