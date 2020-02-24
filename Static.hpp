#pragma once
#ifndef WORLDSIM_STATIC_HPP
#define WORLDSIM_STATIC_HPP

/* WorldSim:Static.hpp
   #include "Static.hpp"

   Statics are objects which can't move on their own, and therefore
   they don't need to know their coordinates.
   They also only tick once per day.

   Static subclasses:
      Plant
      Tree
      Building
      Furniture

   Statics may block movement and/or LOS
*/

class Static: public HasTexture
{
   public:
   unsigned char blockMove; // bitfield blocks movement in direction starting NW and going clockwise
   unsigned char blockLOS; // bitfield blocks LOS in direction starting NW and going clockwise
   
   // HP
   // DAMAGE RESIST
   
   Static()
   {
      blockMove=0;
      blockLOS=0;
   }
   
   virtual Texture* currentTexture()
   {
      return 0;
   }
};

// object capable of holding Ingredients
class Harvestable
{
   public:
};


// Abstract flora plan.

// alternatively: flora ID (0-255) -> lookup table.
// flora stats update once per 24 hours on average.
// flora growth/death won't be modelled when player is absent because it won't be noticable.
// each biome gets its own list of flora

class Flora: public Static
{
   // Easy food: Food which can easily be obtained/eaten such as fruit
   // Medium food: Food which is moderately easy to obtain/eat such as bark
   // Hard food: Food which is hard to obtain/eat such as leaves
   // This should allow a balance of large and slow, and small and fast herbivores
   // Increment is per 24 hours
   // 1 food = 1 day of food for 1 herbivore
   int easyFood, maxEasyFood, easyFoodIncrement;
   int mediumFood, maxMediumFood, mediumFoodIncrement;
   int hardFood, maxHardFood, hardFoodIncrement;
   
   public:
   Flora()
   {
   }
};

class Static_Tree: public Static
{
   public:
   
   int chopAmount;
   int growth;
   
   Static_Tree(int _growth=100)
   {
      chopAmount=100;
      growth=_growth;
      
      if ( growth > 0 )
      {
         blockMove = 255;
         blockLOS = 255;
      }
      else
      {
         blockMove = 0;
         blockLOS = 0;
      }
   }
   
   Texture* currentTexture() override
   {
      if (chopAmount==0)
      { return &TEX_OBJECT_STUMP;
      }

      if ( growth==0 )
      {
         return &TEX_WORLD_TERRAIN_FOREST_SAPLING;
      }
      return &TEX_WORLD_TERRAIN_FOREST_TREE;
   }
};

#endif
