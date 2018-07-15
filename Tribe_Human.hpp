#pragma once
#ifndef WORLDSIM_TRIBE_HUMAN_HPP
#define WORLDSIM_TRIBE_HUMAN_HPP

/* Tribe_Human.hpp
	#include "Tribe_Human.hpp"

	Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Human tribes build towns and cities.
  
*/

#include "Character.hpp"

class World;

#include "GuildCalendar.hpp"
#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>


class Tribe_Human: public Tribe
{
  private:
    int nArtifactsDropped;
  
	public:
  
  
	
	Tribe_Human();
	
	bool spawn();
  virtual void wander();
  virtual void incrementTicks ( int /* nTicks */ );
  
  void combat (Tribe* /* target */);
	
	Texture* currentTexture();
};

#endif
