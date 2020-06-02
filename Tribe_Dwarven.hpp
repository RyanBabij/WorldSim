#pragma once
#ifndef WORLDSIM_TRIBE_DWARVEN_HPP
#define WORLDSIM_TRIBE_DWARVEN_HPP

/* Tribe_Dwarven.hpp
   #include "Tribe_Dwarven.hpp"

   Description:
   Tribes are nomadic groups. They wander the map trying to survive until they develop enough to become a civilization. Dwarven tribes build fortresses into mountains, and typically stay there growing crops and manufacturing items.

*/

#include "Character.hpp"

class World;

#include "WorldObjectGlobal.hpp"

#include <Container/Table/TableInterface.hpp>


class Tribe_Dwarven: public Tribe
{
   public:
  
  //RandomNonStatic random;
   
   Tribe_Dwarven();

   bool spawn();
  
    /* SIMULATE X TURNS OF THE CIV. */
  void incrementTicks ( int /* nTicks */ );
  
  void wander();
   
   Texture* currentTexture();
};
#endif